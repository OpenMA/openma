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

#include "openma/body/dynamicdescriptor.h"
#include "openma/body/descriptor_p.h"
#include "openma/body/enums.h"
#include "openma/body/joint.h"
#include "openma/body/segment.h"
#include "openma/base/logger.h"
#include "openma/math.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  class DynamicDescriptorPrivate : public DescriptorPrivate
  {
    OPENMA_DECLARE_PINT_ACCESSOR(DynamicDescriptor)
      
  public:
    DynamicDescriptorPrivate(DynamicDescriptor* pint, const std::string& name, const std::array<unsigned,9>& order, const std::array<double,9>& scale);
    ~DynamicDescriptorPrivate();
    
    std::array<unsigned,9> Order;
    std::array<double,9> Scale;
    const TimeSequence* RepresentationFrameTimeSequence;
    std::string OutputSuffix;
    const TimeSequence* ForceTimeSequence;
    math::Vector OutputForceData;
    std::string OutputForceUnit;
    const TimeSequence* MomentTimeSequence;
    math::Vector OutputMomentData;
    std::string OutputMomentUnit;
    const TimeSequence* PowerTimeSequence;
    math::Vector OutputPowerData;
    std::string OutputPowerUnit;
  };
  
  DynamicDescriptorPrivate::DynamicDescriptorPrivate(DynamicDescriptor* pint, const std::string& name, const std::array<unsigned,9>& order, const std::array<double,9>& scale)
  : DescriptorPrivate(pint,name),
    Order(order), Scale(scale),
    RepresentationFrameTimeSequence(nullptr),
    OutputSuffix(),
    ForceTimeSequence(nullptr), OutputForceData(), OutputForceUnit(),
    MomentTimeSequence(nullptr), OutputMomentData(), OutputMomentUnit(),
    PowerTimeSequence(nullptr), OutputPowerData(), OutputPowerUnit()
  {};
  
  DynamicDescriptorPrivate::~DynamicDescriptorPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(body::DynamicDescriptor);

namespace ma
{
namespace body
{
  /**
   * @class DynamicDescriptor openma/body/dynamicdescriptor.h
   * @brief Describe Joint dynamics (forces, moments, and powers) in requested reference frame
   *
   * @todo List the available options for this descriptor:
   *  - representationFrame (RepresentationFrame::Global by default)
   *  - representationFrameSuffix (empty by default)
   *  - massNormalization (false by default)
   *  - enableScaleAdaptation (false by default)
   * 
   * @ingroup openma_body
   */
  
  
  /**
   * Create a Dynamic descriptor where the name of @a parent is used as the base name for the outputs.
   * The scale coefficients possibly used in the process() method are all set to 1.0.
   * The order indices possibly used in the process() method are set to keep the original column order (i.e. 0,1,2 for forces, moments, and powers).
   */
  DynamicDescriptor::DynamicDescriptor(Node* parent)
  : DynamicDescriptor({{0,1,2,0,1,2,0,1,2}}, {{1.,1.,1.,1.,1.,1.,1.,1.,1.}}, parent)
  {};
  
  
  /**
   * Create a Dynamic descriptor where @a order and @a scale arrays can adapt data column order and data scaling (e.g. invert the sign of the data to have a specific positive direction)
   * The name of @a parent is used as the base name for the outputs.
   * The content of @a order must be set only with values equals to 0, 1, or 2. These indices correspond to the 0-based column index for each ouput (forces, moments, and powers).
   *
   * @note In case you pass values in @a order that are not in the range [0,2] no error will be triggered until you use the evaluate() method.
   * @note The content of @a order and @a scale is not used if the representation frame is set to RepresentationFrame::Global.
   */
  DynamicDescriptor::DynamicDescriptor(const std::array<unsigned,9>& order, const std::array<double,9>& scale, Node* parent)
  : Descriptor(*new DynamicDescriptorPrivate(this, parent != nullptr ? parent->name() : "", order, scale), parent)
  {};
  
  /**
   * Destructor
   */
  DynamicDescriptor::~DynamicDescriptor() _OPENMA_NOEXCEPT = default;

  /**
   * Create a deep copy of the object and return it as another object.
   */
  DynamicDescriptor* DynamicDescriptor::clone(Node* parent) const
  {
    auto dest = new DynamicDescriptor();
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void DynamicDescriptor::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const DynamicDescriptor*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Descriptor::copy(src);
  };
  
  /**
   */
  bool DynamicDescriptor::prepare(const Node* input, const std::unordered_map<std::string, Any>& options)
  {
    auto optr = this->pimpl();
    const Joint* joint = nullptr;
    if ((joint = node_cast<const Joint*>(input)) != nullptr)
    {
      RepresentationFrame reference = RepresentationFrame::Global;
      std::string referenceSuffix;
      auto it = options.cend();
      if ((it = options.find("representationFrame")) != options.cend())
      {
        reference = it->second.cast<RepresentationFrame>();
      }
      if ((it = options.find("representationFrameSuffix")) != options.cend())
      {
        referenceSuffix = it->second.cast<std::string>();
      }
      Segment* seg = nullptr;
      switch (reference)
      {
      case RepresentationFrame::Global:
        optr->OutputSuffix = ".Global";
        break;
      case RepresentationFrame::Proximal:
        optr->OutputSuffix = ".Local.Proximal";
        seg = joint->proximalSegment();
        if (seg == nullptr)
        {
          error("Proximal segment not found. Impossible to describe the dynamic of the joint %s", joint->name().c_str());
          return false;
        }
        optr->RepresentationFrameTimeSequence = seg->findChild<TimeSequence*>(seg->name() + referenceSuffix, {{"type", TimeSequence::Pose}});
        if (optr->RepresentationFrameTimeSequence == nullptr)
        {
          error("Time sequence representing the pose of the proximal segment was not found. Impossible to describe the dynamic of the joint %s", joint->name().c_str());
          return false;
        }
        break;
      case RepresentationFrame::Distal:
        optr->OutputSuffix = ".Local.Distal";
        seg = joint->distalSegment();
        if (seg == nullptr)
        {
          error("Distal segment not found. Impossible to describe the dynamic of the joint %s", joint->name().c_str());
          return false;
        }
        optr->RepresentationFrameTimeSequence = seg->findChild<TimeSequence*>(seg->name() + referenceSuffix, {{"type", TimeSequence::Pose}});
        if (optr->RepresentationFrameTimeSequence == nullptr)
        {
          error("Time sequence representing the pose of the distal segment was not found. Impossible to describe the dynamic of the joint %s", joint->name().c_str());
          return false;
        }
        break;
      }
      optr->ForceTimeSequence = joint->findChild<const TimeSequence*>({},{{"type",ma::TimeSequence::Force}},false);
      optr->MomentTimeSequence = joint->findChild<const TimeSequence*>({},{{"type",ma::TimeSequence::Moment}},false);
      // optr->PowerTimeSequence = joint->findChild<const TimeSequence*>({},{{"type",ma::TimeSequence::Power}},false);
      if ((optr->ForceTimeSequence == nullptr) || (optr->MomentTimeSequence == nullptr)) // || (optr->PowerTimeSequence == nullptr))
      {
        error("Force or moment data was not found. Impossible to describe the dynamic of the joint %s", joint->name().c_str());
        return false;
      }
      if (((it = options.find("enableScaleAdaptation")) != options.cend()) && (it->second.cast<bool>()))
      {
        for (size_t i = 0 ; i < 9 ; ++i)
        {
          if (optr->Order[i] > 2u)
          {
            error("At least one column index used in data rearrangment is not in the range [0,2]. Impossible to describe the dynamic of the joint %s", joint->name().c_str());
            return false;
          }
        }
      }
    }
    else
    {
      error("The input is not a Joint object. Impossible to describe another kind of object");
      return false;
    }
    return true;
  };
  
  /**
   * Transform orientations into Dynamic angles. By default, the unit used is the radian and no adaptation scale is used.
   * To modify this behaviour, you can use the options:
   *  - massNormalization: double value > 0
   *  - enableScaleAdaptation: boolean value
   */
  bool DynamicDescriptor::process(const std::unordered_map<std::string, Any>& options)
  {
    auto optr = this->pimpl();
    
    std::array<double,9> scale{{1.,1.,1.,1.,1.,1.,1.,1.,1.}}; // unit conversion 
    auto it = options.cend();
    // Option massNormalization activated?
    if ((it = options.find("massNormalization")) != options.cend())
    {
      double massNormalization = it->second.cast<double>();
      if ( massNormalization > 0.)
      {
        scale[0] /= massNormalization;
        scale[1] /= massNormalization;
        scale[2] /= massNormalization;
        scale[3] /= massNormalization;
        scale[4] /= massNormalization;
        scale[5] /= massNormalization;
        scale[6] /= massNormalization;
        scale[7] /= massNormalization;
        scale[8] /= massNormalization;
        optr->OutputForceUnit += "/kg";
        optr->OutputMomentUnit += "/kg";
        optr->OutputPowerUnit += "/kg";
      }
      
    }
    // Option enableScaleAdaptation activated?
    if (((it = options.find("enableScaleAdaptation")) != options.cend()) && (it->second.cast<bool>()))
    {
      scale[0] *= optr->Scale[0];
      scale[1] *= optr->Scale[1];
      scale[2] *= optr->Scale[2];
      scale[3] *= optr->Scale[3];
      scale[4] *= optr->Scale[4];
      scale[5] *= optr->Scale[5];
      scale[6] *= optr->Scale[6];
      scale[7] *= optr->Scale[7];
      scale[8] *= optr->Scale[8];
    }
    // Let's compute the output
    if (optr->RepresentationFrameTimeSequence != nullptr)
    {
      math::Pose P = math::to_pose(optr->RepresentationFrameTimeSequence).inverse();
      math::Vector temp;
      // Force
      temp = P.block<9>(0).transform(ma::math::to_vector(optr->ForceTimeSequence));
      optr->OutputForceData.resize(temp.rows());
      optr->OutputForceData.values().col(0) = temp.values().col(optr->Order[0]);
      optr->OutputForceData.values().col(1) = temp.values().col(optr->Order[1]);
      optr->OutputForceData.values().col(2) = temp.values().col(optr->Order[2]);
      optr->OutputForceData.residuals() = temp.residuals();
      // Moment
      temp = P.block<9>(0).transform(ma::math::to_vector(optr->MomentTimeSequence));
      optr->OutputMomentData.resize(temp.rows());
      optr->OutputMomentData.values().col(0) = temp.values().col(optr->Order[3]);
      optr->OutputMomentData.values().col(1) = temp.values().col(optr->Order[4]);
      optr->OutputMomentData.values().col(2) = temp.values().col(optr->Order[5]);
      optr->OutputMomentData.residuals() = temp.residuals();
      // optr->OutputPowerData = P.block<9>().transform(ma::math::to_vector(optr->PowerTimeSequence));
      // optr->OutputPowerData =  ma::math::to_vector(optr->PowerTimeSequence);
      
    }
    else
    {
      optr->OutputForceData =  ma::math::to_vector(optr->ForceTimeSequence);
      optr->OutputMomentData = ma::math::to_vector(optr->MomentTimeSequence);
      // optr->OutputPowerData =  ma::math::to_vector(optr->PowerTimeSequence);
    }
    optr->OutputForceData.values().col(0) *= scale[0];
    optr->OutputForceData.values().col(1) *= scale[1];
    optr->OutputForceData.values().col(2) *= scale[2];
    optr->OutputMomentData.values().col(0) *= scale[3];
    optr->OutputMomentData.values().col(1) *= scale[4];
    optr->OutputMomentData.values().col(2) *= scale[5];
    // optr->OutputPowerData.block<1>(0) *= scale[6];
    // optr->OutputPowerData.block<1>(1) *= scale[7];
    // optr->OutputPowerData.block<1>(2) *= scale[8];
    return true;
  };
  
  /**
   * Export the dynamic data as TimeSequence. The name of the TimeSequence corresponds to the name of the Descriptor combined with a suffix. No option is available for this method.
   */
  bool DynamicDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    OPENMA_UNUSED(options);
    auto optr = this->pimpl();
    math::to_timesequence(optr->OutputForceData,  this->name()+".Force"+optr->OutputSuffix,  optr->ForceTimeSequence->sampleRate(), optr->ForceTimeSequence->startTime(), TimeSequence::Force,  optr->OutputForceUnit,  output);
    math::to_timesequence(optr->OutputMomentData, this->name()+".Moment"+optr->OutputSuffix, optr->MomentTimeSequence->sampleRate(), optr->MomentTimeSequence->startTime(), TimeSequence::Moment, optr->OutputMomentUnit, output);
    // math::to_timesequence(optr->OutputPowerData,  this->name()+".Power",                     optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Power,  optr->OutputPowerUnit,  output);
    return true;
  };
 };
};