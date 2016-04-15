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

#include "c3ddatastream.h"
#include "openma/config.h" // _OPENMA_ARCH, _OPENMA_IEEE_BE
#include "openma/io/binarystream.h"

#include <cmath> // fabs
#include <cstring> // memcpy

namespace ma
{
namespace io
{
  // ------------------------------------------------------------------------ //
  
  C3DDataStream::C3DDataStream(BinaryStream* raw)
  : Raw(raw)
  {};
  
  C3DDataStream::~C3DDataStream() = default;
  
  // ------------------------------------------------------------------------ //
  
  C3DDataStreamSignedInteger::C3DDataStreamSignedInteger(BinaryStream* raw)
  : C3DDataStream(raw)
  {};
  
  C3DDataStreamSignedInteger::~C3DDataStreamSignedInteger() = default;
  
  void C3DDataStreamSignedInteger::readPoint(double* x, double* y, double* z, double* residual, double pointScaleFactor)
  {
    int8_t byteptr[2];
    int16_t residualAndMask;
    *x = this->Raw->readI16() * pointScaleFactor;
    *y = this->Raw->readI16() * pointScaleFactor;
    *z = this->Raw->readI16() * pointScaleFactor;
    residualAndMask = this->Raw->readI16();
    memcpy(&byteptr, &residualAndMask, sizeof(byteptr));
#if _OPENMA_ARCH == _OPENMA_IEEE_BE
    *residual = (byteptr[0] >= 0) ? static_cast<double>(byteptr[1]) * pointScaleFactor : -1;
#else
    *residual = (byteptr[1] >= 0) ? static_cast<double>(byteptr[0]) * pointScaleFactor : -1;
#endif
  };
  
  double C3DDataStreamSignedInteger::readAnalog()
  {
    return static_cast<double>(this->Raw->readI16());
  };
  
  void C3DDataStreamSignedInteger::writePoint(double x, double y, double z, double residual, double pointScaleFactor)
  {
    int8_t byteptr[2];
    int16_t residualAndMask; 
#if defined(_MSC_VER)
    this->Raw->writeI16(static_cast<int16_t>(floor(x / pointScaleFactor + 0.5)));
    this->Raw->writeI16(static_cast<int16_t>(floor(y / pointScaleFactor + 0.5)));
    this->Raw->writeI16(static_cast<int16_t>(floor(z / pointScaleFactor + 0.5)));
#else
    this->Raw->writeI16(static_cast<int16_t>(static_cast<float>(x / pointScaleFactor)));
    this->Raw->writeI16(static_cast<int16_t>(static_cast<float>(y / pointScaleFactor)));
    this->Raw->writeI16(static_cast<int16_t>(static_cast<float>(z / pointScaleFactor)));
#endif
#if _OPENMA_ARCH == _OPENMA_IEEE_BE
    if (residual >= 0.0) {byteptr[0] = 0; byteptr[1] = static_cast<int8_t>(residual / pointScaleFactor);}
    else {byteptr[0] = -1; byteptr[1] = -1;}
#else
    if (residual >= 0.0) {byteptr[1] = 0; byteptr[0] = static_cast<int8_t>(residual / pointScaleFactor);}
    else {byteptr[1] = -1; byteptr[0] = -1;}
#endif
    memcpy(&residualAndMask, &byteptr, sizeof(residualAndMask));
    this->Raw->writeI16(residualAndMask);
  };
  
  void C3DDataStreamSignedInteger::writeAnalog(double v)
  {
    this->Raw->writeI16(static_cast<int16_t>(v));
  };
  
  // ------------------------------------------------------------------------ //

  C3DDataStreamUnsignedInteger::C3DDataStreamUnsignedInteger(BinaryStream* raw)
  : C3DDataStreamSignedInteger(raw)
  {};
  
  C3DDataStreamUnsignedInteger::~C3DDataStreamUnsignedInteger() = default;
  
  double C3DDataStreamUnsignedInteger::readAnalog()
  {
    return static_cast<double>(this->Raw->readU16());
  };
  void C3DDataStreamUnsignedInteger::writeAnalog(double v)
  {
    this->Raw->writeI16(static_cast<uint16_t>(v));
  };

  // ------------------------------------------------------------------------ //

  C3DDataStreamFloat::C3DDataStreamFloat(BinaryStream* raw)
  : C3DDataStream(raw)
  {};
  
  C3DDataStreamFloat::~C3DDataStreamFloat() = default;
  
  void C3DDataStreamFloat::readPoint(double* x, double* y, double* z, double* residual, double pointScaleFactor)
  {
    int8_t byteptr[2];
    int16_t residualAndMask;
    *x = this->Raw->readFloat();
    *y = this->Raw->readFloat();
    *z = this->Raw->readFloat();
    residualAndMask = static_cast<int16_t>(this->Raw->readFloat());
    memcpy(&byteptr, &residualAndMask, sizeof(byteptr));
    // FIX: It seems that for UNSGINED 16 bits in float format, the residual is negative.
    //      The residual is now calculated as the fabs(byteptr[1] * scaleFactor3d).
#if _OPENMA_ARCH == _OPENMA_IEEE_BE
    *residual = (byteptr[0] >= 0) ? fabs(static_cast<double>(byteptr[1]) * pointScaleFactor) : -1.0;
#else
    *residual = (byteptr[1] >= 0) ? fabs(static_cast<double>(byteptr[0]) * pointScaleFactor) : -1.0;
#endif
  };
    
  double C3DDataStreamFloat::readAnalog()
  {
    return this->Raw->readFloat();
  };
    
  void C3DDataStreamFloat::writePoint(double x, double y, double z, double residual, double pointScaleFactor)
  {
    int8_t byteptr[2];
    int16_t residualAndMask;
    this->Raw->writeFloat(static_cast<float>(x));
    this->Raw->writeFloat(static_cast<float>(y));
    this->Raw->writeFloat(static_cast<float>(z));
#if _OPENMA_ARCH == _OPENMA_IEEE_BE
    if (residual >= 0.0) {byteptr[0] = 0; byteptr[1] = static_cast<int8_t>(residual / pointScaleFactor);}
    else {byteptr[0] = -1; byteptr[1] = -1;}
#else
    if (residual >= 0.0) {byteptr[1] = 0; byteptr[0] = static_cast<int8_t>(residual / pointScaleFactor);}
    else {byteptr[1] = -1; byteptr[0] = -1;}
#endif
    memcpy(&residualAndMask, &byteptr, sizeof(residualAndMask));
    this->Raw->writeFloat(static_cast<float>(residualAndMask));
  };
  
  void C3DDataStreamFloat::writeAnalog(double v)
  {
    this->Raw->writeFloat(static_cast<float>(v));
  };
};
};