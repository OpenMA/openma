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

namespace ma
{
  class Any
  {
  public:
    Any();
    %extend {
      Any(const SWIGTYPE* value) {auto ptr = new ma::Any; ma_Any_assign(ptr, value); return ptr;};
    }
    Any(const Any& other);
/*    template <typename U, typename D = void*, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> Any(U&& value, D&& dimensions = D{});
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> Any(std::initializer_list<U> values, std::initializer_list<size_t> dimensions = {});*/
    ~Any();
    
    std::vector<unsigned> dimensions() const;
    size_t size() const;
    
    bool isValid() const;
    bool isEmpty() const;
    void swap(Any& other);
/*
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> bool isEqual(U&& value) const;
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> void assign(U&& value);
*/    
/*    template <typename U, typename> U cast() const;*/
    %extend {
      SWIGTYPE* cast(const char* type = nullptr) const;
      void assign(const SWIGTYPE* value);
    };
    
    /*
    template <typename U, typename = typename std::enable_if<!std::is_same<Any, typename std::decay<U>::type>::value>::type> U cast(size_t idx) const;

    template<class U> operator U() const;

    Any& operator=(const Any& other);

    friend bool operator==(const Any& lhs, const Any& rhs);
    friend bool operator!=(const Any& lhs, const Any& rhs);

    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator==(const A& lhs, const U& rhs) {return lhs.isEqual(rhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator==(const U& lhs, const A& rhs) {return rhs.isEqual(lhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator!=(const A& lhs, const U& rhs) {return !lhs.isEqual(rhs);};
    template <typename U, typename A, typename = typename std::enable_if<std::is_same<Any, typename std::decay<A>::type>::value>::type> friend inline bool operator!=(const U& lhs, const A& rhs) {return !rhs.isEqual(lhs);};
*/
  };
};
