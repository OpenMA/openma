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

#include "qtminternals.h"

#include "openma/io/binarystream.h"
#include "openma/io/buffer.h"
#include "openma/io/device.h"
#include "openma/io/handler.h" // FormatError
#include "openma/io/utils.h"

#include <cassert>

// #define _OPENMA_IO_QTM_DEBUG
#ifdef _OPENMA_IO_QTM_DEBUG
#include <iostream>
void _openma_io_qtm_debug_disp_compound_directoy(btk::QTMCompoundDocument* olecf, btk::QTMCompoundDocument::DictEntry* entry, const std::string& indent = "");
#endif

namespace ma
{
namespace io
{
  QTMConfiguration::Item::~Item()
  {
    for (auto property: this->Properties)
      delete property;
    for (auto child: this->Children)
      delete child;
  };

  const QTMConfiguration::Property* QTMConfiguration::Item::property(const std::string& name) const _OPENMA_NOEXCEPT
  {
    for (const auto property: this->Properties)
    {
      if (property->DictEntry->Name.compare(name) == 0)
        return property;
    }
    return nullptr;
  };

  QTMConfiguration::~QTMConfiguration()
  {
    delete this->RootItem;
  }

  const QTMConfiguration::Item* QTMConfiguration::findItem(const std::string& name) const _OPENMA_NOEXCEPT
  {
    for (const auto child: this->RootItem->Children)
    {
      if (child->DictEntry->Name.compare(name) == 0)
        return child;
    }
    return nullptr;
  };

  // ----------------------------------------------------------------------- //

  QTMCompoundDocument::QTMCompoundDocument()
  : sector_size(0), 
    short_sector_size(0), 
    num_sectors_SAT(0), 
    first_sid_DIR(0), 
    standard_stream_minimum_size(0), 
    first_sid_SSAT(0), 
    num_sectors_SSAT(0), 
    first_sid_MSAT(0), 
    num_sectors_MSAT(0), 
    sids_MSAT(nullptr), 
    sids_SAT(nullptr), 
    sids_SSAT(nullptr),
    sids_DRE(nullptr),
    dir_entries()
  {};

  QTMCompoundDocument::~QTMCompoundDocument()
  {
    if (this->sids_MSAT != 0)
      delete[] this->sids_MSAT;
    if (this->sids_SAT != 0)
      delete[] this->sids_SAT;
    if (this->sids_SSAT != 0)
      delete[] this->sids_SSAT;
    for (std::vector<DictEntry*>::iterator it = this->dir_entries.begin() ; it != this->dir_entries.end() ; ++it)
      delete *it;
    this->dir_entries.clear();
  };

  void QTMCompoundDocument::load(BinaryStream* stream)
  {
    // GUID - Class identifier (not used and seems to be set to 0 everytime)
    stream->device()->seek(16, Origin::Current);
    // File format revision number (should be all the time 4.62)
    int16_t minor = stream->readI16();
    int16_t major = stream->readI16();
    if ((minor != 62) && (major != 4))
      throw(FormatError("Only the compound format version 4.62 is supported"));
    // Byte order identifier (already done)
    stream->device()->seek(2, Origin::Current);
    // Size of a sector in the compound document file
    this->sector_size = static_cast<int32_t>(std::pow(2.0f, stream->readI16()));
    // Size of a short-sector in the short-stream container stream
    this->short_sector_size = static_cast<int32_t>(std::pow(2.0f, stream->readI16()));
    // Next 10 bytes might not used but the 7th byte has a value (0x06 or 0x08)...
    stream->device()->seek(10, Origin::Current);
    // Total number of sectors used for the sector allocation table (SAT).
    this->num_sectors_SAT = stream->readI32();
    // Sector identifier (SID) of first sector of the directory stream
    this->first_sid_DIR = stream->readI32();
    // Next 4 bytes are not used
    stream->device()->seek(4, Origin::Current);
    // Minimum number of bytes for a stream
    this->standard_stream_minimum_size = stream->readU32();
    // SID for the first sector in the Short SAT (SSAT)
    this->first_sid_SSAT = stream->readI32();
    // Total number of sectors used for the SSAT
    this->num_sectors_SSAT = stream->readI32();
    // SID for the first sector in the Master SAT (MSAT)
    this->first_sid_MSAT = stream->readI32();
    // Total number of sectors used for the MSAT
    this->num_sectors_MSAT = stream->readI32();
    // ------------------------------------------------------------------- //
    //                           ALLOCATION TABLES
    // ------------------------------------------------------------------- //
    // MSAT
    const int olecfNumSecIDsBySector_MSAT = (this->sector_size / 4) - 1;
    int olecfNumSecIDs_MSAT = 109 + this->num_sectors_MSAT * olecfNumSecIDsBySector_MSAT;
    this->sids_MSAT = new int32_t[olecfNumSecIDs_MSAT];
    stream->readI32(109, this->sids_MSAT);
    int olecfNumSecIDsRead_MSAT = 109;
    int32_t olecfNextSID_MSAT = this->first_sid_MSAT;
    for (int i = 0 ; i < this->num_sectors_MSAT ; ++i)
    {
      stream->device()->seek((olecfNextSID_MSAT+1) * this->sector_size, Origin::Begin); // +1: Header
      stream->readI32(olecfNumSecIDsBySector_MSAT, this->sids_MSAT+olecfNumSecIDsBySector_MSAT);
      olecfNumSecIDsRead_MSAT += olecfNumSecIDsBySector_MSAT;
      olecfNextSID_MSAT = stream->readI32();
    }
    if (olecfNextSID_MSAT != -2)
      throw(FormatError("Wrong SID for the last sector identifier used by the master sector allocation table."));
    // SAT
    const int olecfNumSecIDsBySector_SAT = this->sector_size / 4;
    int olecfNumSecIDs_SAT = this->num_sectors_SAT * olecfNumSecIDsBySector_SAT;
    this->sids_SAT = new int32_t[olecfNumSecIDs_SAT];
    for (int i = 0 ; i < olecfNumSecIDsRead_MSAT ; ++i)
    {
      if (this->sids_MSAT[i] == -1)
        continue;
      stream->device()->seek((this->sids_MSAT[i]+1) * this->sector_size, Origin::Begin);
      stream->readI32(olecfNumSecIDsBySector_SAT, this->sids_SAT+i*olecfNumSecIDsBySector_SAT);
    }
    if (this->sids_SAT[0] != -3)
      throw(FormatError("Corrupted file. Wrong SID for the first sector identifier used by the sector allocation table."));
    // SSAT
    const int olecfNumSecIDsBySector_SSAT = this->sector_size / 4;
    int olecfNumSecIDs_SSAT = this->num_sectors_SSAT * olecfNumSecIDsBySector_SSAT;
    this->sids_SSAT = new int32_t[olecfNumSecIDs_SSAT];
    int32_t olecfNextSID_SSAT = this->first_sid_SSAT;
    for (int32_t i = 0 ; i < this->num_sectors_SSAT ; ++i)
    {
      stream->device()->seek((olecfNextSID_SSAT+1) * this->sector_size, Origin::Begin);
      stream->readI32(olecfNumSecIDsBySector_SSAT, this->sids_SSAT+i*olecfNumSecIDsBySector_SSAT);
      olecfNextSID_SSAT = this->sids_SAT[olecfNextSID_SSAT];
    }
    if (olecfNextSID_SSAT != -2)
      throw(FormatError("Corrupted file. Wrong SID for the last sector identifier used by the short sector allocation table."));
    // ------------------------------------------------------------------- //
    //                                DIRECTORY
    // ------------------------------------------------------------------- //
    const int olecfNumEntriesBySector_DIR = this->sector_size / 128;
    int32_t next_sid_DIR = this->first_sid_DIR;
    // Try to guess the number of entries to reserve enough space for them
    size_t olecfNumEntries_DIR = 0;
    while (next_sid_DIR != -2)
    {
      olecfNumEntries_DIR += olecfNumEntriesBySector_DIR;
      next_sid_DIR = this->sids_SAT[next_sid_DIR];
    }
    if (olecfNumEntries_DIR == 0)
      throw(FormatError("Corrupted file. There is no entry in the QTMCompoundDocument dictionary which could not be possible."));
    this->dir_entries.reserve(olecfNumEntries_DIR);
    // Extract entries
    next_sid_DIR = this->first_sid_DIR;
    while (next_sid_DIR != -2)
    {
      stream->device()->seek((next_sid_DIR+1) * this->sector_size, Origin::Begin);
      for (int i = 0 ; i < olecfNumEntriesBySector_DIR ; ++i)
      {
        QTMCompoundDocument::DictEntry* entry = new QTMCompoundDocument::DictEntry;
        char dirNameWStr[64] = {0};
        stream->readChar(64, dirNameWStr);
        // We assume the wide string contains only extended ASCII characters.
        // Only the even characters are kept from 0 to 62. The last character must be always the null characeter.
        int16_t dirNameNumChar = stream->readI16()/2 - 1;
        dirNameNumChar = (dirNameNumChar < 0 ) ? 0 : dirNameNumChar;
        entry->name.resize(dirNameNumChar, ' ');
        for (int j = 0 ; j < dirNameNumChar ; ++j)
          entry->name[j] = dirNameWStr[2*j];
        trim_string(&(entry->name),0x00);
        entry->type = stream->readI8();
        // The next byte is the color of the entry stored a red/black tree. But we do not use this information here.
        stream->device()->seek(1, Origin::Current);
        entry->left_id = stream->readI32();
        entry->right_id = stream->readI32();
        entry->sub_id = stream->readI32();
        // Skip the next 36 bytes are they do not seem important here (Class identifier, user flags, creation time, modification time,)
        stream->device()->seek(36, Origin::Current);
        entry->first_sid = stream->readI32();
        entry->size = stream->readU32();
        // Last 4 bytes are reserved
        stream->device()->seek(4, Origin::Current);
      
        this->dir_entries.push_back(entry);
      }
      next_sid_DIR = this->sids_SAT[next_sid_DIR];
    }
    if (this->dir_entries[0]->name.compare("Root Entry") != 0)
      throw(FormatError("Corrupted file. Unexpected name for the first entry of the QTMCompoundDocument dictionary."));
    else
    {
      // Create the table to find where the SSAT are stored in the SAT
      int32_t num_sids_DRE = static_cast<int32_t>(std::ceil(static_cast<float>(this->dir_entries[0]->size) / static_cast<float>(this->sector_size)));
      this->sids_DRE = new int32_t[num_sids_DRE];
      int32_t entry_next_sid = this->dir_entries[0]->first_sid;
      int32_t inc = 0;
      while (entry_next_sid != -2)
      {
        assert(inc < num_sids_DRE);
        this->sids_DRE[inc] = entry_next_sid + 1;
        entry_next_sid = this->sids_SAT[entry_next_sid];
        ++inc;
      }
    }
  
#ifdef _OPENMA_IO_QTM_DEBUG
    std::cout << std::endl;
    _openma_io_qtm_debug_disp_compound_directoy(this, this->dir_entries[0]);
#endif
  };

  bool QTMCompoundDocument::setBufferToEntry(Buffer* buffer, const std::string& path)
  {
    QTMCompoundDocument::DictEntry* entry = this->findEntry(this->dir_entries[0], path);
    if ((entry == 0) || (entry->size == 0))
      return false;
    int32_t entry_next_sid = entry->first_sid;
    bool shortTable = (entry->size < this->standard_stream_minimum_size);
    int32_t chunk_size = shortTable ? this->short_sector_size : this->sector_size;
    int32_t shift_sid = shortTable ? 0 : 1;
    std::vector<size_t> chunk_ids;
    chunk_ids.reserve(entry->size / chunk_size);
    while (entry_next_sid != -2)
    {
      int32_t chunk_id = entry_next_sid + shift_sid;
      if (shortTable)
      {
        entry_next_sid = this->sids_SSAT[entry_next_sid];
        int32_t ssid = chunk_id * this->short_sector_size;
        int32_t sid = ssid / this->sector_size;
        chunk_id = (this->sids_DRE[sid] * this->sector_size + (ssid % this->sector_size)) / this->short_sector_size;
      }
      else
      {
        entry_next_sid = this->sids_SAT[entry_next_sid];
      }  
      chunk_ids.push_back(chunk_id);
    }
    buffer->setChunks(chunk_ids, chunk_size);
    return true;
  };

  // Find the entry with the given label. The character '/' has a special meaning used to split child from parent.
  QTMCompoundDocument::DictEntry* QTMCompoundDocument::findEntry(QTMCompoundDocument::DictEntry* parent, const std::string& path) const
  {
    if ((parent == 0) || (parent->sub_id == -1))
      return 0;
    QTMCompoundDocument::DictEntry* entry = 0;
    std::string current = path, next;
    size_t pos = path.find_first_of('/');
    if (pos != std::string::npos)
    {  
      current = path.substr(0,pos);
      next = path.substr(pos+1);
    }
    QTMCompoundDocument::DictEntry* child = this->findLevelEntry(this->dir_entries[parent->sub_id], current);
    if (next.empty())
      entry = child;
    else
      entry = this->findEntry(child, next);
    return entry;
  };

  QTMCompoundDocument::DictEntry* QTMCompoundDocument::findLevelEntry(QTMCompoundDocument::DictEntry* level, const std::string& name) const
  {
    if (level->name.compare(name) == 0)
      return level;
    if (level->left_id != -1)
    {
      QTMCompoundDocument::DictEntry* entry = this->findLevelEntry(this->dir_entries[level->left_id], name);
      if (entry != 0)
        return entry;
    }
    if (level->right_id != -1)
    {  
      QTMCompoundDocument::DictEntry* entry = this->findLevelEntry(this->dir_entries[level->right_id], name);
      if (entry != 0)
        return entry;
    }
    return 0;
  }
};
};

#ifdef _BTK_IO_QTM_DEBUG
  void _btk_io_qtm_debug_disp_compound_directoy(btk::QTMCompoundDocument* olecf, btk::QTMCompoundDocument::DictEntry* entry, const std::string& indent)
  {
    std::cout << indent;
    if (entry->type == btk::QTMCompoundDocument::DictEntry::RootStorage)
      std::cout << "* ";
    else if (entry->type == btk::QTMCompoundDocument::DictEntry::Storage)
      std::cout << "+ ";
    else if (entry->type == btk::QTMCompoundDocument::DictEntry::Stream)
      std::cout << "- ";
    else
      std::cout << "? ";
    std::cout << entry->name;
    if ((entry->type == btk::QTMCompoundDocument::DictEntry::Stream) || (entry->type == btk::QTMCompoundDocument::DictEntry::RootStorage))
    {
      std::cout << " (" << entry->size << " bytes)";
      int32_t entry_next_sid = entry->first_sid;
      bool shortTable = false;
      if (entry->size < olecf.standard_stream_minimum_size)
      {
        std::cout << " - SSAT";
        shortTable = true;
      }
      else
      {
        std::cout << " - SAT";
      }
      std::cout << " - SIDs: [";
      while (entry_next_sid != -2)
      {
        std::cout << entry_next_sid + (shortTable ? 0 : 1);
        entry_next_sid = shortTable ? olecf.sids_SSAT[entry_next_sid] : olecf.sids_SAT[entry_next_sid];
        if (entry_next_sid != -2)
          std::cout << ",";
      }
      std::cout << "]";
    }
    std::cout << std::endl;
    if (entry->sub_id != -1)
      _btk_io_qtm_debug_disp_compound_directoy(olecf, olecf.dir_entries[entry->sub_id], indent+"  ");
    if (entry->left_id != -1)
      _btk_io_qtm_debug_disp_compound_directoy(olecf, olecf.dir_entries[entry->left_id], indent);
    if (entry->right_id != -1)
      _btk_io_qtm_debug_disp_compound_directoy(olecf, olecf.dir_entries[entry->right_id], indent);
  };
#endif