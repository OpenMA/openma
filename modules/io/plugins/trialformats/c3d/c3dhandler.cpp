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

#include "c3dhandler.h"
#include "c3ddatastream.h"

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

#include <string>
#include <list>
#include <vector>
#include <tuple>
#include <array>
#include <memory> // std::unique_ptr

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace io
{
  class C3DHandlerPrivate : public HandlerPrivate
  {
  public:
    C3DHandlerPrivate();
    ~C3DHandlerPrivate() _OPENMA_NOEXCEPT;

    C3DHandlerPrivate(const C3DHandlerPrivate& ) = delete;
    C3DHandlerPrivate(C3DHandlerPrivate&& ) _OPENMA_NOEXCEPT = delete;
    C3DHandlerPrivate& operator=(const C3DHandlerPrivate& ) = delete;
    C3DHandlerPrivate& operator=(const C3DHandlerPrivate&& ) _OPENMA_NOEXCEPT = delete;
    
    double PointScale;
    int PointMaximumInterpolationGap;
    int AnalogResolution;
    std::vector<double> AnalogChannelScale;
    std::vector<double> AnalogZeroOffset;
    double AnalogUniversalScale;
    bool AnalogSignedIntegerFormat;
    
    template <typename T>
    static inline typename std::enable_if<!std::is_same<T,std::string>::value,T>::type generateDefaultValue(T&& blank, int idx)
    {
      OPENMA_UNUSED(idx);
      return blank;
    };
  
    template <typename T>
    static inline typename std::enable_if<std::is_same<T,std::string>::value,T>::type generateDefaultValue(T&& blank, int idx)
    {
      return (blank.empty() ? blank : blank + std::to_string(idx));
    };
    
    template <typename T>
    static void mergeProperties(std::vector<T>* target, Trial* trial, const std::string& base, int finalSize = -1, T&& defaultValue = T());
  };
  
  C3DHandlerPrivate::C3DHandlerPrivate()
  : HandlerPrivate(),
    PointScale(1.0), PointMaximumInterpolationGap(0),
    AnalogResolution(12), AnalogChannelScale(), AnalogZeroOffset(), AnalogUniversalScale(1.0), AnalogSignedIntegerFormat(true)
  {};
  
  C3DHandlerPrivate::~C3DHandlerPrivate() _OPENMA_NOEXCEPT = default;
  
  template <typename T>
  void C3DHandlerPrivate::mergeProperties(std::vector<T>* target, Trial* trial, const std::string& base, int finalSize, T&& defaultValue)
  {
    target->clear();
    int numCollapsed = 0;
    int inc = 2;
    std::string label = base;
    while (1)
    {
      Any prop = trial->property(label);
      if (!prop.isValid())
        break;
      std::vector<T> values = prop.cast<std::vector<T>>();
      size_t num = target->size() + values.size();
      if (!values.empty())
      {
        auto itVal = values.begin();
        if ((static_cast<int>(num) >= finalSize) && (finalSize != -1))
          std::advance(itVal, finalSize - target->size());
        else
          itVal = values.end();
        target->insert(target->end(), values.begin(), itVal);
        numCollapsed = target->size();
      }
      if (numCollapsed == finalSize)
        break;
      label = base + std::to_string(inc);
      ++inc;
    }
    if (numCollapsed < finalSize)
    {
      target->resize(finalSize);
      for (int inc = numCollapsed ; inc < finalSize ; ++inc)
        target->operator[](inc) = generateDefaultValue(std::forward<T>(defaultValue), inc+1);
    }
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
   * Note: Events containing the property "c3d_fromheader" will be stored in the header of the format. However a maximum of 18 events
   */

  C3DHandler::C3DHandler()
  : Handler(*new C3DHandlerPrivate)
  {};
  
  C3DHandler::~C3DHandler() _OPENMA_NOEXCEPT = default;
  
  Signature C3DHandler::verifySignature(const Device* const device) _OPENMA_NOEXCEPT
  {
    char signature[2] = {0};
    device->peek(signature,sizeof(signature));
    if ((signature[0] <= 0) || (signature[1] != 0x50))
      return Signature::Invalid;
    return Signature::Valid;
  };

  
  Signature C3DHandler::verifySignature() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return verifySignature(optr->Source);
  };
  
  void C3DHandler::readDevice(Node* output)
  {
    auto optr = this->pimpl();
    BinaryStream stream(optr->Source);
    
    optr->Source->setExceptions(State::End | State::Fail | State::Error);
    int8_t parameterFirstBlock = stream.readI8();
    if (parameterFirstBlock <= 0)
      throw(FormatError("Invalid first block number for the parameter section"));
    if (stream.readI8() != 80)
      throw(FormatError("Wrong header key"));
    optr->Source->seek(512 * (parameterFirstBlock - 1) + 3, Origin::Begin);
    switch (stream.readI8())
    {
    // Intel processor type
    case 1:
      warning("ORG.C3D - %s - Wrong processor type. Trying to continue by using the INTEL processor", optr->Source->name());
    case 84:
      stream.setByteOrder(ByteOrder::IEEELittleEndian);
      break;
    // DEC (VAX, PDP-11) processor type
    case 2:
      warning("ORG.C3D - %s - Wrong processor type. Trying to continue by using the DEC processor", optr->Source->name());
    case 85:
      stream.setByteOrder(ByteOrder::VAXLittleEndian);
      break;
    // MIPS processor (SGI/MIPS) processor type
    case 3:
      warning("ORG.C3D - %s - Wrong processor type. Trying to continue by using the MIPS processor", optr->Source->name());
    case 86:
      stream.setByteOrder(ByteOrder::IEEEBigEndian);
      break;
    default :
      throw(FormatError("Invalid processor type"));
      break;
    };
    optr->Source->seek(2, Origin::Begin);
    Trial* trial = new Trial(strip_path(optr->Source->name()),output);
    uint16_t pointNumber = 0,
             totalAnalogSamplesPer3dFrame = 0,
             dataFirstBlock = 0, 
             numberSamplesPerAnalogChannel = 0;
    size_t firstSampleIndex = 0, lastSampleIndex = 0;
    float pointScaleFactor = 0.0, pointSampleRate = 0.0;
    bool hasHeader = false;
    if (parameterFirstBlock != 1)
    {
      hasHeader = true;
  // Header
      pointNumber = stream.readU16(); // (word 02)
      totalAnalogSamplesPer3dFrame = stream.readU16(); // (word 03)
      firstSampleIndex = stream.readU16(); // (word 04)
      lastSampleIndex = stream.readU16(); // (word 05)
      optr->PointMaximumInterpolationGap = stream.readU16(); // (word 06)
      trial->setProperty("c3d_pointmaxinterpgap", optr->PointMaximumInterpolationGap);
      pointScaleFactor = stream.readFloat(); // (word 07-08)
      if (pointScaleFactor == 0)
        throw(FormatError("Incorrect 3D scale factor"));
      optr->PointScale = pointScaleFactor;
      dataFirstBlock = stream.readU16(); // (word 09)
      numberSamplesPerAnalogChannel = stream.readU16(); // (word 10)
      pointSampleRate = stream.readFloat(); // (word 11-12)
      if (pointSampleRate <= 0.0)
        throw(FormatError("Incorrect sample rate. This value cannot be negative or null"));
      optr->Source->seek(270, Origin::Current); // word 13-147 => 135 words => 270 bytes 
  // Event in Header section
      const int maxEvents = 18;
      const uint16_t headerKeyValue = 12345;
      uint16_t labelRangeSection = stream.readU16(); // (word 148)
      uint16_t labelRangeFirstBlock = stream.readU16(); // (word 149)
      if (labelRangeSection == headerKeyValue)
      {
        warning("ORG.C3D - %s - A 'Label and Range Section' was detected but OpenMA doesn't support it", optr->Source->name());
        if (labelRangeFirstBlock < 2)
          warning("ORG.C3D - %s - The 'Label and Range Section' address is incorrect", optr->Source->name());
      }
      uint16_t labelEventFormat = stream.readU16(); // (word 150)
      uint16_t eventNumber = stream.readU16();
      if (eventNumber != 0)
      {
        std::vector<Event*> events(eventNumber,nullptr);
        optr->Source->seek(2, Origin::Current); // (word 152)
        for (size_t i = 0 ; i < eventNumber ; ++i)
          events[i] = new Event("",stream.readFloat(),{},{},trial->events());
        optr->Source->seek((maxEvents * 4) - (eventNumber * 4), Origin::Current);
        // No need to read event's activation status.
        optr->Source->seek(maxEvents, Origin::Current);
        optr->Source->seek(2, Origin::Current); // (word 198)
        const int numCharLabelEvent = (labelEventFormat == headerKeyValue) ? 4 : 2;
        for (size_t i = 0 ; i < eventNumber ; ++i)
        {
          events[i]->setName(trim_string(stream.readString(numCharLabelEvent)));
          // Property to know that these events are stored in the header of the C3D format. 
          events[i]->setProperty("c3d_fromheader",true);
        }
      }
  // Parameter
      optr->Source->seek((512 * (parameterFirstBlock - 1)), Origin::Begin);
      // From the C3D documentation:
      // "The first two bytes of the parameter record are only meaningful if they also form the first word of the file"
      // [...]
      // "This is because one common technique for creating C3D files used to be to maintain a parameter 'template' as a separate file"
      optr->Source->seek(2, Origin::Current);
      uint8_t blockNumber = stream.readU8();
      optr->Source->seek(1, Origin::Current); // Processor type
      size_t totalBytesRead = 4; // the four bytes read previously.
      using group_t = std::tuple<int,std::string>;  // id, label
      std::list<group_t> groups;
      using parameter_t = std::tuple<int,std::string,Any>;  // id, label, value(s)
      std::list<parameter_t> parameters;
      // std::list<MetaData::Pointer> parameters;
      // MetaData::Pointer root = output->GetMetaData();
      bool alreadyDisplayParameterOverflowMessage = false;
      // Extract the group and parameters
      while (1)
      { 
        int8_t numCharLabel = stream.readI8(); totalBytesRead += abs(numCharLabel) + 1;
        if (numCharLabel == 0)
          break; // Parameter section end
        int8_t id = stream.readI8(); totalBytesRead += 1;
        if (id == 0)
          throw(FormatError("Error during the ID extraction in the parameter section - ID equal to 0"));
        bool lastEntry = false; // Used to determine the end of the Parameter section
        std::string label = stream.readString(abs(numCharLabel));
        int offset = stream.readU16(); totalBytesRead += offset;
        if (offset == 0)
          lastEntry = true;
        offset -= 2;
        if (id < 0)
        {
          groups.emplace_back(id,label);
          uint8_t numCharDesc = stream.readU8(); offset -= 1;
          // The group's description is not stored and then sought.
          optr->Source->seek(numCharDesc, Origin::Current); offset -= numCharDesc;
        }
        else
        {
          int8_t type = stream.readI8(); offset -= 1;
          int8_t numDims = stream.readI8(); offset -= 1;
          std::vector<uint8_t> dims(numDims);
          stream.readU8(numDims,dims.data()); offset -= numDims;
          int prod = 1;
          int8_t inc = 0 ; while (inc < numDims) prod *= dims[inc++];
          int dataSize = static_cast<int>(prod * abs(type));
          bool dataSizeExceeded = (dataSize > offset) && (!lastEntry);
          if (dataSizeExceeded)
            warning("ORG.C3D - %s - The size of the data for the parameter '%s' exceeds the space available before the next entry. Trying to continue...", optr->Source->name(), label.c_str());
          Any value;
          switch (type)
          {
          case -1: // Char (transformed into strings)
            {
            int rows = 1;
            if (dims.size() >= 2)
            {
              int8_t inc2 = 1 ; while (inc2 < numDims) rows *= dims[inc2++];
              prod = dims[0]; // reused
              std::vector<uint8_t>(dims.begin()+1, dims.end()).swap(dims); // Remove the first element
            }
            else
              dims = std::vector<uint8_t>{1};
            std::vector<std::string> p(dataSizeExceeded ? 0 : rows);
            stream.readString(prod, p.size(), p.data());
            value = Any(p, dims);
            break;
            }
          case 1: // Byte
            {
            std::vector<int8_t> p(dataSizeExceeded ? 0 : prod);
            stream.readI8(p.size(), p.data());
            value = Any(p, dims);
            break;
            }
          case 2: // Integer
            {
            std::vector<int16_t> p(dataSizeExceeded ? 0 : prod);
            stream.readI16(p.size(), p.data());
            value = Any(p, dims);
            break;
            }
          case 4: // Real
            {
            std::vector<float> p(dataSizeExceeded ? 0 : prod);
            stream.readFloat(p.size(), p.data());
            value = Any(p, dims);
            break;
            }
          default :
            throw(FormatError("Data parameter type unknown for the entry: '" + label + "'"));
            break;
          }
          parameters.emplace_back(id,label,value);
          offset -= dataSize;
          if (offset != 0)
          {
            uint8_t numCharDesc = stream.readU8(); offset -= 1;
            // The parameter's description is not stored and then sought.
            optr->Source->seek(numCharDesc, Origin::Current);
            offset -= numCharDesc;
          }
          else
            warning("ORG.C3D - %s - Where is the byte to set the number of characters in the description of the parameter '%s'? Trying to continue...", optr->Source->name(), label.c_str());
        }
        if (lastEntry)
          offset = 0;
        if (offset < 0)
        {
          warning("ORG.C3D - %s - Error during the pointing of the next parameter. Trying to continue...", optr->Source->name());
          optr->Source->seek(offset, Origin::Current);
          offset = 0;
        }
        // Checks if the next parameter is not pointing in the Data section.
        if ((totalBytesRead + offset) > static_cast<unsigned int>((blockNumber * 512)))
        {
          if ((totalBytesRead + offset) > static_cast<unsigned int>(((dataFirstBlock - 2) * 512)))
          {
            warning("ORG.C3D - %s - The next parameter is pointing in the Data section. Parameters' extraction is stopped", optr->Source->name());
            totalBytesRead = blockNumber * 512; // Force the number of totalBytesRead to not trigger the "Bad data first block" exception.
            break;
          }
          else if (!alreadyDisplayParameterOverflowMessage)
          {
            warning("ORG.C3D - %s - The next parameter is pointing outside of the parameter section but not yet in the Data section. Trying to continue...", optr->Source->name());
            alreadyDisplayParameterOverflowMessage = true;
          }
        }
        if (lastEntry)
          break; // Parameter section end
        optr->Source->seek(offset, Origin::Current);
      }
      // Assemble groups and parameters
      auto itG = groups.begin();
      auto itP = parameters.begin();
      if (!parameters.empty())
      {
        while(1)
        {
          if (itP == parameters.end())
          {
            ++itG;
            itP = parameters.begin();
            if ((itG == groups.end()) || (itP == parameters.end()))
              break;
          }
          if (std::get<0>(*itG) == -std::get<0>(*itP))
          {
            trial->setProperty(std::get<1>(*itG)+":"+std::get<1>(*itP),std::move(std::get<2>(*itP)));
            itP = parameters.erase(itP); 
          }
          else
            ++itP;
        }
      }
      if (parameters.size() != 0)
        warning("ORG.C3D - %s - Some parameters are orphans. No group has the same ID. These parameters are lost", optr->Source->name());
      int totalBlocksRead = static_cast<int>(ceil((double)totalBytesRead / 512.0));
      if (totalBlocksRead != blockNumber)
      {
        warning("ORG.C3D - %s - The number of blocks to read in the parameter section is different than the number of blocks read. The value kept is the number of blocks read", optr->Source->name());
        blockNumber = totalBlocksRead;
      }
    // Events in Parameter section
      std::string eventPrefix = "EVENT:";
      Any eventUsed = trial->property(eventPrefix+"USED");
      if (!eventUsed.isValid())
      {
        // Take a chance to find the events in EVENTS group instead of EVENT
        // The BKINtechnologies Dexterit-E software use similar metadata than what proposed Vicon (See sample22 on C3D.org)
        std::string eventPrefix = "EVENTS:";
        eventUsed =  trial->property(eventPrefix+"USED");
        if (eventUsed.isValid())
          warning("ORG.C3D - %s - EVENTS group found instead of EVENT. The EVENTS group is used to extract events", optr->Source->name());
      }
      if (eventUsed.isValid())
      {
        int eventsNumber = eventUsed;
        std::vector<std::string> eventsLabel;
        std::vector<double> eventsTime;
        // std::vector<int> eventsId;
        std::vector<std::string> eventsContext;
        std::vector<std::string> eventsSubject;
        std::vector<std::string> eventsDescription;
        C3DHandlerPrivate::mergeProperties<std::string>(&eventsLabel, trial, eventPrefix+"LABELS", eventsNumber, "uname*");
        C3DHandlerPrivate::mergeProperties(&eventsTime, trial, eventPrefix+"TIMES");
        if (eventsTime.size() < static_cast<size_t>(2 * eventsNumber))
          warning("ORG.C3D - %s - The EVENT:TIMES doesn't contain the appropriate number of values. The extracted times could be corrupted", optr->Source->name());
        eventsTime.resize(2 * eventsNumber, 0.0);
        C3DHandlerPrivate::mergeProperties(&eventsContext, trial, eventPrefix+"CONTEXTS", eventsNumber);
        C3DHandlerPrivate::mergeProperties(&eventsSubject, trial, eventPrefix+"SUBJECTS", eventsNumber);
        C3DHandlerPrivate::mergeProperties(&eventsDescription, trial, eventPrefix+"DESCRIPTIONS", eventsNumber);
        // C3DHandlerPrivate::mergeProperties(eventsId, trial, eventPrefix+"ICON_IDS", eventsNumber);
        for (int incEvt = 0 ; incEvt < eventsNumber ; ++incEvt)
        {
          Event* event = new Event(trim_string(eventsLabel[incEvt]),
                                   eventsTime[2 * incEvt] * 60 + eventsTime[2 * incEvt + 1],
                                   trim_string(eventsContext[incEvt]),
                                   trim_string(eventsSubject[incEvt]),
                                   trial->events());
          event->setDescription(eventsDescription[incEvt]);
        }
      }
    // Configure the acquisition based on some metadata
      // - ANALOG:BITS
      Any analogBits = trial->property("ANALOG:BITS");
      if (analogBits.isValid())
        optr->AnalogResolution = analogBits;
      // - TRIAL:ACTUAL_START_FIELD
      Any trialActualStartField = trial->property("TRIAL:ACTUAL_START_FIELD");
      if (trialActualStartField.isValid())
      {
        // int lsb = trialActualStartField.cast<int>(0);
        // int hsb = trialActualStartField.cast<int>(1);
        // int start = hsb << 16 | lsb;
        size_t start = trialActualStartField.cast<size_t>(1) << 16 | trialActualStartField.cast<size_t>(0);
        if (start != firstSampleIndex)
        {
          if ((firstSampleIndex != 65535) && hasHeader)
            warning("ORG.C3D - %s - The first sample index wrote in the header is different than in the parameter TRIAL:ACTUAL_START_FIELD. The value in the parameter is kept", optr->Source->name());
          firstSampleIndex = start;
        }
      }
      // - TRIAL:ACTUAL_END_FIELD
      Any trialActualEndField = trial->property("TRIAL:ACTUAL_END_FIELD");
      if (trialActualEndField.isValid())
      {
        // int lsb = trialActualEndField.cast<int>(0);
        // int hsb = trialActualEndField.cast<int>(1);
        // int end = hsb << 16 | lsb;
        size_t end = trialActualEndField.cast<size_t>(1) << 16 | trialActualEndField.cast<size_t>(0);
        if (end != lastSampleIndex)
        {
          if ((lastSampleIndex != 65535) && hasHeader)
            warning("ORG.C3D - %s - The last sample index wrote in the header is different than in the parameter TRIAL:ACTUAL_END_FIELD. The number of samples is modified by keeping the value in the parameter", optr->Source->name());
          lastSampleIndex = end;
        }
      }
      // - POINT:LONG_FRAMES
      //   TODO: IMPLEMENT THE CASE WHERE C3D FILES COME FROM C-MOTION WITH MORE THAN 65535 samples.
      // - POINT:UNITS*
      std::vector<std::string> pointUnits{
        trial->property("POINT:UNITS"),
        trial->property("POINT:ANGLE_UNITS"),
        trial->property("POINT:FORCE_UNITS"),
        trial->property("POINT:MOMENT_UNITS"),
        trial->property("POINT:POWER_UNITS"),
        trial->property("POINT:SCALAR_UNITS")
      };
    // Data
      if (dataFirstBlock != 0)
      {
        if (dataFirstBlock < (parameterFirstBlock + blockNumber))
          throw(FormatError("Bad data first block"));
        optr->Source->seek((512 * (dataFirstBlock - 1)), Origin::Begin);
        if (numberSamplesPerAnalogChannel == 0)
          numberSamplesPerAnalogChannel = 1;
        uint16_t analogNumber = totalAnalogSamplesPer3dFrame / numberSamplesPerAnalogChannel;
        // ANALOG
        Any analogUsed = trial->property("ANALOG:USED");
        if (analogUsed.isValid())
        {
          // ANALOG:USED
          if (analogNumber != analogUsed)
            warning("ORG.C3D - %s - The number of analog channels wrote in the header section and in the parameter section are not the same. The value kept is from the header section", optr->Source->name());
          optr->AnalogChannelScale.resize(analogNumber, 1.0);
          optr->AnalogZeroOffset.resize(analogNumber, 0.0);
          optr->AnalogUniversalScale = 1.0;

          if (analogNumber != 0)
          {
            // Check if values in ANALOG:OFFSET correspond to the informations in ANALOG:FORMAT and ANALOG:BITS
            std::vector<int16_t> zeroOffset;
            C3DHandlerPrivate::mergeProperties(&zeroOffset, trial, "ANALOG:OFFSET",analogNumber,int16_t(0));
            int bits = optr->AnalogResolution;
            for (size_t inc = 0 ; inc < optr->AnalogZeroOffset.size() ; ++inc)
            {
              if (fabs(static_cast<double>(zeroOffset[inc])) > pow(2.0,bits))
              {
                bits += 2;
                --inc;
              }
            }
            if (bits != optr->AnalogResolution)
            {
              if (bits >= 16)
              {
                optr->AnalogResolution = 16;
                optr->AnalogSignedIntegerFormat = false;
              }
              else
                optr->AnalogResolution = 12;
              warning("ORG.C3D - %s - Analog format and/or their resolution are inconsistent with analog offsets. They were updated", optr->Source->name());
            }
            // - ANALOG:FORMAT
            if (trial->property("ANALOG:FORMAT").cast<std::string>().compare("UNSIGNED") == 0)
              optr->AnalogSignedIntegerFormat = false;
            else
              optr->AnalogSignedIntegerFormat = true;
            // - ANALOG:OFFSET
            if (optr->AnalogSignedIntegerFormat == false) // unsigned
            {
              for (size_t inc = 0 ; inc < optr->AnalogZeroOffset.size() ; ++inc)
                optr->AnalogZeroOffset[inc] = static_cast<double>(static_cast<uint16_t>(zeroOffset[inc]));
            }
            else // signed
              C3DHandlerPrivate::mergeProperties(&optr->AnalogZeroOffset, trial, "ANALOG:OFFSET", analogNumber, 0.0);
            // - ANALOG:SCALE
            C3DHandlerPrivate::mergeProperties(&optr->AnalogChannelScale, trial, "ANALOG:SCALE", analogNumber, 1.0);
            // - ANALOG:GEN_SCALE
            optr->AnalogUniversalScale = trial->property("ANALOG:GEN_SCALE");
            if (optr->AnalogUniversalScale == 0.0)
            {
              warning("ORG.C3D - %s - Analog universal scaling factor error. Value zero (0) replaced by one (1)", optr->Source->name());
              optr->AnalogUniversalScale = 1.0;
            }
          }
        }
        // POINT
        // POINT:USED
        if (trial->property("POINT:USED") != pointNumber)
          warning("ORG.C3D - %s - The number of points wrote in the header section and in the parameter section are not the same. The value kept is from the header section", optr->Source->name());
        // POINT:SCALE
        if (fabs(trial->property("POINT:SCALE").cast<double>() - optr->PointScale) > std::numeric_limits<float>::epsilon())
          warning("ORG.C3D - %s - The point scaling factor written in the header and in the parameter POINT:SCALE are not the same. The first value is kept", optr->Source->name());
        std::unique_ptr<C3DDataStream> dataStream; 
        if (optr->PointScale > 0) // integer
        {
          if (!optr->AnalogSignedIntegerFormat)
            dataStream.reset(new C3DDataStreamUnsignedInteger(&stream));
          else
            dataStream.reset(new C3DDataStreamSignedInteger(&stream));
        }
        else // float
        {
          dataStream.reset(new C3DDataStreamFloat(&stream));
        }
        size_t pointSamples = lastSampleIndex - firstSampleIndex + 1;
        double startTime = static_cast<double>(firstSampleIndex-1) / pointSampleRate;
        auto points = make_nodes<TimeSequence*>(pointNumber,4,pointSamples,pointSampleRate,startTime,TimeSequence::Marker,pointUnits[0],trial->timeSequences());
        auto analogs = make_nodes<TimeSequence*>(analogNumber,1,pointSamples*numberSamplesPerAnalogChannel,pointSampleRate*numberSamplesPerAnalogChannel,startTime,TimeSequence::Analog,"V",trial->timeSequences());
        try
        {
          for (size_t sample = 0 ; sample < pointSamples ; ++sample)
          {
            for (auto& pt: points)
            {
              dataStream->readPoint(
                &(pt->data()[sample]),
                &(pt->data()[sample + pointSamples]),
                &(pt->data()[sample + 2*pointSamples]),
                &(pt->data()[sample + 3*pointSamples]),
                optr->PointScale);
            }
            size_t analogSample = numberSamplesPerAnalogChannel * sample;
            for (int subsample = 0 ; subsample < numberSamplesPerAnalogChannel ; ++subsample)
            {
              size_t incChannel = 0;
              for (auto& an: analogs)
              {
                an->data()[analogSample+subsample] = (dataStream->readAnalog() - optr->AnalogZeroOffset[incChannel]) * optr->AnalogChannelScale[incChannel] * optr->AnalogUniversalScale;
                ++incChannel;
              }
            }
          }
        }
        catch (FormatError& )
        {
          // Let's try to continue even if the file is corrupted
          if (optr->Source->atEnd())
            warning("ORG.C3D - %s - Some points and/or analog data cannot be extracted and are set as invalid", optr->Source->name());
          else
            throw;
        }
        // Label, description, unit and type
        size_t inc = 0; 
        bool c3dFromMotion = (trial->property("MANUFACTURER:Company") == "Motion Analysis Corp");
        std::vector<std::string> labels, descriptions;
        // POINT Label, description, unit
        // NOTE: C3D files exported from "Motion Analysis Corp." softwares (EvaRT, Cortex) seem to use POINT:LABELS and POINTS:DESCRIPTIONS as a short and long version of the points' label respectively. Point's Label used in EvaRT and Cortex correspond to values stored in POINTS:DESCRIPTIONS. To distinguish C3D files exported from "Motion Analysis Corp." softwares, it is possible to check the value in the parameter MANUFACTURER:Company.
        // NOTE #2: Moreover, With (at least) Cortex 2.1.1 the occlusion of markers are not set by a mask and residuals equals to -1 but by coordinates set by 9999999 ...
        if (!c3dFromMotion)
        {
          // POINT:LABELS & POINT:DESCRIPTIONS
          C3DHandlerPrivate::mergeProperties<std::string>(&labels, trial, "POINT:LABELS", pointNumber, "uname*");
          C3DHandlerPrivate::mergeProperties<std::string>(&descriptions, trial, "POINT:DESCRIPTIONS", pointNumber);
        }
        else
        {
          // POINT:DESCRIPTIONS (which is in fact the exact label)
          C3DHandlerPrivate::mergeProperties<std::string>(&labels, trial, "POINT:DESCRIPTIONS", pointNumber, "uname*");
          descriptions.resize(pointNumber);
          // Set the timesqeunces' but also adapt coordinates and residuals for occluded markers
          inc = 0;
          for (auto& pt: points)
          {
            for (size_t sample = 0 ; sample < pointSamples ; ++sample)
            {
              // Check only the value on coordinate X to speed up this part
              if (fabs(pt->data()[sample] - 9999999.0) < std::numeric_limits<float>::epsilon())
              {
                pt->data()[sample] = 0.0;
                pt->data()[sample + pointSamples] = 0.0;
                pt->data()[sample + 2*pointSamples] = 0.0;
                pt->data()[sample + 3*pointSamples] = -1.0; // residual
              }
            }
          }
        }
        inc = 0;
        for (auto& pt: points)
        {
          pt->setName(trim_string(labels[inc]));
          pt->setDescription(trim_string(descriptions[inc]));
          pt->setScale(fabs(pointScaleFactor));
          ++inc;
        }
        // Point's type and unit
        const std::array<std::string,5> pointTypeNames{{"POINT:ANGLES","POINT:FORCES","POINT:MOMENTS","POINT:POWERS","POINT:SCALARS"}};
        const std::array<int,5> pointTypeTypes{{TimeSequence::Angle,TimeSequence::Force,TimeSequence::Moment,TimeSequence::Power,TimeSequence::Scalar}};
        for(size_t i = 0 ; i < pointTypeNames.size() ; ++i)
        {
          std::vector<std::string> labels;
          C3DHandlerPrivate::mergeProperties<std::string>(&labels, trial, pointTypeNames[i]);
          for (size_t j = 0 ; j < labels.size() ; ++j)
          {
            auto pt = trial->timeSequences()->findChild<TimeSequence*>(labels[j],{},false);
            if (pt != nullptr)
            {
              pt->setUnit(trim_string(pointUnits[i+1])); // +1: because the first element in pointUnits stores markers' unit.
              pt->setType(pointTypeTypes[i]);
            }
          }
        }
        // ANALOG Label, description, unit
        if (analogUsed.isValid())
        {
          std::vector<std::string> labels, descriptions, units;
          std::vector<int16_t> gains;
          std::vector<float> ranges;
          if (!c3dFromMotion)
          {
            // POINT:LABELS & POINT:DESCRIPTIONS
            C3DHandlerPrivate::mergeProperties<std::string>(&labels, trial, "ANALOG:LABELS", analogNumber, "uname*");
            C3DHandlerPrivate::mergeProperties<std::string>(&descriptions, trial, "ANALOG:DESCRIPTIONS", analogNumber);
          }
          else
          {
            // A for the points, Motion Analysis Corp. uses the parameter to store the exact analogs' label
            C3DHandlerPrivate::mergeProperties<std::string>(&labels, trial, "ANALOG:DESCRIPTIONS", analogNumber, "uname*");
          }
          C3DHandlerPrivate::mergeProperties(&units, trial, "ANALOG:UNITS", analogNumber);
          C3DHandlerPrivate::mergeProperties(&gains, trial, "ANALOG:GAIN", analogNumber);
          C3DHandlerPrivate::mergeProperties(&ranges, trial, "ANALOG:RANGE");
          inc = 0;
          for (auto& an: analogs)
          {
            an->setName(trim_string(labels[inc]));
            an->setDescription(trim_string(descriptions[inc]));
            an->setUnit(trim_string(units[inc]));
            an->setScale(optr->AnalogChannelScale[inc] * optr->AnalogUniversalScale);
            an->setOffset(optr->AnalogZeroOffset[inc]);
            switch(gains[inc])
            {
            case 1: // +/- 10V
              an->setRange(std::array<double,2>{{-10.0,10.0}});
              break;
            case 2: // +/- 5V
              an->setRange(std::array<double,2>{{-5.0,5.0}});
              break;
            case 3: // +/- 2.5V
              an->setRange(std::array<double,2>{{-2.5,2.5}});
              break;
            case 4: // +/- 1.25V
              an->setRange(std::array<double,2>{{-1.25,1.25}});
              break;
            default:
              if (gains[inc] != 0)
              {
                // Possible case 5 which was supported only by BTK (not in the official documentation)
                if (gains[inc] == 5) // +/- 1V
                  an->setRange(std::array<double,2>{{-1.,1.}});
                else
                  warning("ORG.C3D - %s - The extracted gain is not supported by the standard or OpenMA. However, the later gives you the possibility to use ANALOG:RANGE to set custom gain. Please read the documentation for more information", optr->Source->name());
              }
              else
              {
                // Maybe a (non)-symmetric range is given
                if (ranges.size() >= 2*(inc+1))
                  an->setRange(std::array<double,2>{{ranges[2*inc],ranges[2*inc+1]}});
              }
              break;
            }
            ++inc;
          }
        }
      }
    // Finally, try to generate hardware nodes from the trial's properties group/parameter
      // NodeFactory::generateHardwareFromProperties(trial,trial->hardware());
    }
  };
  
  /**
   * Use the native byte order format to write data
   */
  void C3DHandler::writeDevice(const Node* const input)
  {
    auto optr = this->pimpl();
    // Any valid data?
    auto trials = input->findChildren<const Trial*>({},{},false);
    if (trials.empty())
      throw(FormatError("ORG.C3D - No Trial object found in the children of the given node."));
    else if (trials.size() > 1)
      throw(FormatError("ORG.C3D - More than one Trial object was found in the children of the given node."));
    auto trial = trials[0];
    // Create a native binary stream
    BinaryStream stream(optr->Source);
    size_t writtenBytes = 0;
    double sampleRate = 0.0;
    double startTime = 0.0;
    unsigned frames = 0;
    double pointScaleFactor = 1.0;
    uint16_t numberAnalogSamplesPerPointSample = 1;
    const size_t maxNumEventsHeader = 18;
    std::vector<const TimeSequence*> points, analogs;
    std::vector<const Event*> events, eventsHeader;
    // Prepare the content of the Trial
    //  - Any timesequence? If yes, then this is not a template file
    auto timeSequencesNode = trial->findChild("TimeSequences",{},false);
    if ((timeSequencesNode != nullptr) && !timeSequencesNode->children().empty())
    {
      // Verify if:
      //  1. All the time sequence starts at the same time
      //  2. All the sample frequencies are the same
      //  3. All the number of samples are the same
      // Do it on the points
      points = timeSequencesNode->findChildren<const TimeSequence*>({},{{"components",4}},false);
      auto it = points.begin();
      while (it != points.end())
      {
        if (((*it)->type() & TimeSequence::Reconstructed) != TimeSequence::Reconstructed)
          it = points.erase(it);
        else
          ++it;
      }
      bool first = true;
      for (const auto& point: points)
      {
        if (first)
        {
          sampleRate = point->sampleRate();
          startTime = point->startTime();
          frames = point->samples();
          pointScaleFactor = point->scale();
          first = false;
        }
        if (fabs(point->sampleRate() - sampleRate) > std::numeric_limits<float>::epsilon())
          throw(FormatError("ORG.C3D - The TimeSequence '" + point->name() + "' marked as 'Reconstructed' with 4 components does not have the same sample frequency than the others."));
        if (fabs(point->startTime() - startTime) > std::numeric_limits<float>::epsilon())
          throw(FormatError("ORG.C3D - The TimeSequence '" + point->name() + "' marked as 'Reconstructed' with 4 components does not have the same start time than the others."));
        if (point->samples() != frames)
          throw(FormatError("ORG.C3D - The TimeSequence '" + point->name() + "' marked as 'Reconstructed' with 4 components does not have the same number of samples than the others."));
      }
      // And then the analog channels
      analogs = timeSequencesNode->findChildren<const TimeSequence*>({},{{"type",TimeSequence::Analog},{"components",1}},false);
      optr->AnalogChannelScale.resize(analogs.size());
      optr->AnalogZeroOffset.resize(analogs.size());
      first = true;
      size_t inc = 0;
      for (const auto& analog: analogs)
      {
        if (first)
        {
          numberAnalogSamplesPerPointSample = analog->samples() / frames;
          first = false;
        }
        if ((analog->samples() / frames) != numberAnalogSamplesPerPointSample)
          throw(FormatError("ORG.C3D - The TimeSequence '" + analog->name() + "' marked as 'Analog' with 1 component does not have the same number of samples than the others."));
        if ((analog->samples() % frames) != 0)
          throw(FormatError("ORG.C3D - The TimeSequence '" + analog->name() + "' marked as 'Analog' with 1 component does not have the same sample rate than the others."));
        optr->AnalogChannelScale[inc] = analog->scale();
        optr->AnalogZeroOffset[inc] = analog->offset();
        ++inc;
      }
    }
    int firstFrame = static_cast<int>(startTime * sampleRate) + 1;
    int lastFrame = firstFrame + frames - 1;
    const Any& pointMaximumInterpolationGapProperty = trial->property("c3d_pointmaxinterpgap");
    uint16_t pointMaximumInterpolationGap = pointMaximumInterpolationGapProperty.isValid() ? pointMaximumInterpolationGapProperty.cast<uint16_t>() : 10;
    //  - Look for events to store in the header.
    auto eventsNode = trial->findChild("Events",{},false);
    if (eventsNode != nullptr)
    {
      events = eventsNode->findChildren<const Event*>();
      auto it = events.begin();
      while (it != events.end())
      {
        if ((*it)->property("c3d_fromheader").cast<bool>())
        {
          if ((*it)->name().size() > 4)
            throw(FormatError("ORG.C3D - The event '" + (*it)->name() + "' is set to be stored in the format header but its label has more than 4 characters."));
          eventsHeader.push_back(*it);
          it = events.erase(it);
        }
        else
          ++it;
      }
      if (eventsHeader.size() > maxNumEventsHeader)
        throw(FormatError("ORG.C3D - The number of events to store in the header is greater to the maximum number allowed (i.e. " + std::to_string(maxNumEventsHeader) + ")."));
    }
    bool templateContent = analogs.empty() && points.empty() && events.empty() && eventsHeader.empty();
    // Write the prepared content
    if (!templateContent)
    {
    // ====== //
    // HEADER //
    // ====== //
      // The number of the first block of the Parameter section
      writtenBytes += stream.writeI8(static_cast<int8_t>(2));
      // C3D header key
      writtenBytes += stream.writeI8(static_cast<int8_t>(80));
      // Number of points
      writtenBytes += stream.writeU16(static_cast<uint16_t>(points.size()));
      // Total number of analog samples per 3d frame
      writtenBytes += stream.writeU16(static_cast<uint16_t>(analogs.size() * numberAnalogSamplesPerPointSample));
      // First frame
      writtenBytes += stream.writeU16(static_cast<uint16_t>(firstFrame > 65535 ? 65535 : firstFrame));
      // Last frame
      writtenBytes += stream.writeU16(static_cast<uint16_t>(lastFrame > 65535 ? 65535 : lastFrame));
      // Maximum interpolation gap in 3D frames
      writtenBytes += stream.writeU16(static_cast<uint16_t>(pointMaximumInterpolationGap));
      // The 3D scale factor
      writtenBytes += stream.writeFloat(-1.0f * static_cast<float>(pointScaleFactor));
      // The (false) number of the first block of the Data section
      writtenBytes += stream.writeU16(static_cast<uint16_t>(0));
      // The number of analog samples per analog channel
      writtenBytes += stream.writeU16(static_cast<uint16_t>(numberAnalogSamplesPerPointSample));
      // The 3D frame rate
      writtenBytes += stream.writeFloat(static_cast<float>(sampleRate));
      // For future used : word 13-147 => 135 words unused => 270 bytes
      writtenBytes += stream.fill(270);
      // Label and Range data
      writtenBytes += stream.writeU16(static_cast<uint16_t>(0));
      // The first block of the Label and Range section
      writtenBytes += stream.writeU16(static_cast<uint16_t>(0));
      // The event label format. 
      writtenBytes += stream.writeU16(static_cast<uint16_t>(12345));
      // Number of defined time events
      writtenBytes += stream.writeI16(static_cast<int16_t>(eventsHeader.size()));
      // Word 152 : Reserved for future use
      writtenBytes += stream.fill(2);
      // Event time
      for(const auto& event : eventsHeader)
        writtenBytes += stream.writeFloat(static_cast<float>(event->time()));
      writtenBytes += stream.fill(4*(maxNumEventsHeader-eventsHeader.size()));
      // Event display flags (not supported in OpenMA. Always set to 1)
      for(size_t i = 0, len = eventsHeader.size() ; i < len ; ++i)
        writtenBytes += stream.writeI8(static_cast<int8_t>(1));
      writtenBytes += stream.fill(maxNumEventsHeader-eventsHeader.size());
      // Word 198 : Reserved for future use
      writtenBytes += stream.fill(2);
      // Event labels
      for(const auto& event : eventsHeader)
      {  
        std::string label = event->name();
        label.resize(4,' ');
        writtenBytes += stream.writeString(label);
      }
      writtenBytes += stream.fill(4*(maxNumEventsHeader-eventsHeader.size()));
      // Fill the end of the header section with 0x00
      stream.fill(512 - writtenBytes);
    }
    // ========= //
    // PARAMETER //
    // ========= //
    writtenBytes = 0;
    // The number of the first block of the Parameter data in the Parameter section
    writtenBytes += stream.writeI8(static_cast<int8_t>(1));
     // C3D header key
    writtenBytes += stream.writeI8(static_cast<int8_t>(80));
    // The (false) number of parameter block. This data is re-write at the end of this function
    writtenBytes += stream.writeI8(static_cast<int8_t>(0));
    // The processor type
    writtenBytes += stream.writeI8(static_cast<int8_t>(static_cast<int>(stream.byteOrder()) + 83));
    // Transfrom the trial's dynamic properties into group and parameters
    const auto& dynamicProperties = trial->dynamicProperties();
    std::vector<const char*> properties;
    properties.reserve(dynamicProperties.size());
    for (auto it = dynamicProperties.cbegin() ; it != dynamicProperties.cend() ; ++it)
    {
      // Remove properties that do not find the required format: 'GROUP:PARAMETER'.
      if (it->first.find(':') == std::string::npos)
        continue;
      // Special parameter updated during the writing 
      if (it->first.compare("POINT:DATA_START") == 0)
        continue;
      properties.push_back(it->first.c_str());
    }
    std::sort(properties.begin(), properties.end(), [](const char* lhs, const char* rhs){return strcmp(lhs,rhs) < 0;});
    using group_t = std::tuple<const char*,uint8_t,int8_t>; // label (chars & len), id
    std::vector<group_t> groups;
    using parameter_t = std::tuple<const char*,uint8_t,const Any&,int8_t>; // label (chars & len), value, id
    std::vector<parameter_t> parameters;
    const char* cur = nullptr;
    int id = 0;
    int8_t idPoint = 0;
    for (auto it = properties.cbegin() ; it != properties.cend() ; ++it)
    {
      const char* ch = strchr(*it,':');
      size_t idx = ch - *it;
      if ((idx == 0) || (idx > 127))
        throw(FormatError("ORG.C3D - The name of the group must be between 1 and 127 characters: '" + std::string(*it,idx+1) + "'."));
      if ((cur == nullptr) || (strncmp(cur, *it, idx) != 0))
      {
        if (id > 127)
          throw(FormatError("ORG.C3D - A maximum of 127 groups was reached."));
        groups.emplace_back(*it, static_cast<uint8_t>(idx), static_cast<int8_t>(++id));
        if (strncmp(*it, "POINT", idx) == 0)
          idPoint = id;
        cur = *it;
      }
      ++ch; ++idx; // Next character
      size_t len = strlen(*it) - idx;
      if ((len == 0) || (len > 127))
        throw(FormatError("ORG.C3D - The name of the parameter must be between 1 and 127 characters: '" + std::string(ch,idx+1) + "'."));
      const auto& value = dynamicProperties.find(*it)->second;
      if (!value.isArithmetic() && !value.isString())
        throw(FormatError("ORG.C3D - The value stored in the ma::Any object '" + std::string(*it) + "' is not compatible with the C3D format."));
      parameters.emplace_back(ch, static_cast<uint8_t>(len), value, id);
    }
    for (const auto& group: groups)
    {
      // Number of characters in the name
      writtenBytes += stream.writeI8(std::get<1>(group));
      // ID number
      writtenBytes += stream.writeI8(static_cast<int8_t>(-1 * std::get<2>(group)));
      // Name
      writtenBytes += stream.writeChar(std::get<1>(group),std::get<0>(group));
      // The Offset to the next parameter/group
      writtenBytes += stream.writeU16(static_cast<uint16_t>(3));
      // Number of characters in the description
      writtenBytes += stream.writeU8(static_cast<uint8_t>(0));
      // The description
      // writtenBytes += stream.writeString(???);
    }
    for (const auto& parameter: parameters)
    {
      // Verify the compatibility of the property (format, dimensions, etc.)
      const auto& data = std::get<2>(parameter);
      const auto& type = data.type();
      bool single = (data.size() == 1);
      auto dimensions = data.dimensions();
      int8_t format = 0;
      std::vector<std::string> temp;
      std::function<size_t()> writeValue;
      if ((type == static_typeid<const char*>()) || (type == static_typeid<std::string>()))
      {
        format = -1;
        temp = data.cast<std::vector<std::string>>();
        if (single)
        {
          dimensions[0] = temp[0].length();
        }
        else
        {
          size_t max = 0;
          for (auto& t : temp)
            max = std::max(max, t.length());
          dimensions.insert(dimensions.begin(), max);
          for (auto& t : temp)
            t.resize(max,' ');
        }
        writeValue = [&]()->size_t{return stream.writeString(temp.size(), temp.data());};
      }
      else if ((type == static_typeid<bool>()) || (type == static_typeid<int8_t>()))
      {
        format = 1;
        if (single) dimensions.clear();
        writeValue = [&]()->size_t{return stream.writeI8(data.size(), data.cast<std::vector<int8_t>>().data());};
      }
      else if ((type == static_typeid<uint8_t>()) || (type == static_typeid<int16_t>()))
      {
        format = 2;
        if (single) dimensions.clear();
        writeValue = [&]()->size_t{return stream.writeI16(data.size(), data.cast<std::vector<int16_t>>().data());};
      }
      else if (type == static_typeid<float>())
      {
        format = 4;
        if (single) dimensions.clear();
        writeValue = [&]()->size_t{return stream.writeFloat(data.size(), data.cast<std::vector<float>>().data());};
      }
      else if ((type == static_typeid<double>()) || (type == static_typeid<long double>()))
      {
        // Check if the values are greater than the maximum allowed for a float
        const auto& values = data.cast<std::vector<double>>();
        for (const auto& value: values)
          if (value > std::numeric_limits<float>::max()) throw(FormatError("ORG.C3D - At least one value in a parameter exceeds the maximum for a real."));
        writeValue = [&]()->size_t{return stream.writeFloat(data.size(), data.cast<std::vector<float>>().data());};
        format = 4;
        if (single) dimensions.clear();
      }
      else 
      {
        // All the other formats are integer. Check if the values are not greater than the maximum allowed for an int16_t.
        const auto& values = data.cast<std::vector<int>>();
        for (const auto& value: values)
          if (value > std::numeric_limits<int16_t>::max()) throw(FormatError("ORG.C3D - At least one value in a parameter exceeds the maximum for an integer."));
        writeValue = [&]()->size_t{return stream.writeI16(data.size(), data.cast<std::vector<int16_t>>().data());};
        format = 2;
        if (single) dimensions.clear();
      }
      if (dimensions.size() > 7)
        throw(FormatError("ORG.C3D - The number of dimensions exceeds the maximum of 7 dimensions."));
      for (const auto& dim : dimensions)
        if (dim > 255) throw(FormatError("ORG.C3D - One of the dimension exceeds the maximum of 255 elements."));
      size_t size = std::accumulate(dimensions.begin(), dimensions.end(), std::abs(format), std::multiplies<size_t>());;
      size_t offset = 5 + dimensions.size() + size;
      if (offset > 65535)
        throw(FormatError("ORG.C3D - Elements' size exceeds the maximum of 65535 bytes."));
      std::vector<uint8_t> dims(dimensions.cbegin(), dimensions.cend());
      // Write the data
      // Number of characters in the name
      writtenBytes += stream.writeI8(std::get<1>(parameter));
      // ID number
      writtenBytes += stream.writeI8(std::get<3>(parameter));
      // Name
      writtenBytes += stream.writeChar(std::get<1>(parameter),std::get<0>(parameter));
      // Offset
      writtenBytes += stream.writeU16(static_cast<uint16_t>(offset));
      // Format
      writtenBytes += stream.writeI8(format);
      // Dimension(s)
      writtenBytes += stream.writeU8(static_cast<uint8_t>(dims.size()));
      writtenBytes += stream.writeU8(dims.size(), dims.data());
      // Values
      writtenBytes += writeValue();
      // Number of characters in the description
      writtenBytes += stream.writeU8(static_cast<uint8_t>(0));
      // The description
      // writtenBytes += stream.writeString(???);
    }
    // POINT:DATA_START final
    uint16_t dataStartBlock = 0;
    if (!templateContent)
    {
      size_t totalWrittenBytes = writtenBytes + 25; // 25: 1 + 1 + 16 + 2 + 1 + 1 + 2 + 1
      totalWrittenBytes += (512 - (totalWrittenBytes % 512));
      uint8_t pNB = static_cast<uint8_t>(totalWrittenBytes / 512);
      dataStartBlock = 2 + pNB;

      writtenBytes += stream.writeI8(10);
      // ID number
      writtenBytes += stream.writeI8(idPoint);
      // Name
      writtenBytes += stream.writeChar(10,"DATA_START");
      // Offset
      writtenBytes += stream.writeU16(7);
      // Format
      writtenBytes += stream.writeI8(2);
      // Dimension's size
      writtenBytes += stream.writeU8(0);
      // Values
      writtenBytes += stream.writeU16(dataStartBlock);
      // Number of characters in the description
      writtenBytes += stream.writeU8(static_cast<uint8_t>(0));

      writtenBytes += stream.fill(512 - (writtenBytes % 512));
      // Back to the parameter: number of blocks
      optr->Source->seek(512 * (2 - 1) + 2, Origin::Begin);
      stream.writeU8(pNB);
      // Back to the header: data first block
      optr->Source->seek(16, Origin::Begin);
      stream.writeU16(dataStartBlock);
    }
    else
    {
      writtenBytes += stream.fill(512 - (writtenBytes % 512));
      uint8_t pNB = static_cast<uint8_t>(writtenBytes / 512);
      // Back to the parameter: number of blocks
      optr->Source->seek(2, Origin::Begin);
      stream.writeU8(pNB);
    }
    if (writtenBytes > (255 * 512)) // 255 * 512 = max size
      throw(FormatError("ORG.C3D - Total size reserved for the parameters was exceeded."));
    
    // ==== //
    // DATA //
    // ==== //
    if (!templateContent)
    {
      optr->Source->seek(512 * (dataStartBlock - 1), Origin::Begin);
      C3DDataStreamFloat dataStream(&stream);
      for (size_t frame = 0 ; frame < frames ; ++frame)
      {
        for (const auto& point : points)
        {
          dataStream.writePoint(
            point->data()[frame],
            point->data()[frame + frames],
            point->data()[frame + 2*frames],
            point->data()[frame + 3*frames],
            pointScaleFactor);
        }
        size_t inc = 0, incChannel = 0, analogFrame = numberAnalogSamplesPerPointSample * frame;
        auto itA = analogs.cbegin();
        while (itA != analogs.cend())
        {
          dataStream.writeAnalog(
            (*itA)->data()[analogFrame]
            / optr->AnalogChannelScale[incChannel]
            / optr->AnalogUniversalScale
            + optr->AnalogZeroOffset[incChannel]);
          ++itA; ++incChannel;
          if ((itA == analogs.cend()) && (inc < static_cast<size_t>(numberAnalogSamplesPerPointSample - 1)))
          {
            itA = analogs.cbegin();
            incChannel = 0;
            ++inc; ++analogFrame;
          }
        }
      }
    }
  };
};
};