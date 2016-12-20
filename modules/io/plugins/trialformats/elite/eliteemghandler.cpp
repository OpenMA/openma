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

#include "eliteemghandler.h"
#include "openma/io/binarystream.h"
#include "openma/io/device.h"
#include "openma/io/enums.h"
#include "openma/io/utils.h"
#include "openma/base/trial.h"
#include "openma/base/timesequence.h"

#include <algorithm> // std::transform

namespace ma
{
namespace io
{
  EliteEmgHandler::EliteEmgHandler()
  : Handler()
  {};
  
  EliteEmgHandler::~EliteEmgHandler() _OPENMA_NOEXCEPT = default;
  
  Signature EliteEmgHandler::verifySignature(const Device* const device) _OPENMA_NOEXCEPT
  {
    // NOTE There is no known signature for the Elite EMG/EMR/EMF format
    //      Instead the verification is realized on the name of the device
    //      Moreover, the 2 first bytes seems to be equal to 0x00 everytime.
    //      They will be also verified.
    std::string lowercase = device->name();
    std::transform(lowercase.begin(), lowercase.end(), lowercase.begin(), tolower);
    std::string::size_type pos = std::string::npos;
    if ((((pos = lowercase.rfind(".emg")) == std::string::npos) && ((pos = lowercase.rfind(".emf")) == std::string::npos) && ((pos = lowercase.rfind(".emr")) == std::string::npos)) || (pos != (lowercase.length() - 4))) 
      return Signature::Invalid;
    char signature[2] = {0};
    if ((device->peek(signature,sizeof(signature)) != 2) || (signature[0] != 0x00) || (signature[1] != 0x00))
      return Signature::Invalid;
    return Signature::Valid;
  };

  Signature EliteEmgHandler::verifySignature() const _OPENMA_NOEXCEPT
  {
    return EliteEmgHandler::verifySignature(this->device());
  };
  
  void EliteEmgHandler::readDevice(ma::Node* output)
  {
    BinaryStream stream(this->device(), ByteOrder::IEEELittleEndian);
    this->device()->setExceptions(State::End | State::Fail | State::Error);
    // Bytes 1-2: 0x0000
    this->device()->seek(4, Origin::Begin);
    int numSamples = stream.readU16() + stream.readU16() * 1000;
    int numAnalogs = stream.readU16();
    double sampleRate = static_cast<double>(stream.readU16());
    Trial* trial = new Trial(strip_path(this->device()->name()),output);
    auto analogs = make_nodes<TimeSequence*>(numAnalogs,1,numSamples,sampleRate,0.0,TimeSequence::Analog,"V",trial->timeSequences());
    // Labels (max: 32 labels)
    for (auto& analog : analogs)
    {
      std::string label = stream.readString(8);
      trim_string(&label);
      crop_string(&label);
      analog->setName(label);
    }
    this->device()->seek((32 - numAnalogs) * 8, Origin::Current);
    // Scales (init)
    std::vector<double> scales = std::vector<double>(numAnalogs, 1.0);
    int i = 0;
    // Units
    for (auto& analog : analogs)
    {
      std::string unit = stream.readString(4);
      trim_string(&unit);
      crop_string(&unit);
      analog->setUnit(unit);
      if (unit.compare("mV") == 0)
        scales[i] = 1000.0;
    }
    // General ADC gain?
    this->device()->seek((32 - numAnalogs) * 4, Origin::Current);
    double adcGain = static_cast<double>(stream.readU16());
    // Others unknown data ...
    this->device()->seek(114, Origin::Current);
    // Analog channel settings
    // First parameter ... What is it?
    this->device()->seek(32*2, Origin::Current);
    // Second parameter ... What is it?
    this->device()->seek(32*2, Origin::Current);
    // Third parameter ... What is it?
    this->device()->seek(32*2, Origin::Current);
    // Gain
    for (int i = 0 ; i < numAnalogs ; ++i)
      scales[i] *= 20.0 / (4096.0 * adcGain * static_cast<double>(stream.readU16()));
    // Scaled values
    this->device()->seek(1024, Origin::Begin);
    for (int i = 0 ; i < numSamples ; ++i)
    {
      int inc = 0;
      for (auto& analog : analogs)
        analog->data()[i] = stream.readI16() * scales[inc++];
    }
  };
};
};
