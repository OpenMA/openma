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

#ifndef __openma_io_c3ddatastream_h
#define __openma_io_c3ddatastream_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#include <type_traits>
#include <vector>

namespace ma
{
namespace io
{
  class BinaryStream;
  
  class C3DDataStream
  {
  public:
    C3DDataStream(BinaryStream* raw);
    virtual ~C3DDataStream();
    
    virtual void readPoint(double* x, double* y, double* z, double* residual, double pointScaleFactor) = 0;
    virtual double readAnalog() = 0;
    virtual void writePoint(double x, double y, double z, double residual, double pointScaleFactor) = 0;
    virtual void writeAnalog(double v) = 0;
    
    C3DDataStream(const C3DDataStream& ) = delete;
    C3DDataStream(C3DDataStream&& ) _OPENMA_NOEXCEPT = delete;
    C3DDataStream& operator=(const C3DDataStream& ) = delete;
    C3DDataStream& operator=(const C3DDataStream&& ) _OPENMA_NOEXCEPT = delete;
    
  protected:
    BinaryStream* Raw;
  };
  
  // Integer format + signed analog data
  class C3DDataStreamSignedInteger : public C3DDataStream
  {
  public:
    C3DDataStreamSignedInteger(BinaryStream* raw);
    ~C3DDataStreamSignedInteger();
    
    virtual void readPoint(double* x, double* y, double* z, double* residual, double pointScaleFactor) final;
    virtual double readAnalog() override;
    virtual void writePoint(double x, double y, double z, double residual, double pointScaleFactor) final;
    virtual void writeAnalog(double v) override;
  };
  
  // Integer format + unsigned analog data
  class C3DDataStreamUnsignedInteger : public C3DDataStreamSignedInteger
  {
  public:
    C3DDataStreamUnsignedInteger(BinaryStream* raw);
    ~C3DDataStreamUnsignedInteger();
    
    virtual double readAnalog() final;
    virtual void writeAnalog(double v) final;
  };
  
  // Float format + signed/unsigned analog data
  class C3DDataStreamFloat : public C3DDataStream
  {
  public:
    C3DDataStreamFloat(BinaryStream* raw);
    ~C3DDataStreamFloat();
    
    virtual void readPoint(double* x, double* y, double* z, double* residual, double pointScaleFactor) final;
    virtual double readAnalog() final;
    virtual void writePoint(double x, double y, double z, double residual, double pointScaleFactor) final;
    virtual void writeAnalog(double v) final;
  };
};
};

#endif // __openma_io_c3ddatastream_h
