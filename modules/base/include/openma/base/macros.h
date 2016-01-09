/* 
 * Open Source Movement Analysis Library
 * Copyright (C) 2015, Moveck Solution Inc., all rights reserved.
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

/*
 * The header macros.h defines standard system-wide macros, constants, and other
 * parameters.
 */

#ifndef __openma_macros_h
#define __openma_macros_h

/**
 * Macro to use when an input is not used in the function.
 * Usefull when the compiler gives you a warning to say that the variable is not used.
 */
#define OPENMA_UNUSED(x) (void)x;

/**
 * MSVC 2013 does not support the C++11 keyword 'constexpr' but it will supported in MSVC 2015.
 *
 * [1] http://blogs.msdn.com/b/vcblog/archive/2015/04/29/c-11-14-17-features-in-vs-2015-rc.aspx
 */
#if defined(_MSC_VER) && (_MSC_VER < 1900)
  #define _OPENMA_CONSTEXPR const
  #define _OPENMA_CONSTEXPR_CONST const 
#else
  #define _OPENMA_CONSTEXPR constexpr
  #define _OPENMA_CONSTEXPR_CONST constexpr const
#endif

/**
 * MSVC 2013 does not support the C++11 keyword 'noexcept' but it will supported in MSVC 2015.
 *
 * [1] http://blogs.msdn.com/b/vcblog/archive/2015/04/29/c-11-14-17-features-in-vs-2015-rc.aspx
 */
#if defined(_MSC_VER) && (_MSC_VER < 1900)
  #define _OPENMA_NOEXCEPT
#else
  #define _OPENMA_NOEXCEPT noexcept
#endif

/*
 * Internal macro to stringify the given input.
 */
#define __OPENMA_STRINGIFY(x) #x

/**
 * Macro to stringify the given input.
 */
#define OPENMA_STRINGIFY(x) __OPENMA_STRINGIFY(x)

#endif // ____openma_macros_h
