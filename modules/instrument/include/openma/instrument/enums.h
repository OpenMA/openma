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

#ifndef __openma_instrument_enums_h
#define __openma_instrument_enums_h

namespace ma
{
namespace instrument
{
  enum class Location : char {Origin = 0x01, SurfaceOrigin, CentreOfPressure, PointOfApplication};
  
  /**
   * @enum Location
   * Predefined values to refer to physical locations
   * This is used for example to determine where force plate wrench (force, moment, position) has to be computed.
   * @ingroup openma_instrument
   */
  /**
   * @var Location Location::Origin
   * Origin of the device.
   */
  /**
   * @var Location Location::SurfaceOrigin
   * Fixed position located at the surface of the device. Compared to Origin, this location takes into account of a possible distance between the true origin of the device and the surface where information is measured (e.g. Force plate origin vs Laboratory floor).
   */
  /**
   * @var Location Location::CentreOfPressure
   * A varying position computed from vertical (normal) component of the measured force and horizontal moments.
   */
  /**
   * @var Location Location::PointOfApplication
   * Compared to the CenterOfPressure, the computation of this position does no assumption on the horizontal forces.
   * This would give a more accurate position of the exact point of application.
   * For more information, you can read the following reference:
   * Shimba T.@n
   * <em>An estimation of center of gravity from force platform data</em>.@n
   * Journal of Biomechanics, <b>1984</b>, 17(1), 53–60.
   */
};
};

#endif // __openma_body_enums_h