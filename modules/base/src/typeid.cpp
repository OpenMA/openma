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

#include "openma/base/typeid.h"

// Arithmetic types
OPENMA_INSTANCE_STATIC_TYPEID(bool)
OPENMA_INSTANCE_STATIC_TYPEID(char)
// OPENMA_INSTANCE_STATIC_TYPEID(char16_t) // same as unsigned short int
// OPENMA_INSTANCE_STATIC_TYPEID(char32_t) // same as unsigned int
OPENMA_INSTANCE_STATIC_TYPEID(wchar_t)
OPENMA_INSTANCE_STATIC_TYPEID(signed char)
OPENMA_INSTANCE_STATIC_TYPEID(short int)
OPENMA_INSTANCE_STATIC_TYPEID(int)
OPENMA_INSTANCE_STATIC_TYPEID(long int)
OPENMA_INSTANCE_STATIC_TYPEID(long long int)
OPENMA_INSTANCE_STATIC_TYPEID(unsigned char)
OPENMA_INSTANCE_STATIC_TYPEID(unsigned short int)
OPENMA_INSTANCE_STATIC_TYPEID(unsigned int)
OPENMA_INSTANCE_STATIC_TYPEID(unsigned long int)
OPENMA_INSTANCE_STATIC_TYPEID(unsigned long long int)
OPENMA_INSTANCE_STATIC_TYPEID(float)
OPENMA_INSTANCE_STATIC_TYPEID(double)
OPENMA_INSTANCE_STATIC_TYPEID(long double)
// String types
OPENMA_INSTANCE_STATIC_TYPEID(std::string)
OPENMA_INSTANCE_STATIC_TYPEID(const char*)