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

#ifndef __openma_base_timesequence_h
#define __openma_base_timesequence_h

#include "openma/base/node.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR, _OPENMA_NOEXCEPT

#include <vector>
#include <array>
#include <numeric>
#include <initializer_list>

namespace ma
{
  class TimeSequencePrivate;
  
  class OPENMA_BASE_EXPORT TimeSequence : public Node
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(TimeSequence)
    OPENMA_DECLARE_NODEID(TimeSequence, Node)
    
  public:
    typedef enum : int {
      Unknown = 0x00,
      Reconstructed = 0x01,
      Marker = 0x02  | Reconstructed,
      Angle  = 0x04  | Reconstructed,
      Force  = 0x08  | Reconstructed,
      Moment = 0x10  | Reconstructed,
      Power  = 0x20  | Reconstructed,
      Scalar = 0x40  | Reconstructed,
      Pose   = 0x80  | Reconstructed,
      Wrench = 0x100 | Reconstructed,
      Velocity = 0x200,
      Analog = 0x1000,
      Other  = 0x10000
    } Type;
#if defined(_MSC_VER) && (_MSC_VER < 1900)
    static _OPENMA_CONSTEXPR std::array<double,2> InfinityRange;
#else
    static _OPENMA_CONSTEXPR std::array<double,2> InfinityRange{{-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()}};
#endif
    
    TimeSequence(const std::string& name, unsigned components, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range, Node* parent = nullptr);
    TimeSequence(const std::string& name, unsigned components, unsigned samples, double rate, double start, int type, const std::string& unit, Node* parent = nullptr);
    TimeSequence(const std::string& name, const std::vector<unsigned>& components, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range, Node* parent = nullptr);
    TimeSequence(const std::string& name, const std::vector<unsigned>& components, unsigned samples, double rate, double start, int type, const std::string& unit, Node* parent = nullptr);
    ~TimeSequence() _OPENMA_NOEXCEPT;
    
    TimeSequence(const TimeSequence& ) = delete;
    TimeSequence(TimeSequence&& ) _OPENMA_NOEXCEPT = delete;
    TimeSequence& operator=(const TimeSequence& ) = delete;
    TimeSequence& operator=(TimeSequence&& ) _OPENMA_NOEXCEPT = delete;
    
    double sampleRate() const _OPENMA_NOEXCEPT;
    void setSampleRate(double value) _OPENMA_NOEXCEPT;
    
    const std::vector<unsigned>& dimensions() const _OPENMA_NOEXCEPT;
    unsigned samples() const _OPENMA_NOEXCEPT;
    unsigned components() const _OPENMA_NOEXCEPT;
    size_t elements() const _OPENMA_NOEXCEPT;
    
    double duration() const _OPENMA_NOEXCEPT;
    
    int type() const _OPENMA_NOEXCEPT;
    void setType(int value) _OPENMA_NOEXCEPT;
    
    const std::string& unit() const _OPENMA_NOEXCEPT;
    void setUnit(const std::string& value) _OPENMA_NOEXCEPT;
    
    double startTime() const _OPENMA_NOEXCEPT;
    void setStartTime(double value) _OPENMA_NOEXCEPT;
    
    double scale() const _OPENMA_NOEXCEPT;
    void setScale(double value) _OPENMA_NOEXCEPT;
    
    double offset() const _OPENMA_NOEXCEPT;
    void setOffset(double value) _OPENMA_NOEXCEPT;
    
    const std::array<double,2>& range() const _OPENMA_NOEXCEPT;
    void setRange(const std::array<double,2>& value) _OPENMA_NOEXCEPT;
    
    const double* data() const _OPENMA_NOEXCEPT;
    double* data() _OPENMA_NOEXCEPT;
    
    template <typename... Is> double data(unsigned sample, Is... indices) const _OPENMA_NOEXCEPT;
    template <typename... Is> double& data(unsigned sample, Is... indices) _OPENMA_NOEXCEPT;
    
    void resize(unsigned samples);
    
    virtual TimeSequence* clone(Node* parent = nullptr) const override;
    virtual void copy(const Node* source) _OPENMA_NOEXCEPT override;
  
  private:
    TimeSequence(const std::string& name, Node* parent = nullptr);
    double& data(unsigned sample, std::initializer_list<unsigned>&& indices) const _OPENMA_NOEXCEPT;
  };
  
  OPENMA_BASE_EXPORT bool compare_timesequences_properties(const std::vector<TimeSequence*>& tss, double& sampleRate, double& startTime, unsigned& samples);
};
  
OPENMA_EXPORT_STATIC_TYPEID(ma::TimeSequence, OPENMA_BASE_EXPORT);

namespace ma
{  
  template <typename... Is>
  inline double TimeSequence::data(unsigned sample, Is... indices) const _OPENMA_NOEXCEPT
  {
    return this->data(sample, {static_cast<unsigned>(indices)...});
  };
  
  template <typename... Is>
  inline double& TimeSequence::data(unsigned sample, Is... indices) _OPENMA_NOEXCEPT
  {
    return this->data(sample, {static_cast<unsigned>(indices)...});
  };
};

#endif // __openma_base_timesequence_h