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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
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

#ifndef __openma_body_enums_h
#define __openma_body_enums_h

#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

namespace ma
{
namespace body
{
  
#ifndef DOXYGEN_SHOULD_SKIP_THIS
  
  // NOTE: The next enums are fake strongly typed enumerations to simplify the syntax in the code (e.g. with the Segment class constructor).
  
  struct Side
  {
    enum: short
    {
      Center = 1,
      Left = 2,
      Right = 4,
      Both =  Left | Right
    };
    
    Side() = delete;
    ~Side() _OPENMA_NOEXCEPT = delete;
    Side(const Side& ) = delete;
    Side(Side&& ) _OPENMA_NOEXCEPT = delete;
    Side& operator=(const Side& ) = delete;
    Side& operator=(Side&& ) _OPENMA_NOEXCEPT = delete;
  };
  
  struct Region
  {
    enum: short
    {
      Upper = 1,
      Lower = 2,
      Full = Upper | Lower,
    };
    
    Region() = delete;
    ~Region() _OPENMA_NOEXCEPT = delete;
    Region(const Region& ) = delete;
    Region(Region&& ) _OPENMA_NOEXCEPT = delete;
    Region& operator=(const Region& ) = delete;
    Region& operator=(Region&& ) _OPENMA_NOEXCEPT = delete;
  };
  
  struct Part {
    enum: short
    {
      Head = 1,
      Torso,
      Clavicle,
      Arm,
      Forearm,
      Hand,
      Pelvis,
      Thigh,
      Shank,
      Foot,
      User = 1024
    };
    
    Part() = delete;
    ~Part() _OPENMA_NOEXCEPT = delete;
    Part(const Part& ) = delete;
    Part(Part&& ) _OPENMA_NOEXCEPT = delete;
    Part& operator=(const Part& ) = delete;
    Part& operator=(Part&& ) _OPENMA_NOEXCEPT = delete;
  };
#endif
  
  enum class RepresentationFrame: char
  {
    Global = 0x01,
    Proximal,
    Distal
  };
  
  /**
   * @enum RepresentationFrame
   * Enumerator to specify in which reference frame some data have to be represented (expressed)
   * @ingroup openma_body
   */
  /**
   * @var RepresentationFrame RepresentationFrame::Global
   * Data have to be repesented in the global frame
   */
  /**
   * @var RepresentationFrame RepresentationFrame::Proximal
   * Data have to be repesented in the proximal frame (e.g. associated with a Joint)
   */
  /**
   * @var RepresentationFrame RepresentationFrame::Distal
   * Data have to be repesented in the distal frame (e.g. associated with a Joint)
   */
    
#ifdef DOXYGEN_SHOULD_SKIP_THIS
  
  /**
   * @enum Side
   * Predefined values to set the location of an element (e.g. Segment, Model).
   * These values can be used for example to adapt computed joint kinematics for a clinical interpretation.
   * @relates Segment
   * @relates Model
   * @ingroup openma_body
   */
  /**
   * @var Side Side::Center
   * Element (e.g. Segment) defined in the center.
   */
  /**
   * @var Side Side::Left
   * Element (e.g. Segment, Model) defined on the left side.
   */
  /**
   * @var Side Side::Right
   * Element (e.g. Segment, Model) defined on the right side.
   */
  /**
   * @var Side Side::Both
   * Element (e.g. Model) covering both sides.
   */
   
  /**
   * @enum Region
   * Predefined values to represent the region of a system
   * These values can be used for example to define a set of segments adapted to a specific region.
   * @relates Model
   * @ingroup openma_body
   */
  /**
   * @var Region Region::Upper
   * Upper region of a body. On the human body, this would correspond to the trunk, the upper limbs and the head.
   */
  /**
   * @var Region Region::Lower
   * Lower region of a body. On the human body, this would correspond to the pelvis and the lower limbs.
   */
  /**
   * @var Region Region::Full
   * Combination of the upper and lower body.
   */
     
  /**
   * @enum Part
   * Predefined values to represent the part of a system
   * These values can be used for example to identify a segment. 
   * @relates Segment
   * @ingroup openma_body
   */
  /**
   * @var Part Part::Head
   * Head segment
   */
  /**
   * @var Part Part::Torso
   * Torso segment
   */
  /**
   * @var Part Part::Clavicle
   * Clavicle segment
   */
  /**
   * @var Part Part::Arm
   * Arm segment
   */
  /**
   * @var Part Part::Forearm
   * Forearm segment
   */
  /**
   * @var Part Part::Hand
   * Hand segment
   */
  /**
   * @var Part Part::Pelvis
   * Pelvis segment
   */
  /**
   * @var Part Part::Thigh
   * Thigh segment
   */
  /**
   * @var Part Part::Shank
   * Shank segment
   */
  /**
   * @var Part Part::Foot
   * Foot segment
   */
  /**
   * @var Part Part::Other
   * Custom segment defined by the developer.
   */
  
#endif
};
};

#endif // __openma_body_enums_h