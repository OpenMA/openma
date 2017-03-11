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

#include "hpfhandler.h"

#include "openma/io/handler_p.h"
#include "openma/io/buffer.h"
#include "openma/io/device.h"
#include "openma/io/binarystream.h"
#include "openma/io/enums.h"
#include "openma/io/utils.h"
#include "openma/base/any.h"
#include "openma/base/node.h"
#include "openma/base/trial.h"
#include "openma/base/timesequence.h"
#include "openma/base/logger.h"

#include <string>
#include <list>
#include <vector>
#include <cmath>


#include <pugixml/pugixml.h>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace io
{
  struct HPFChunkInfo
  {
    size_t position;
    int64_t id;
    int64_t size;
    int32_t group;
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
  * The HPF file format is used in the software EMGWorks 4.0 (Delsys Inc., Natick, USA) and contains only analogs data.
  *
  * @note The reader was implemented based on the document <em>DT High Performance File Format Specification</em> (Document Number: 22760, Rev A) proposed by the company Data Translation, Inc. (Marlborough, USA).
  * No restriction was found which could mention the prohibition of the use this documentation to implement a file reader.
  *
  */

  HPFHandler::HPFHandler()
  : Handler()
  {};
  
  HPFHandler::~HPFHandler() _OPENMA_NOEXCEPT = default;
  
  Signature HPFHandler::verifySignature(const Device* const device) _OPENMA_NOEXCEPT
  {
    char tmp[20] = {0};
    unsigned tmpsize = sizeof(tmp);
    if ((device->peek(tmp,tmpsize) != tmpsize))
      return Signature::Invalid;
    Buffer buffer;
    buffer.open(tmp,tmpsize);
    BinaryStream stream(&buffer, ByteOrder::IEEELittleEndian);
    auto chunkID = stream.readI64();
    buffer.seek(8, Origin::Current); // chunkSize
    if ((chunkID != 0x1000) || (stream.readString(4).compare("datx") != 0))
      return Signature::Invalid;
    return Signature::Valid;
  };
  
  Signature HPFHandler::verifySignature() const _OPENMA_NOEXCEPT
  {
    return verifySignature(this->device());
  };
  
  void HPFHandler::readDevice(Node* output)
  {
    BinaryStream stream(this->device(), ByteOrder::IEEELittleEndian);
    this->device()->setExceptions(State::End | State::Fail | State::Error);
    // Header
    auto chunkID = stream.readI64();
    auto chunkSize = stream.readI64();
    if ((chunkID != 0x1000) || (stream.readString(4).compare("datx") != 0))
      throw(FormatError("Invalid HPF file."));
    auto fileVersion = stream.readI64();
    if (fileVersion != 0x10001)
      throw(FormatError("Unsupported version of the HPF file format."));
    this->device()->seek(chunkSize, Origin::Begin);
    
    // Extract chunks information. For this, we look until the end of the file
    // to find the type of chunks and their size.
    // Only two types of chunk are extracted and the others are discarded:
    //  - Channel information chunk: 0x2000
    //  - Data chunk: 0x3000
    const int64_t channelInfoChunkID = 0x2000;
    const int64_t dataChunkID = 0x3000;
    std::list<HPFChunkInfo> chunks;
    // EOF bit can be triggered by the Read* methods while FAIL can be triggered by the SeekRead method.
    this->device()->setExceptions(State::Error);
    do
    {
      size_t chunkPosition = static_cast<size_t>(this->device()->tell());
      chunkID = stream.readI64();
      chunkSize = stream.readI64();
      int64_t already_read = 16;
      if ((chunkID == channelInfoChunkID) || (chunkID == dataChunkID))
      {
        HPFChunkInfo ci;
        ci.position = chunkPosition;
        ci.id = chunkID;
        ci.size = chunkSize;
        ci.group = stream.readI32();
        chunks.push_back(ci);
        already_read += 4;
      }
      this->device()->seek(chunkSize - already_read, Origin::Current);
    } while (!this->device()->atEnd());
    // Check if only one Channel information chunk exists. Data with different time
    // increments per channel is not supported in the library Biomechanical ToolKit. 
    HPFChunkInfo* channelInfoChunk = 0;
    for (auto it = chunks.begin() ; it != chunks.end() ; ++it)
    {
      if (it->id == channelInfoChunkID)
      {
        if (channelInfoChunk == 0)
          channelInfoChunk = &(*it);
        else
          throw(FormatError("HPF file with more than one channel information chunk is not supported."));
      }
    }
    if (channelInfoChunk == 0)
      throw(FormatError("No channel information chunk found. Impossible to configure the acqusition and the analog channels."));
    // Set the IO exceptions as initially
    this->device()->clear();
    this->device()->setExceptions(State::End | State::Fail | State::Error);
    // Go to the chunk after the group ID
    this->device()->seek(channelInfoChunk->position + 20, Origin::Begin);
    // Extract Channel Info
    int32_t numAnalogChannels = stream.readI32();
    int32_t numAnalogChannelsParsed = 0;
    // The remaining part of the chunk contains XML data
    // The reading of the XML data is done with pugixml
    // Depending of the configuration of OpenMA and pugixml, we try to avoid any memory allocation for the XML data
    const size_t channelInfoChunkXmlDataOffset = 24;
    size_t xmlBufferSize = channelInfoChunk->size - channelInfoChunkXmlDataOffset;
    char* xmlBuffer = new char[xmlBufferSize];
    stream.readChar(xmlBufferSize, xmlBuffer);
    std::string errmsg;
    Trial* trial = new Trial(strip_path(this->device()->name()),output);
    auto tss = std::vector<TimeSequence*>(numAnalogChannels, nullptr);
    pugi::xml_document xmlDoc;
    pugi::xml_parse_result xmlResult = xmlDoc.load_buffer_inplace(xmlBuffer, xmlBufferSize, pugi::parse_minimal, pugi::encoding_utf8);
    if (xmlResult)
    {
      pugi::xml_node channels = xmlDoc.child("ChannelInformationData");
      for (pugi::xml_node channel = channels.child("ChannelInformation"); channel; channel = channel.next_sibling("ChannelInformation"))
      {
        std::string label = channel.child_value("Name");
        std::string unit = channel.child_value("Unit");
        // In case the unit is set to the string 'Volts', it is replaced by 'V' as most of the file format use only the letter 'V'.
        if (unit.compare("Volts") == 0)
          unit = "V";
        double requestedPerChannelSampleRate = std::strtod(channel.child_value("RequestedPerChannelSampleRate"), nullptr);
        // In some case (conversion from Delsys EMG file?), the RequestedPerChannelSampleRate is null but not the PerChannelSampleRate.
        if (requestedPerChannelSampleRate == 0.0)
          requestedPerChannelSampleRate = std::strtod(channel.child_value("PerChannelSampleRate"), nullptr);
        // Do not know if Delsys used another storage format than Float
        if (strcmp(channel.child_value("DataType"), "Float") != 0)
        {
          errmsg = "The HPF file reader supports only data stored as float.";
          break;
        }
        // To simplify the implementation of the reader, the data index is assumed sorted.
        if (std::atoi(channel.child_value("DataIndex")) != numAnalogChannelsParsed)
        {
          errmsg = "The HPF file reader supports only data with a sorted index.";
          break;
        }
        // Try to set the gain
        double rangeMin = std::strtod(channel.child_value("RangeMin"),nullptr);
        double rangeMax = std::strtod(channel.child_value("RangeMax"),nullptr);
        if (std::fabs(rangeMin + rangeMax) < std::numeric_limits<float>::epsilon())
        {
          // In some case (conversion from Delsys EMG file?), the Range(Min|Max) values are null. The external gain is then extracted to try to set the analog gain.
          if (rangeMax == 0.0)
          {
            warning("Unknown range for the channel '%s'. Trying from the external gain...", label.c_str());
            rangeMax = std::strtod(channel.child_value("ExternalGain"),nullptr);
            rangeMin = -rangeMax;
          }

        }
        // Append the configured analog channel into the acquisition
        tss[numAnalogChannelsParsed++] = new TimeSequence(label, 1, 0, requestedPerChannelSampleRate, 0.0, TimeSequence::Analog, unit, 1.0, 0.0, {{rangeMin, rangeMax}}, trial->timeSequences());
      }
    }
    else
      errmsg = "Error during the parsing of the XML data: " + std::string(xmlResult.description());
    if ((numAnalogChannelsParsed != numAnalogChannels) && errmsg.empty())
      errmsg = "The number of analog channels parsed (" + std::to_string(numAnalogChannelsParsed) + ") is different than the number of channels stored in the configuration (" + std::to_string(numAnalogChannels) + ")";
    delete[] xmlBuffer;
    if (!errmsg.empty())
      throw(FormatError(errmsg));
    // Extract the data
    for (auto it = chunks.cbegin() ; it != chunks.cend() ; ++it)
    {
      if (it->id != dataChunkID)
        continue;
      // Go to the chunk after the chunk size
      this->device()->seek(it->position + 16, Origin::Begin);
      // Extract and verify the configuration of the chunk
      // - Just in case the group ID is not the same
      if (stream.readI32() != channelInfoChunk->group)
        continue;
      int64_t dataStartIndex = stream.readI64();
      int64_t channelDataCount = stream.readI32();
      // - If for some reason the the number of channels is greater than in the configuration, the value is bounded.
      if (channelDataCount > numAnalogChannels)
        channelDataCount = numAnalogChannels;
      std::vector<uint32_t> channelDescriptor(2*channelDataCount);
      stream.readU32(2*channelDataCount, channelDescriptor.data());
      // Read data
      auto itA = tss.begin();
      for (int i = 0 ; i < channelDataCount ; ++i)
      {
        this->device()->seek(it->position + channelDescriptor[i*2], Origin::Begin);
        unsigned samples = channelDescriptor[i*2+1] / 4;
        (*itA)->resize(dataStartIndex+samples);
        auto data = (*itA)->data();
        for (size_t j = 0 ; j < samples ; ++j)
          data[dataStartIndex+j] = stream.readFloat();
        ++itA;
      }
    }
  };
};
};