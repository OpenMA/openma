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

#include "openma/base/timesequence.h"
#include "openma/base/timesequence_p.h"

#include <cassert>
#include <algorithm> // std::copy_n

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
  TimeSequencePrivate::TimeSequencePrivate(TimeSequence* pint, const std::string& name)
  : NodePrivate(pint,name),
    Dimensions(), AccumulatedDimensions(), Samples(0), SampleRate(0.0), StartTime(0.0), Type(0), Unit(), Scale(1.0), Offset(0.0), Range(), Data(nullptr)
  {};
  
  TimeSequencePrivate::TimeSequencePrivate(TimeSequence* pint, const std::string& name, const std::vector<unsigned>& dimensions, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range)
  : NodePrivate(pint,name),
    Dimensions(dimensions), AccumulatedDimensions(), Samples(samples), SampleRate(rate), StartTime(start), Type(type), Unit(unit), Scale(scale), Offset(offset), Range(range), Data(nullptr)
  {
    assert(!dimensions.empty());
    // Allocate data memory;
    if (samples != 0)
    {
      size_t num = 1;
      for(const unsigned& cpt: dimensions)
        num *= cpt;
      assert(num != 0);
      this->Data = new double[samples * num];
    }
    // Compute accumulated dimensions (used for the method data(sample, indices))
    this->AccumulatedDimensions.resize(dimensions.size()-1,dimensions[0]);
    for (int i = static_cast<int>(this->AccumulatedDimensions.size()-1) ; i > 0  ; --i)
      this->AccumulatedDimensions[i-1] = this->AccumulatedDimensions[i] * this->Dimensions[this->AccumulatedDimensions.size()-i];
  };
  
  TimeSequencePrivate::~TimeSequencePrivate() _OPENMA_NOEXCEPT
  {
    delete[] this->Data;
  };
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::TimeSequence);

namespace ma
{
  /**
   * @class TimeSequence openma/base/timesequence.h
   * @brief Multidimensional time series container
   *
   * There is lots of way to store time series data (i.e. data recorded at a fixed sample rate). In OpenMA, recorded data can be markers' trajectories, analog signals, pressure insole, orientation, pose, etc. For example, for each sample, a trajectory has 3 coordinates and one residual, while for an analog channel, a sample is equal to a single value.
   * Instead of distinguish each kind of data series by several subclasses (and add specific members and methods each time), it was decided to create a multidimensionnal time series container. Combined with static and dynamic propertie (see Property), this would give the possibility to store any kind of time series (or should be) wit any kind of sample (in terms of dimensions and components).
   *
   * To go futher, reconstructed data (e.g. acquired by a sterephotogrammetric system) and raw data (e.g. acquired by a digital-to-analog converter: ADC) are stored in the same way. Howver, the former has (at least) a supplementary component to store associated residual. This is the responsability of the developer to add this/these extra components when the TimeSequence object is created. To possibliy know if a TimeSequence object has residual compoents, the type() of the object can be tested against the enum value Reconstructed (see the example below). Of course, this methods works only if the given Type use the flag Reconstructed.
   * @code{.unparsed}
   * // The following code creates a time sequence:
   * // - named LHEE
   * // - where each sample has 1 dimension with 4 components (X, Y, Z, residual)
   * // - with 1000 samples
   * // - sampled at 100.0Hz
   * // - starting at 0.0 second of the beginning of the acqusition
   * // - typed as a Marker
   * // - where the main unit (i.e. for the coordinates) is the millimeter.
   * auto marker = ma::TimeSequence("LHEE",4,1000,100.0,0.0,ma::TimeSequence::Marker,"mm");
   * // Check if the object stores reconstructed data
   * std::cout << marker.type() & ma::TimeSequence::Reconstructed << std::endl;
   * @endcode
   * 
   * For ADC data, some members were added to give supplementary information that could be used useful when exporting the TimeSequence into a file.
   * You can pass to the constructor 3 ADC parameters: @a scale, @a offset, and @a range. By default, the ADC resolution is not stored as it is assumed that all TimeSequence objects use the same converter. However, you could add a dynamic property (e.g. @a resolution) using setProperty() to store this information.
   * In general the file formats proposed in OpenMAto to read/write ADC data use only the three stored ADC parameter (scale, offset, range). You should read their dedicated documentation for more details.
   * @code{.unparsed}
   * // The last three arguments are for:
   * //  - The ADC scale: 1.0
   * //  - The ADC offset: 0.0
   * //  - The ADC range: [-10,10]
   * auto analog = ma::TimeSequence("RRF",1,10000,1000.0,0.0,ma::TimeSequence::Analog,"V",1.0,0.0,{-10.0,10.0});
   * // Store supplementary information
   * analog.setProperty("resolution",16);
   * // Extract it
   * std::cout << analog.property("resolution").cast<int>()
   * @endcode
   *
   * @ingroup openma_base
   */
  
  /**
   * @var TimeSequence::Type
   * Contextual information about the kind of data stored in the time sequence.
   */  
  /**
   * @var TimeSequence::Type TimeSequence::Unknown
   * Unknown type. This is the default type if none is given to a constructor
   */
  /**
   * @var TimeSequence::Type TimeSequence::Reconstructed
   * Internal flag to indicated if these data are reconstructed data. The usage of this flag shall mean that some components are resevred for reconstruction residuals. For example if a TimeSequence is set to the Type Marker (predefined value included the Reconstructed flag), the given dimension should be set to 4. Three of them are for the coordinates and the later is for the reconstruction residuals.
   */
  /**
   * @var TimeSequence::Type TimeSequence::Marker
   * Should be used to represent a reconstructed/computed 3D trajectory. Data sample shall be represented by an 1D array with 4 components: X, Y, Z coordinates and valid/reconstruction residuals. The associated unit shall be millimeter ("mm").
   */
  /**
   * @var TimeSequence::Type TimeSequence::Angle
   * Should be used to represent a reconstructed/computed 3D angle. Data sample shall be represented by an 1D signal with 4 components: X, Y, Z values and valid/reconstruction residuals. The associated unit shall be radian ("rad").
   */
  /**
   * @var TimeSequence::Type TimeSequence::Force
   * Should be used to represent a reconstructed/computed 3D force. Data sample shall be represented by an 1D signal with 4 components: X, Y, Z values and valid/reconstruction residuals. The associated unit shall be newton ("N").
   */
  /**
   * @var TimeSequence::Type TimeSequence::Moment
   * Should be used to represent a reconstructed/computed 3D moment. Data sample shall be represented by an 1D signal with 4 components: X, Y, Z values and valid/reconstruction residuals. The associated unit shall be newton-millimeter ("Nmm").
   */
  /**
   * @var TimeSequence::Type TimeSequence::Power
   * Should be used to represent a reconstructed/computed 1D-2D-3D power. Data sample shall be represented by an 1D signal with 2-3-4 components: X, Y, Z values and valid/reconstruction residuals. The associated unit shall be watt ("W").
   */
  /**
   * @var TimeSequence::Type TimeSequence::Scalar
   * Should be used to represent a reconstructed/computed 1D-2D-3D values. Data sample shall be represented by an 1D signal with 2-3-4 components: X, Y, Z values and valid/reconstruction residuals.
   */
  /**
   * @var TimeSequence::Type TimeSequence::Analog
   * Should be used to represent a measured 1D signal. Data sample shall be represented by an 1D signal with 1 component. The associated unit shall be volt ("V") or any other acquired unit.
   */
  /**
   * @var TimeSequence::Type TimeSequence::Pose
   * Should be used to represent a reconstructed/computed 3D pose (3D orientation and 3D position). Data sample shall be represented by an 1D signal with 13 components: R11, R21, R31, R12, R22, R32, R31, R23, R33, pX, pY, pZ values and valid/reconstruction residuals. No unit (empty string) shall bet set.
   */
  /**
   * @var TimeSequence::Type TimeSequence::Other
   * To be used to extend predefined type. This could be usefull to extract all the node with a specific type.
   */
  
#ifdef DOXYGEN_SHOULD_TAKE_THIS
  /** * @brief Fake structure to create node's properties */
  struct TimeSequence::__Doxygen_Properties
  {
  /**
   * This property holds the number of samples of a Timesequnce. By default, this property is set to 0.
   * @sa samples() setSamples()
   */
  unsigned samples;
  /**
   * This property holds the sample rate of a Timesequnce. By default, this property is set to 0.
   * @sa sampleRate() setSampleRate()
   */
  double sampleRate;
  /**
   * This property holds the start time of a Timesequnce. By default, this property is set to 0.0.
   * @sa startTime() setStartTime()
   */
  double startTime;
  /**
   * This property holds the type of a Timesequnce. By default, this property is set to TimeSequence::Unknown.
   * @sa type() setType()
   */
  int type;
  /**
   * This property holds the unit of a Timesequnce. By default, this property contains an empty string.
   * @note The modification of this property does not influence the data. This is only for information purpose
   * @sa unit() setUnit()
   */
  std::string unit;
  /**
   * This property holds the scale of a Timesequnce. By default, this property is set to 1.0.
   * @note The modification of this property does not influence the data. This is only for information purpose
   * @sa scale() setScale()
   */
  double scale;
  /**
   * This property holds the offset of a Timesequnce. By default, this property is set to 0.
   * @note The modification of this property does not influence the data. This is only for information purpose
   * @sa offset() setOffset()
   */
  double offset;
  /**
   * This property holds the range of a Timesequnce. By default, this property is set to [-infinity, +infinity].
   * @note The modification of this property does not influence the data. This is only for information purpose
   * @sa range() setRange()
   */
  std::array<double,2> range;
  };
#endif
  
  /**
   * @var TimeSequence::InfinityRange
   * Constant array to represent an infinity range: [-infinity, +infinity].
   */
#if defined(_MSC_VER) && (_MSC_VER < 1900)
  _OPENMA_CONSTEXPR std::array<double,2> TimeSequence::InfinityRange{{-std::numeric_limits<double>::infinity(),std::numeric_limits<double>::infinity()}};
#else  
  _OPENMA_CONSTEXPR std::array<double,2> TimeSequence::InfinityRange;
#endif
  
  /**
   *  Complete constructor for time series signal with 1D data sample. This constructor can be usefull for Analog signal recorded by a digital-to-analog converter (DAC).
   * @note The given @a type should be based on the enum Type.
   */
  TimeSequence::TimeSequence(const std::string& name, unsigned components, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range, Node* parent)
  : TimeSequence(name,std::vector<unsigned>{components},samples,rate,start,type,unit,scale,offset,range,parent)
  {};

  /**
   *  Simplified constructor for time series signal with 1D data sample. Lots of predefined type (Marker, Angle, Force, Moment, etc.) should use this constructor.
   * @note The given @a type should be based on the enum Type.
   */
  TimeSequence::TimeSequence(const std::string& name, unsigned components, unsigned samples, double rate, double start, int type, const std::string& unit, Node* parent)
  : TimeSequence(name,components,samples,rate,start,type,unit,1.0,0.0,InfinityRange,parent)
  {};

  /**
   * Complete constructor for time series signal with xD data sample. The number of data sample dimensions is determined by the number of @a components given. This constructor can be usefull for pressure matrix or any measuring system with 2D (or more) data sample dimension.
   * @note The given @a type should be based on the enum Type.
   */
  TimeSequence::TimeSequence(const std::string& name, const std::vector<unsigned>& components, unsigned samples, double rate, double start, int type, const std::string& unit, double scale, double offset, const std::array<double,2>& range, Node* parent)
  : Node(*new TimeSequencePrivate(this,name,components,samples,rate,start,type,unit,scale,offset,range),parent)
  {};
  
  /**
   * Simplified constructor for time series signal with xD data sample. The number of data sample dimensions is determined by the number of @a components given. This constructor can be usefull for pressure matrix or any measuring system with 2D (or more) data sample dimension.
   * @note The given @a type should be based on the enum Type.
   */
  TimeSequence::TimeSequence(const std::string& name, const std::vector<unsigned>& components, unsigned samples, double rate, double start, int type, const std::string& unit, Node* parent)
  : TimeSequence(name,components,samples,rate,start,type,unit,1.0,0.0,InfinityRange,parent)
  {};
  
  TimeSequence::TimeSequence(const std::string& name, Node* parent)
  : Node(*new TimeSequencePrivate(this,name), parent)
  {};
  
  /*
   * @fn TimeSequence::~TimeSequence()
   * Destructor (default)
   */
  TimeSequence::~TimeSequence() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns the sample rate
   */
  double TimeSequence::sampleRate() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->SampleRate;
  };

  /**
   * Sets the sample rate.
   * This method triggers the modified() method if the given value is different of the stored one.
   */ 
  void TimeSequence::setSampleRate(double value)_OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->SampleRate)
      return;
    optr->SampleRate = value;
    this->modified();
  };

  /**
   * Returns the data sample dimensions.
   */
  const std::vector<unsigned>& TimeSequence::dimensions() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Dimensions;
  };
  
  /**
   * Returns the number of data samples stored in the time sequence.
   */
  unsigned TimeSequence::samples() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Samples;
  };

  /**
   * Returns the total number of components for one data sample.
   * This method multiplies each dimensions together to compute the total number of component.
   * It can be usefull to convert a xD signal into a 1D signal.
   */
  unsigned TimeSequence::components() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return std::accumulate(optr->Dimensions.cbegin(), optr->Dimensions.cend(), 1, [](unsigned x, unsigned y)->unsigned {return x*y;});
  };

  /**
   * Returns the total number of elements. This corresponds to computation of the number of samples by the total number of components.
   */
  size_t TimeSequence::elements() const _OPENMA_NOEXCEPT
  {
    return static_cast<size_t>(this->samples()) * static_cast<size_t>(this->components());
  };
  
  /**
   * Returns the duration of the time sequence. This corresponds to the division of the number of samples by the sample rate.
   */
  double TimeSequence::duration() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->SampleRate <= 0.0)
      return 0.0;
    return static_cast<double>(optr->Samples) / optr->SampleRate;
  };

  /**
   * Returns the type assocaited with the time sequence.
   */
  int TimeSequence::type() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Type;
  };
  
  /**
   * Sets the type associated with this time sequence.
   * This method triggers the modified() method if the given value is different of the stored one.
   */
  void TimeSequence::setType(int value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (optr->Type == value)
      return;
    optr->Type = value;
    this->modified();
  };
  
  /**
   * Returns the unit associated with this time sequence.
   */
  const std::string& TimeSequence::unit() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Unit;
  };
  
  /**
   * Sets the unit associated with this time sequence.
   * This method triggers the modified() method if the given value is different of the stored one.
   */
  void TimeSequence::setUnit(const std::string& value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->Unit)
      return;
    optr->Unit = value;
    this->modified();
  };

 /**
  * Returns the starting time associated with this time sequence.
  */
  double TimeSequence::startTime() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->StartTime;
  };
  
 /**
  * Sets the starting time associated with this time sequence.
  * This method triggers the modified() method if the given value is different of the stored one.
  */
  void TimeSequence::setStartTime(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->StartTime)
      return;
    optr->StartTime = value;
    this->modified();
  };

 /**
  * Returns the scaling factor used to record the signal and possibly transform it from raw data to stored measurement
  */
  double TimeSequence::scale() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Scale;
  };
  
  /**
   * Sets the scaling factor that was possibly used to transform raw data to stored measurement
   */
  void TimeSequence::setScale(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->Scale)
      return;
    optr->Scale = value;
    this->modified();
  };
  
  /**
   * Returns the offset value that was possibly used to transform raw data to stored measurement
   */
  double TimeSequence::offset() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Offset;
  };
  
  /**
   * Sets the offset value that was possibly used to transform raw data to stored measurement
   */
  void TimeSequence::setOffset(double value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->Offset)
      return;
    optr->Offset = value;
    this->modified();
  };
  
  /**
   * Returns the range of measurement of a possible digital-to-analog converter used to record stored data. For example [-10V, 10V].
   */
  const std::array<double,2>& TimeSequence::range() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Range;
  };
  
  /**
   * Sets the range of measurement of a possible digital-to-analog converter used to record stored data. For example [-10V, 10V].
   */
  void TimeSequence::setRange(const std::array<double,2>& value) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    if (value == optr->Range)
      return;
    optr->Range = value;
    this->modified();
  };
  
  /**
   * Return the pointer storing the internal data
   */
  const double* TimeSequence::data() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
  /**
   * Return the pointer storing the internal data. These data are stored by column.
   * @warning You should used this method very carefully. It is recommended to call the method modified() manually if you apply modifications on the data.
   */
  double* TimeSequence::data() _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Data;
  };
  
 
  /**
   * @fn template <typename... Is> double TimeSequence::data(unsigned sample, Is... indices) const _OPENMA_NOEXCEPT
   * Extract a read-only element of the time sequence for the given @a sample index and dimensions @a indices. In case the the number of @a indices is not consistent with the number of dimension, ths missing ones are set to 0.
   */
  
  /**
   * @fn template <typename... Is> double& TimeSequence::data(unsigned sample, Is... indices) _OPENMA_NOEXCEPT
   * Extract a reference to a read-only element of the time sequence for the given @a sample index and dimensions @a indices. In case the the number of @a indices is not consistent with the number of dimension, ths missing ones are set to 0. If the data are modified by this method, it is adviced to call modified() manually
   */
  
  /**
   * Resize the data to fit the number of @a samples.
   * Internally a new buffer is created and previous data are copied. Afterwards, the method deletes the previous data.
   */
  void TimeSequence::resize(unsigned samples)
  {
    auto optr = this->pimpl();
    if (optr->Samples == samples)
      return;
    double* oldData = optr->Data;
    unsigned num = this->components();
    assert(num != 0);
    optr->Data = new double[num * samples];
    unsigned s = std::min(optr->Samples,samples);
    for (unsigned i = 0 ; i < num ; ++i)
      std::copy_n(oldData + i*optr->Samples, s, optr->Data + i*samples);
    optr->Samples = samples;
    delete oldData;
    this->modified();
  };

  /**
   * Internal method to extract an element based on the given @a sample index and dimensions @a indices
   */
  double& TimeSequence::data(unsigned sample, std::initializer_list<unsigned>&& indices) const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    assert(sample < optr->Samples);
    assert(indices.size() <= optr->Dimensions.size());
    auto it = indices.begin();
    std::advance(it,indices.size()-1);
    size_t col = (indices.size() == 0) ? 0 : *it;
    it = indices.begin();
    for (size_t i = 0, len = std::min(indices.size(),optr->AccumulatedDimensions.size()) ; i < len ; ++i)
    {
      col += optr->AccumulatedDimensions[i] * *it;
      ++it;
    }
    return optr->Data[col*optr->Samples+sample];
  };
  
  /**
   * Create a new Event object on the heap
   */
  Node* TimeSequence::allocateNew() const
  {
    return new TimeSequence(this->name());
  };
  
  /**
   * Copy the content of the @a source
   */
  void TimeSequence::copyContents(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const TimeSequence*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copyContents(src);
    optr->Dimensions = optr_src->Dimensions;
    optr->AccumulatedDimensions = optr_src->AccumulatedDimensions;
    optr->Samples = optr_src->Samples;
    optr->SampleRate = optr_src->SampleRate;
    optr->StartTime = optr_src->StartTime;
    optr->Type = optr_src->Type;
    optr->Unit = optr_src->Unit;
    optr->Scale = optr_src->Scale;
    optr->Offset = optr_src->Offset;
    optr->Range = optr_src->Range;
    delete optr->Data;
    size_t numelts = src->elements();
    optr->Data = new double[numelts];
    std::copy_n(optr_src->Data, numelts, optr->Data);
  };
  
  // ----------------------------------------------------------------------- //
  
  /**
   * Compare different properties for each TimeSequence passed in @a timeSequences.
   * If for each property the values are the same, this function returns true, otherwise false.
   * The properties compared are:
   *  - the sample rate ;
   *  - the start time ;
   *  - the number of samples.
   * The common values are returned by the inout arguments @a sampleRate, @a startTime, @a samples.
   * @relates TimeSequence
   * @ingroup openma_base
   */
  bool compare_timesequences_properties(const std::vector<TimeSequence*>& timeSequences, double& sampleRate, double& startTime, unsigned& samples)
  {
    bool first = true;
    for (const auto& ts: timeSequences)
    {
      if (first)
      {
        sampleRate = ts->sampleRate();
        startTime = ts->startTime();
        samples = ts->samples();
        first = false;
      }
      if ((fabs(ts->sampleRate() - sampleRate) > std::numeric_limits<float>::epsilon())
       || (fabs(ts->startTime() - startTime) > std::numeric_limits<float>::epsilon())
       || (ts->samples() != samples))
        return false;
    }
    return true;
  };
};