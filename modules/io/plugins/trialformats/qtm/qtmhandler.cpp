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

#include "qtmhandler.h"
#include "qtminternals.h"

#include "openma/io/handler_p.h"
#include "openma/io/binarystream.h"
#include "openma/io/buffer.h"
#include "openma/io/device.h"
#include "openma/io/handler.h" // FormatError
#include "openma/io/utils.h"
#include "openma/base/trial.h"
#include "openma/base/logger.h"

#include "lzo1xdec/lzo1xdec.h"

#include <iostream>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

namespace ma
{
namespace io
{
  class QTMHandlerPrivate : public HandlerPrivate
  {
  public:
    QTMHandlerPrivate();
    ~QTMHandlerPrivate() _OPENMA_NOEXCEPT;

    QTMHandlerPrivate(const QTMHandlerPrivate& ) = delete;
    QTMHandlerPrivate(QTMHandlerPrivate&& ) _OPENMA_NOEXCEPT = delete;
    QTMHandlerPrivate& operator=(const QTMHandlerPrivate& ) = delete;
    QTMHandlerPrivate& operator=(const QTMHandlerPrivate&& ) _OPENMA_NOEXCEPT = delete;
    
    void extractConfigurationProperties(QTMConfiguration::Item* item, std::vector<QTMConfiguration::DictPropertyEntry>* properties, BinaryStream* stream);
    void extractConfigurationChildren(QTMConfiguration::Item* item, QTMConfiguration* config, BinaryStream* stream);
    void extractConfiguration(QTMConfiguration* config, const std::string& configname, QTMCompoundDocument* doc, BinaryStream* stream);
  };
  
  QTMHandlerPrivate::QTMHandlerPrivate()
  : HandlerPrivate()
  {};

  QTMHandlerPrivate::~QTMHandlerPrivate() _OPENMA_NOEXCEPT = default;

  void QTMHandlerPrivate::extractConfigurationProperties(QTMConfiguration::Item* item, std::vector<QTMConfiguration::DictPropertyEntry>* properties, BinaryStream* stream)
  {
    int32_t numProperties = stream->readI32();
    item->Properties.reserve(numProperties);
    for (int32_t i = 0 ; i < numProperties ; ++i)
    {
      int32_t idx = stream->readI32();
      if (idx > static_cast<int32_t>(properties->size()))
        throw(FormatError("Corrupted file. Index for one property is out of range"));
      auto prop = new QTMConfiguration::Property;
      prop->DictEntry = &(properties->operator[](idx-1));
      int32_t offset = stream->readI32();
      int32_t len = 0;
      switch(prop->DictEntry->Flags[0])
      {
      case 3: // Signed integer 32-bit (4 bytes)
        prop->Value = stream->readI32();
        len = 4;
        break;
      case 7: // Unsigned integer 32-bit (4 bytes)
        prop->Value = stream->readU32();
        len = 4;
        break;
      case 9: // Float32 (4 bytes)
        prop->Value = stream->readFloat();
        len = 4;
        break;
      case 10: // Float64 (8 bytes)
        prop->Value = stream->readDouble();
        len = 8;
        break;
      case 11: // Boolean  (1 byte)
        prop->Value = stream->readI8();
        len = 1;
        break;
      case 13: // Enum (4 bytes)
        //  NOTE: The enumators have to be found from the 4th proprerty
        prop->Value = stream->readI32();
        len = 4;
        break;
      case 14: // Enumerators
        // It seems the 4th proprerty link to next enumators
        throw(FormatError("Type '14' used by the metadata '" + prop->DictEntry->Name + "' is recognized but not yet supported"));
        break;
      case 18: // Date (4 bytes)
        prop->Value = stream->readI32();
        len = 4;
        break;
      case 19: // TimeOfDay (4 bytes)
        prop->Value = stream->readI32();
        len = 4;
        break;
      case 20: // GUID (16 bytes)
        prop->Value = stream->readString(16);
        len = 16;
        break;
      case 21: // UnicodeString (dynamic length)
        {
        // FIXME: It it necessary to manage the encoding based on the content of the third flag (prop->DictEntry->Flags[2])?
        len = stream->readI32()*2 + 2; // *2: UTF-16 ; +2: Null character
        std::vector<char> temp(len, 0x00);
        stream->readChar(len,temp.data());
        std::string str(len/2 - 1, '\0');
        for (size_t j = 0 ; j < str.length() ; ++j)
          str[j] = temp[2*j];
        trim_string(&str,0x00);
        prop->Value = str;
        len += 4; // +4: dynamic size
        }
        break;
      case 23: // Array?
        throw(FormatError("Type '23' used by the metadata '" + prop->DictEntry->Name + "' is recognized but not yet supported"));
        break;
      case 24: // Structure
        {
        // NOTE: Due to the large possibilities of a structure, only the raw binary data are stored in the property. A btk::Buffer associated with a btk::BinaryStream should be used to extract the exact content of a structure
        std::vector<char> temp(offset, 0x00);
        stream->readChar(offset,temp.data());
        prop->Value = temp;
        len = offset;
        }
        break;
      case 25: // ContainerReference
        // FIXME: How to manage this type?
        prop->Value = stream->readI32();
        len = 4;
        break;
      default:
        throw(FormatError("Unexpected type for property '" + prop->DictEntry->Name + "': " + std::to_string(prop->DictEntry->Flags[0])));
      }
      stream->device()->seek(offset-len, Origin::Current);
      item->Properties.emplace_back(prop);
    }
    // Extra values?
    int32_t numExtraValues = stream->readI32();
    if (numExtraValues != 0)
    {
      item->Extras.resize(numExtraValues);
      for (int32_t i = 0 ; i < numExtraValues ; ++i)
      {
        int32_t indexExtra = stream->readI32();
        if ((indexExtra < 1) || (indexExtra > numExtraValues))
          throw(FormatError("Corrupted file. Index for extra value is out of range"));
        item->Extras[indexExtra-1] = stream->readI32();
      }
    }
  };

  void QTMHandlerPrivate::extractConfigurationChildren(QTMConfiguration::Item* item, QTMConfiguration* config, BinaryStream* stream)
  {
    if (item->DictIndex < 1)
      throw(FormatError("Corrupted file. An index value is negative"));
    if (item->DictIndex > static_cast<int32_t>(config->DictItemEntries.size()))
      throw(FormatError("Corrupted file. An index value exceeds the number of extracted item(s)"));
    item->DictEntry = &(config->DictItemEntries[item->DictIndex-1]);
    // std::cout << "\n * " << (*groups)[idx-1].name << std::endl;
    this->extractConfigurationProperties(item, &(config->DictPropertyEntries), stream);
    int32_t numChildren = stream->readI32();
    item->Children.reserve(numChildren);
    for (int32_t i = 0 ; i < numChildren ; ++i)
    {
      auto child = new QTMConfiguration::Item;
      child->DictIndex = stream->readI32();
      child->GlobalIndex = stream->readI32(); // ???
      this->extractConfigurationChildren(child, config, stream);
      item->Children.emplace_back(child);
    }
    int32_t numTOC = stream->readI32();
    if (numTOC != 0)
    {
      for (int32_t i = 0 ; i < numTOC ; ++i)
      {
#if 1
        stream->device()->seek(4, Origin::Current);
#else
        int32_t category = stream->readI32();
#endif
        int32_t numItems = stream->readI32();
        // Let's skip this part for the moment. Give the order for the read items
        stream->device()->seek(numItems*4, Origin::Current);
      }
    }
  };

  void QTMHandlerPrivate::extractConfiguration(QTMConfiguration* config, const std::string& configname, QTMCompoundDocument* doc, BinaryStream* stream)
  {
    Device* origDevice = stream->device();
    // The information given to the buffer device is not complete. The chunks associated with each entry are set in the method QTMCompoundDocument::setBufferToEntry.
    Buffer buffer;
    buffer.open(origDevice->data(), origDevice->size());
    // The binary stream now rely on the data of the buffer.
    stream->setDevice(&buffer);
    // Look for the metadata...
    // ------------------------
    if (doc->setBufferToEntry(&buffer, configname + "/Metadata") == 0)
      throw(FormatError("Corrupted file. One entry for the project settings was not found (Project Metadata/Metadata)"));
    // 1. Extract the items configuration (name, type)
    int32_t numItems = stream->readI32();
    if (stream->readI32() != 192)
      throw(FormatError("Unexpected key to extract entries for the item dictionary. This might be a limitation of the parser"));
    config->DictItemEntries.reserve(numItems);
    for (int32_t i = 0 ; i < numItems ; ++i)
    {
      // Name: ASCII string
      std::string name = stream->readString(stream->readI32());
      trim_string(&name,0x00);
      // Name: UTF-16 string (not used)
      stream->device()->seek(stream->readI32(), Origin::Current);
      // Type?
      config->DictItemEntries.emplace_back(name, stream->readI32());
      // std::cout << " * " << groups.size() << ". " << entry.name << " (type: " << entry.value.cast<std::string>() << ")" << std::endl;
    }
    int32_t numProperties = stream->readI32();
    if (stream->readI32() != 276)
      throw(FormatError("Unexpected key to extract entries for the propriety dictionary. This might be a limitation of the parser"));
    config->DictPropertyEntries.reserve(numProperties);
    for (int32_t i = 0 ; i < numProperties ; ++i)
    {
      // Name: ASCII string
      std::string name = stream->readString(stream->readI32());
      trim_string(&name,0x00);
      // Description? (ASCII string)
      std::string description = stream->readString(stream->readI32());
      trim_string(&description,0x00);
      // Name: UTF-16 string (not used)
      stream->device()->seek(stream->readI32(), Origin::Current);
      // Flags? (20 bytes)
      std::array<int32_t,5> flags;
      stream->readI32(5,flags.data());
      config->DictPropertyEntries.emplace_back(name, description, flags);

#if 1      
      const auto& entry = config->DictPropertyEntries.back();
      std::cout << " - " << config->DictPropertyEntries.size() << ". " << entry.Name << " (type: " << entry.Flags[0] << " ; f[1]: " << entry.Flags[1] << " ; f[2]: " << entry.Flags[2] << " ; f[3]: " << entry.Flags[3] << " ; f[4]: " << entry.Flags[4] << ")" << std::endl;
#endif
    }
    // ... And then the associated values
    // ----------------------------------
    if (doc->setBufferToEntry(&buffer, configname + "/Data Items") == 0)
      throw(FormatError("Corrupted file. One entry for the project settings was not found (Project Metadata/Data Items)"));
    config->RootItem = new QTMConfiguration::Item;
    config->RootItem->DictIndex = 1;
    this->extractConfigurationChildren(config->RootItem, config, stream);
    // Set back the stream to its original device
    stream->setDevice(origDevice);
  };
};
};

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

_OPENMA_CONSTEXPR uint8_t _openma_io_qtm_olecf_magic[8] = {0xD0,0xCF,0x11,0xE0,0xA1,0xB1,0x1A,0xE1};

namespace ma
{
namespace io
{

  /*
   * Note: Events containing the property "qtm_fromheader" will be stored in the header of the format. However a maximum of 18 events
   */

  QTMHandler::QTMHandler()
  : Handler(*new QTMHandlerPrivate)
  {};
  
  QTMHandler::~QTMHandler() _OPENMA_NOEXCEPT = default;
  
  Signature QTMHandler::verifySignature(const Device* const device) _OPENMA_NOEXCEPT
  {
    char signature[8] = {0};
    device->peek(signature,sizeof(signature));
    for (size_t i = 0 ; i < sizeof(signature) ; ++i)
    {
      if (static_cast<uint8_t>(signature[i]) != _openma_io_qtm_olecf_magic[i])
        return Signature::Invalid;
    }
    return Signature::Valid;
  };

  
  Signature QTMHandler::verifySignature() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return verifySignature(optr->Source);
  };
  
  void QTMHandler::readDevice(Node* output)
  {
    auto optr = this->pimpl();
    BinaryStream stream(optr->Source);
    
    optr->Source->setExceptions(State::End | State::Fail | State::Error);
    
    // Jump to Byte order identifier to extract correctly the other information
    // The signature is not checked as it is already done
    optr->Source->seek(28, Origin::Begin);
    char byteOrder[2] = {0};
    optr->Source->read(byteOrder,2);
    if ((byteOrder[0] == char(0xFE)) && (byteOrder[1] == char(0xFF)))
    {
      stream.setByteOrder(ByteOrder::IEEELittleEndian);
    }
    else if ((byteOrder[0] == char(0xFF)) && (byteOrder[1] == char(0xFE)))
    {
      stream.setByteOrder(ByteOrder::IEEEBigEndian);
    }
    else
      throw(FormatError("Unknown byte order identifier"));
    
    // Go back after the signature
    optr->Source->seek(8, Origin::Begin);
    // Load the internal compound document
    QTMCompoundDocument doc;
    doc.load(&stream);
    
    // At this point the trial can be created
    Trial* trial = new Trial(strip_path(optr->Source->name()),output);
    
    // The binary stream now relies on the buffer content.
    Buffer buffer;
    buffer.open(optr->Source->data(), optr->Source->size());
    stream.setDevice(&buffer);
    
    // Extract markers data
    // FIXME: What is the unit for markers' coordinates?
    // FIXME: What is the samples frequency for markers?
    if (doc.setBufferToEntry(&buffer, "Measurement/3D points") != 0)
    {
      // NOTE: This is not sure if the internal structure is fixed or not
      //       but the tested QTM files have each time 4 elements in the
      //       entry "Measurement/3D points":
      //       1. Unknown (was 0 each time)
      //       2. Unknown (was 0 each time)
      //       3. Markers data
      //       4. Seems to be a table with some indexes inside. 
      //          This part is not yet understood
      if (stream.readI32() != 4)
        throw(FormatError("Unsupported number of elements for the entry 'Measurement/3D points'. This might be a limitation of the parser"));
      for (int32_t i = 0 ; i < 4 ; ++i)
      {
        uint32_t blockIndex = stream.readU32();
        uint32_t blockSize = stream.readU32();
        // The index 3 is the block with the markers data
        if (blockIndex == 3)
        {
          // General number of samples 
          uint32_t numSamples = stream.readU32();
          // Array with (valid?) points' ID
          uint32_t numValidMarkers = stream.readU32() / 4;
          std::vector<uint32_t> pids(numValidMarkers,0);
          stream.readU32(pids.size(), pids.data());
          uint32_t numStoredMarkers = stream.readU32();
          for (uint32_t j = 0 ; j < numStoredMarkers ; ++j)
          {
            uint32_t indexMarker = stream.readU32();
            uint32_t dataSize = stream.readU32();
            // In case the extracted ID matches one of the value in the array
            if (std::find(pids.begin(), pids.end(), indexMarker) != pids.end())
            {
              --numValidMarkers;
              // The next 36 bytes are not understood...
              buffer.seek(36, Origin::Current);
              // Marker's name
              int32_t numBytesLabel = stream.readI32();
              int32_t numCharLabel = numBytesLabel / 2 - 1;
              std::vector<char> temp(numBytesLabel, 0x00);
              stream.readChar(temp.size(), temp.data());
              std::string label;
              if (numBytesLabel != 2)
              {
                label.resize(numCharLabel);
                for (int32_t k = 0 ; k < numCharLabel ; ++k)
                  label[k] = temp[2*k];
                trim_string(&(label),0x00);
              }
              else // Null character
                label = "uname*" + std::to_string(j+1);
              // std::cout << " - Marker's name: " << label << std::endl;
              // Number of parts in the reconstructed trajectory
              uint32_t numParts = stream.readU32();
              // std::cout << "   - parts: " << numParts << std::endl;
              // For each part...
              for (uint32_t k = 0 ; k < numParts ; ++k)
              {
                // Size of this part
                uint32_t partSize = stream.readU32();
                // What is the meaning of these 2 bytes?
                if (stream.readU16() != 1)
                  throw(FormatError("Unexpected extracted value inside markers part. This might be a limitation of the parser"));
                // Starting sample (seems to be a 1-based index)
                uint32_t partStartIndex = stream.readU32() - 1;
                // Number of samples
                uint32_t partNumSamples = stream.readU32();
                if ((partStartIndex + partNumSamples) > numSamples)
                  throw(FormatError("Total number of samples for marker '" + label + "' exceeded"));
                // Array of 'partNumSamples' x 4 double (8 bytes)
                buffer.seek(partSize-10, Origin::Current);
              }
            }
            // Otherwise, let's move to the next marker
            else
              buffer.seek(dataSize, Origin::Current);
          }
          if (numValidMarkers != 0)
            warning("QUALISYS.QTM - %s - Not every valid markers were found", optr->Source->name());
          break;
        }
        else
        {
          // Let's move to the next block
          buffer.seek(blockSize, Origin::Current);
        }
      }
    }
    
    // FIXME: How-to retrieve the number of force platforms?
    //        No usefull information was found in Measurement/Forceplates/Data Members
#if 0
    // Extract force platform configuration
    if (0)
    {
      uint32_t numForceplates = 0; // ???
      for (uint32_t i = 0 ; i < numForceplates ; ++i)
      {
        // FIXME: Extract the good name associated with each force platform
        if (doc.setBufferToEntry(&buffer, "Measurement/Forceplates/???/Settings") != 0)
        {
          int32_t numElements = stream.readI32();
            throw(FormatError("Unsupported number of elements for the entry 'Measurement/3D points'. This might be a limitation of the parser"));
          for (int32_t j = 0 ; j < numElements ; ++j)
          {
            uint32_t blockIndex = stream.readU32();
            uint32_t blockSize = stream.readU32();
            // #1: FIXME: What contains this element?
            // #2: FIXME: What contains this element? Could be the activation of the forceplate
            // #3: FIXME: What contains this element?
            // #4: Name (UTF-16)
            if (blockIndex == 4)
            {
              std::vector<char> temp(blockSize*2, 0x00);
              stream.readChar(temp.size(), temp.data());
              std::string label(blockSize-1, 0x00);
              for (size_t k = 0 ; k < label.size() ; ++k)
                label[k] = temp[2*k];
              trim_string(&label,0x00);
            }
            // #5: Corners coordinates
            else if (blockIndex == 5)
            {
              if (blockSize < 48) // 4 corners x 3 coordinates x 4 bytes (float 32-bit)
                throw(FormatError("Wrong data size for corners coordinates"));
              std::array<float,12> corners;
              stream.readFloat(corners.size(), corners.data());
            }
            // #6: Analog channel indices
            // NOTE: Use the property 'Channel number' of each child of 'Analog Data' to get the associated signal
            else if (blockIndex == 6)
            {
              // FIXME: How do you know the number of channels associated with this forceplate?
              uint32_t numChannels = blockSize / 4;
              std::vector<int32_t> indexChannels(numChannels,-1);
              stream.readI32(indexChannels.size(), indexChannels.data());
            }
            // #7: FIXME: What contains this element? Seems to be used to scale something
            // #8: Calibration matrix
            else if (blockIndex == 8)
            {
              uint32_t num = blockSize / 8;
              // TODO: Had a test to check is the size of the calibration matrix corresponds to the input/output of the platform
              std::vector<double> calibration(num,0.0);
              stream.readDouble(calibration.size(), calibration.data());
            }
            // #9: FIXME: What contains this element?
            // #10: FIXME: What contains this element?
            // #11: FIXME: What contains this element?
            // #12: Width (unit? Millimeter everytime?)
            else if (blockIndex == 12)
            {
              double with = stream.readDouble();
            }
            // #13: Length (unit? Millimeter everytime?)
            else if (blockIndex == 13)
            {
              double length = stream.readDouble();
            }
            // #14: Origin X
            else if (blockIndex == 14)
            {
              double origX = stream.readDouble();
            }
            // #15: Origin Y
            else if (blockIndex == 15)
            {
              double origY = stream.readDouble();
            }
            // #16: Origin Z
            else if (blockIndex == 16)
            {
              double origZ = stream.readDouble();
            }
            else // Let's move to the next block
              buffer.seek(blockSize, Origin::Current);
            // #17 and more ... FIXME: What contains these elements?
          }
          
        }
      }
    }
#endif
    
    // Extract events
    if (doc.setBufferToEntry(&buffer, "Measurement/Events") != 0)
    {
      if (stream.readI32() != 2)
        throw(FormatError("Unsupported number of elements for the entry 'Measurement/Events'. This might be a limitation of the parser"));
      const uint32_t eventBlockSize = 112;
      uint32_t numEvents = -1;
      for (int32_t i = 0 ; i < 2 ; ++i)
      {
        uint32_t blockIndex = stream.readU32();
        uint32_t blockSize = stream.readU32();
        // The first block contains the label and the time associated with each event
        if (blockIndex == 1)
        {
          numEvents = blockSize / eventBlockSize;
          for (uint32_t j = 0 ; j < numEvents ; ++j)
          {
            // Time (in seconds)
            double t = stream.readDouble();
            // Label
            std::string n = stream.readString(4);
            trim_string(&n,0x00);
            // Next 100 bytes could be to store a descrption?
            buffer.seek(100, Origin::Current);
            
            std::cout << "- Event" << n << ": " << t << std::endl;
          }
        }
        // The second block might be related to the activation of an event but this is not sure ... Some tests are added below in case unexpected values were found. This would help to uderstand the exact content of this block.
        else if (blockIndex == 2)
        {
          // NOTE: It is assumed that the second block is everytime after the first block.
          if (blockSize/4 != numEvents)
            throw(FormatError("Unexpected result in the second block of the entry 'Measurement/Events'. This might be a limitation of the parser"));
          for (uint32_t j = 0 ; j < numEvents ; ++j)
          {
            if (stream.readU32() != 255)
              throw(FormatError("Unexpected value in the second block of the entry 'Measurement/Events'. This might be a limitation of the parser"));
          }
        }
        // This would not be possible...
        else
          throw(FormatError("Unsupported block index for the entry 'Measurement/Events'. This might be a limitation of the parser"));
      }
    }
    
    // Extract the configuration "Measurement NBC" which contains the analog data
    QTMConfiguration measurementNBC;
    optr->extractConfiguration(&measurementNBC, "Measurement NBC", &doc, &stream);
    auto analogConfig = measurementNBC.findItem("Analog Data");
    if (analogConfig != nullptr)
    {
      int inc = 1;
      std::string label;
      std::vector<char> encoded, decoded;
      for (const auto child: analogConfig->Children)
      {
        auto labelProp = child->property("Label");
        if (labelProp == nullptr)
          label = "uname*" + std::to_string(inc);
        else
          label = labelProp->Value.cast<std::string>();
        auto seriesidProp = child->property("Series ID");
        if (seriesidProp == nullptr)
          throw(FormatError("Corrupted file. Series ID associated with analog data '" + label + "' not found"));
        if (child->Extras.empty())
          throw(FormatError("Corrupted file. Number of samples for analog data '" + label + "' not found"));
        uint32_t numSamples = child->Extras[0].cast<uint32_t>();
        struct DataSeriesIndex
        {
          uint32_t offsetIn;
          uint32_t offsetOut;
          uint32_t sizeIn;
          uint32_t sizeOut;
        };
        std::vector<DataSeriesIndex> buffersInfo;
        // Data series index: contains information of compressed buffers
        if (doc.setBufferToEntry(&buffer, "Measurement NBC/Data series/" + seriesidProp->Value.cast<std::string>() + "Index") != 0)
        {
          buffersInfo.resize(stream.readU32());
          for (size_t i = 0 ; i < buffersInfo.size() ; ++i)
          {
            auto& info = buffersInfo[i];
            info.offsetIn = stream.readU32();
            info.offsetOut = stream.readU32();
            info.sizeIn = stream.readU32();
            info.sizeOut = stream.readU32();
          }
        }
        else
          throw(FormatError("Corrupted file. Data series index associated with analog data '" + label + "' not found"));
        // Data series: contains compressed buffer(s)
        if (doc.setBufferToEntry(&buffer, "Measurement NBC/Data series/" + seriesidProp->Value.cast<std::string>()) != 0)
        {
          uint32_t offsetIn = 0, readIn = 0, offsetOut = 0;
          for (size_t i = 0 ; i < buffersInfo.size() ; ++i)
          {
            auto& info = buffersInfo[i];
            offsetIn += info.offsetIn;
            buffer.seek(offsetIn-readIn, Origin::Current);
            encoded.resize(info.sizeIn);
            decoded.resize(info.sizeOut);
            int remOutLen = static_cast<int>(decoded.size());
            int remInLen = static_cast<int>(encoded.size());
            stream.readChar(encoded.size(), encoded.data());
            lzo1x_decode(decoded.data(), &remOutLen, encoded.data(), &remInLen);
            if ((remInLen != 0) || (remOutLen != 0))
              throw(FormatError("Error during data decompression for analog data '" + label + "'"));
            readIn += info.sizeIn;
            offsetOut += info.offsetOut + info.sizeOut;
          }
            
          // Need to set back to the buffer associated with the original device
          stream.setDevice(&buffer);

          // std::cout << label << ": " << encodedSize  << ", "<< child->property("Time Base ID")->Value.cast<std::string>() << std::endl;
        }
        else
          throw(FormatError("Corrupted file. Data series associated with analog data '" + label + "' not found"));
        ++inc;
      }
    };
  };
};
};