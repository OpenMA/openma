/* 
 * Open Source Movement Analysis Library
 * Copyright (C) 2016, Moveck Solution Inc., all rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyright holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "bsfhandler.h"

#include "openma/io/handler_p.h"
#include "openma/io/device.h"
#include "openma/io/binarystream.h"
#include "openma/io/enums.h"
#include "openma/io/utils.h"
#include "openma/base/any.h"
#include "openma/base/node.h"
#include "openma/base/trial.h"
#include "openma/base/timesequence.h"
#include "openma/base/event.h"
#include "openma/base/logger.h"

#include "openma/instrument/forceplatetype2.h"

#include <string>
#include <list>
#include <vector>
#include <tuple>
#include <array>
#include <memory> // std::unique_ptr
#include <functional> // std::function
#include <cassert>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace io
{
  _OPENMA_CONSTEXPR int _bsf_maximum_channel_number = 32;
  
  // Internal structure used to store the configuration of each instrument (as proposed in the BSF file format documentation).
  struct BSFInstrumentHeader
  {
    int32_t headerSize;
    int32_t serialNumber;
    int32_t layoutPlatformNumber;
    std::string name;
    int32_t numberOfChannels;
    int32_t hardwareStartChannel;
    int32_t hardwareEndChannel;
    int32_t softwareStartChannel;
    int32_t softwareEndChannel;
    float length;
    float width;
    float offset[3];
    float sensitivity[_bsf_maximum_channel_number];
    int16_t channels[_bsf_maximum_channel_number];
    float transformation[16]; // 4x4 transformation matrix
    float interDistance[3];
    float amplifierGain[_bsf_maximum_channel_number];
    float excitationVoltage[_bsf_maximum_channel_number];
    float acquisitionCardRange[_bsf_maximum_channel_number]; // bits/volt
    float zeroPeriod;
    float latencyPeriod;
    float triggerTime;
    float endTime;
    float postTrigTime;
    int32_t zero[_bsf_maximum_channel_number];
    int32_t rate;
    float triggerValue;
    float endValue;
  };
  
  void _bsf_extract_instrument_header(BinaryStream* stream, BSFInstrumentHeader* header)
  {
    header->headerSize = stream->readI32();
    header->serialNumber = stream->readI32();
    header->layoutPlatformNumber = stream->readI32();
    header->name = trim_string(stream->readString(20));
    header->numberOfChannels = stream->readI32();
    header->hardwareStartChannel = stream->readI32();
    header->hardwareEndChannel = stream->readI32();
    header->softwareStartChannel = stream->readI32();
    header->softwareEndChannel = stream->readI32();
    header->length = stream->readFloat();
    header->width = stream->readFloat();
    stream->readFloat(3, header->offset);
    stream->readFloat(_bsf_maximum_channel_number, header->sensitivity);
    for (int i = 0 ; i < _bsf_maximum_channel_number ; ++i)
    {
      header->channels[i] = stream->readI16();
      stream->device()->seek(2, Origin::Current);
    }
    stream->readFloat(16, header->transformation);
    stream->readFloat(3, header->interDistance);
    stream->readFloat(_bsf_maximum_channel_number, header->amplifierGain);
    stream->readFloat(_bsf_maximum_channel_number, header->excitationVoltage);
    stream->readFloat(_bsf_maximum_channel_number, header->acquisitionCardRange);
    header->zeroPeriod = stream->readFloat();
    header->latencyPeriod = stream->readFloat();
    header->triggerTime = stream->readFloat();
    header->endTime = stream->readFloat();
    header->postTrigTime = stream->readFloat();
    stream->readI32(_bsf_maximum_channel_number, header->zero);
    header->rate = stream->readI32();
    header->triggerValue = stream->readFloat();
    header->endValue = stream->readFloat();
  };
  
  void _bsf_extract_fp_configuration(const BSFInstrumentHeader* header, const float* go, int16_t* ci, double* c, double* o)
  {
    // Channel index
    for (int i = 0 ; i < 6 ; ++i)
      ci[i] = header->channels[i]; // This is a 0-based index
    // Corners expressed in the global frame.
    // The global frame is set here as: axis X going forward, axis Y on the left and axis Z going upward.
    // The corners are set to have the corner #1 on the bottom left side, #2 on the top left side, #3 on the top right side and #4 on the bottom right side.
    const float cx = header->width / 2.0f;
    const float cy = header->length / 2.0f;
    c[0] =    cx + go[0];
    c[1] =   -cy + go[1];
    c[2] =  0.0f + go[2];
    c[3] =   -cx + go[0];
    c[4] =   -cy + go[1];
    c[5] =  0.0f + go[2];
    c[6] =   -cx + go[0];
    c[7] =    cy + go[1];
    c[8] =  0.0f + go[2];
    c[9] =    cx + go[0];
    c[10] =   cy + go[1];
    c[11] = 0.0f + go[2];
    // - Origin (expressed in the global frame) and centered above the origin of the global frame
    o[0] = header->offset[0];
    o[1] = header->offset[1];
    o[2] = header->offset[2];
  };
};
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace io
{
  
 /*
  * The BSF file format is a proprietary file format developed by the AMTI company (Watertown, USA).
  * The reader of this file format was implemented using the documentation of the software NetForce (appendix B), graciously offered by the AMTI company.
  * @warning The implementation of the writer is restricted and required first the permission of the AMTI company.
  * @todo Still need more checks when there are extra instruments, what about the instrument offset, coord, interdist and zero?
  */

  BSFHandler::BSFHandler()
  : Handler()
  {};
  
  BSFHandler::~BSFHandler() _OPENMA_NOEXCEPT = default;
  
  Signature BSFHandler::verifySignature(const Device* const device) _OPENMA_NOEXCEPT
  {
    char signature[4] = {0};
    device->peek(signature,sizeof(signature));
    if ((signature[0] != 0x64) || (signature[1] != 0x00) || (signature[2] != 0x00) || (signature[3] != 0x00))
      return Signature::Invalid;
    return Signature::Valid;
  };
  
  Signature BSFHandler::verifySignature() const _OPENMA_NOEXCEPT
  {
    return verifySignature(this->device());
  };
  
  void BSFHandler::readDevice(Node* output)
  {
    BinaryStream stream(this->device(), ByteOrder::IEEELittleEndian);
    this->device()->setExceptions(State::End | State::Fail | State::Error);

    // Main header (SMnHeaderTag)
    if (stream.readI32() != 100)
      throw FormatError("Wrong header key");
    int32_t headerSize = stream.readI32();
    int32_t numberOfActivePlatforms = stream.readI32();
    int32_t numberOfActiveInstruments = stream.readI32();
    std::string subjectName = trim_string(stream.readString(100));
    std::string testDate = trim_string(stream.readString(12));
    std::string subjectDateOfBirth = trim_string(stream.readString(12));
    double weight = stream.readDouble();
    double height = stream.readDouble();
    std::string sex = trim_string(stream.readString(1));
    this->device()->seek(3, Origin::Current); // Because of "pragma pack(4)": minimum of 4 bytes in the alignment of a member.
    int32_t numberOfTrials = stream.readI32();
    double totalTimeTrial = stream.readDouble(); // seconds
    if (totalTimeTrial <= 0)
      throw(FormatError("Invalid trial time."));
    int32_t zeroMethod = stream.readI32();
    int32_t weightMethod = stream.readI32();
    int32_t delayAfterKeystroke = stream.readI32();
    int32_t triggerMethod = stream.readI32();
    int32_t triggerPlatform = stream.readI32();
    int32_t preTriggerValue = stream.readI32();
    int32_t postTriggerValue = stream.readI32();
    double triggerValue = stream.readDouble();
    this->device()->seek(4, Origin::Current); // FIXME: There is 4 extra bytes in the file used to test this reader! What are they?
    int32_t rate = stream.readI32();
    std::string protocol = trim_string(stream.readString(150));
    std::string testType = trim_string(stream.readString(200));
    std::string commentFile = trim_string(stream.readString(150));
    std::string trialDescriptionFile = trim_string(stream.readString(150));
    std::string examinerName = trim_string(stream.readString(100));
    this->device()->seek(2, Origin::Current); // WARNING: Two (2) extra bytes in the file tested. Could be for alignment?
    int32_t units = stream.readI32(); // 0: english, 1: metric
    if (rate <= 0)
      throw(FormatError("Invalid sample rate."));
    // Instrument headers (SInstrHeaderTag)
    std::vector<BSFInstrumentHeader> instrumentHeaders(numberOfActivePlatforms + numberOfActiveInstruments);
    for (int i = 0 ; i < numberOfActivePlatforms ; ++i)
      _bsf_extract_instrument_header(&stream, instrumentHeaders.data()+i);
    for (int i = 0 ; i < numberOfActiveInstruments ; ++i)
      _bsf_extract_instrument_header(&stream, instrumentHeaders.data()+numberOfActivePlatforms+i);
    // FIXME Current limitaiton of the header
    for (int i = 0 ; i < numberOfActivePlatforms ; ++i)
    {
      if (instrumentHeaders[i].numberOfChannels != 6)
        throw(FormatError("Only force plate with 6 channels is supported. Contact the support to improve this parser."));
    }
    // Init
    int totalNumberOfChannels = 0;
    for (size_t i = 0 ; i < instrumentHeaders.size() ; ++i)
      totalNumberOfChannels += instrumentHeaders[i].numberOfChannels;
    std::vector<double> scale(totalNumberOfChannels); // array to transform ADC values to real values
    size_t inc = 0;
    for (size_t i = 0 ; i < instrumentHeaders.size() ; ++i)
    {
      for (int j = 0 ; j < instrumentHeaders[i].numberOfChannels ; ++j)
        scale[j+inc] = 1000000.0 / (instrumentHeaders[i].sensitivity[j] * instrumentHeaders[i].amplifierGain[j] * instrumentHeaders[i].excitationVoltage[j] * instrumentHeaders[i].acquisitionCardRange[j]);
      inc += instrumentHeaders[i].numberOfChannels;
    }
    Trial* trial = new Trial(strip_path(this->device()->name()),output);
    unsigned samples = static_cast<unsigned>(totalTimeTrial * static_cast<double>(rate));
    auto tss = make_nodes<TimeSequence*>(totalNumberOfChannels, 1, samples, static_cast<double>(rate), 0.0, TimeSequence::Analog, "V", trial->timeSequences());
    std::string forceUnit, momentUnit;
//     if (units == 0) // english
//     {
//       forceUnit = "in";
//       momentUnit = "lb";
//     }
//     else
//     {
      // The FP length/width seems to be set everytime in inch even if the header is set to metric units (so only used for measured value?).
      for (size_t i = 0 ; i < instrumentHeaders.size() ; ++i)
      {
         // Convert from inch to meter
        instrumentHeaders[i].length *= 0.0254f;
        instrumentHeaders[i].width *= 0.0254f;
        instrumentHeaders[i].offset[0] *= 0.0254f;
        instrumentHeaders[i].offset[1] *= 0.0254f;
        instrumentHeaders[i].offset[2] *= 0.0254f;
      }
      // Same for the content of the measure. The BSF file format seems to save the data in pound and pound-inch
      for (int i = 0 ; i < numberOfActivePlatforms ; ++i)
      {
        scale[i*6] *= 4.4482216152605;
        scale[i*6+1] *= 4.4482216152605;
        scale[i*6+2] *= 4.4482216152605;
        scale[i*6+3] *= 0.1129848290276167;
        scale[i*6+4] *= 0.1129848290276167;
        scale[i*6+5] *= 0.1129848290276167;
      }
      forceUnit = "N";
      momentUnit = "Nm";
//     }
    auto it = tss.begin();
    std::string suffix = ((numberOfActivePlatforms == 1) ? "" : "1");
    int suffixInc = 1;
    float globalOrigin[3] = {0.0f, 0.0f, 0.0f};
    int numToAdaptChannelIndex = 0;
    for (int i = 0 ; i < numberOfActivePlatforms ; ++i)
    {
      std::vector<double> corners(12, 0.0);
      std::vector<double> origin(3, 0.0);
      std::vector<int16_t> channelIndices(6);
      double sr = instrumentHeaders[i].rate;
      if (sr <= 0)
        sr = rate;
      (*it)->setName("Fx" + suffix);
      (*it)->setUnit(forceUnit);
      (*it)->setScale(scale[i*6]);
      (*it)->setSampleRate(sr);
      ++it;
      (*it)->setName("Fy" + suffix);
      (*it)->setUnit(forceUnit);
      (*it)->setScale(scale[i*6+1]);
      (*it)->setSampleRate(sr);
      ++it;
      (*it)->setName("Fz" + suffix);
      (*it)->setUnit(forceUnit);
      (*it)->setScale(scale[i*6+2]);
      (*it)->setSampleRate(sr);
      ++it;
      (*it)->setName("Mx" + suffix);
      (*it)->setUnit(momentUnit);
      (*it)->setScale(scale[i*6+3]);
      (*it)->setSampleRate(sr);
      ++it;
      (*it)->setName("My" + suffix);
      (*it)->setUnit(momentUnit);
      (*it)->setScale(scale[i*6+4]);
      (*it)->setSampleRate(sr);
      ++it;
      (*it)->setName("Mz" + suffix);
      (*it)->setUnit(momentUnit);
      (*it)->setScale(scale[i*6+5]);
      (*it)->setSampleRate(sr);
      ++it;
      if (i > 0)
      {
        if ((instrumentHeaders[i].interDistance[0] == 0.0f) && (instrumentHeaders[i].interDistance[1] == 0.0f) && (instrumentHeaders[i].interDistance[2] == 0.0f))
        {
          warning("The distance with the previous force platform is set to 0. The platform is automatically shifted in the front of the previous.");
          instrumentHeaders[i].interDistance[1] = static_cast<float>(instrumentHeaders[i].length + instrumentHeaders[i-1].length) / 2.0f;
        }
      }
      globalOrigin[0] += instrumentHeaders[i].interDistance[0];
      globalOrigin[1] += instrumentHeaders[i].interDistance[1];
      globalOrigin[2] += instrumentHeaders[i].interDistance[2];
      _bsf_extract_fp_configuration(&(instrumentHeaders[i]), globalOrigin, channelIndices.data(), corners.data(), origin.data());
      for (unsigned j = 0 ; j < 6 ; ++j)
        channelIndices[j] += numToAdaptChannelIndex;
      numToAdaptChannelIndex += instrumentHeaders[i].numberOfChannels;
      auto fp = new instrument::ForcePlateType2(instrumentHeaders[i].name, trial->hardwares());
      for (unsigned j = 0 ; j < fp->channelsNumberRequired() ; ++j)
      {
        assert((channelIndices[j] >= 0) && (static_cast<unsigned>(channelIndices[j]) < tss.size()));
        fp->setChannel(j, tss[channelIndices[j]]);
      }
      fp->setGeometry(std::array<double,3>{{origin[0],origin[1],origin[2]}},
                      std::array<double,3>{{corners[0],corners[1],corners[2]}},
                      std::array<double,3>{{corners[3],corners[4],corners[5]}},
                      std::array<double,3>{{corners[6],corners[7],corners[8]}},
                      std::array<double,3>{{corners[9],corners[10],corners[11]}});
      suffix = std::to_string(++suffixInc);
    }
    // Data
    // Note: We want the reaction of the measure, so all the data are multiplied by -1.
    for (unsigned i = 0 ; i < samples ; ++i)
    {
      int inc = 0;
      for (auto ts : tss)
        ts->data()[i] = -1.0 * static_cast<double>(stream.readI16()) * scale[inc++];
    }
  };
};
};