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

#include "openma/body/eulerdescriptor.h"
#include "openma/body/eulerdescriptor_p.h"
#include "openma/body/joint.h"
#include "openma/body/segment.h"
#include "openma/base/logger.h"

#include <cmath> // M_PI

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  EulerDescriptorPrivate::EulerDescriptorPrivate(EulerDescriptor* pint, const std::string& name, const std::array<int,3>& sequence, const std::array<double,3>& scale, const std::array<double,3>& offset)
  : DescriptorPrivate(pint,name), Sequence(sequence), Scale(scale), Offset(offset), BufferData(), OutputData(), OutputSampleRate(0.0), OutputStartTime(0.0), OutputUnit()
  {};
  
  EulerDescriptorPrivate::~EulerDescriptorPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(body::EulerDescriptor);

namespace ma
{
namespace body
{
  /**
   * @class EulerDescriptor openma/body/eulerdescriptor.h
   * @brief Describe a Joint or Segment as Euler Angle.
   *
   * @todo List the available options for this descriptor:
   *  - suffixProximal
   *  - suffixDistal
   *  - enableDegreeConversion
   *  - adaptForInterpretation
   * 
   * @ingroup openma_body
   */
  
#if defined(_MSC_VER) && (_MSC_VER < 1900)
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::XYZ{{0,1,2}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::YZX{{1,2,0}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::ZXY{{2,0,1}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::XZY{{0,2,1}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::ZYX{{2,1,0}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::YXZ{{1,0,2}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::ZXZ{{2,0,2}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::XYX{{0,1,0}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::YZY{{1,2,1}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::ZYZ{{2,1,2}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::XZX{{0,2,0}};
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::YXY{{1,0,1}};
#else
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::XYZ;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::YZX;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::ZXY;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::XZY;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::ZYX;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::YXZ;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::ZXZ;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::XYX;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::YZY;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::ZYZ;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::XZX;
  _OPENMA_CONSTEXPR std::array<int,3> EulerDescriptor::YXY;
#endif
  
  /**
   * @var EulerDescriptor::XYZ
   * Predefined array with 0-based indices to represent the Euler sequence X-Y-Z
   */
  /**
   * @var EulerDescriptor::YZX
   * Predefined array with 0-based indices to represent the Euler sequence Y-Z-X
   */
  /**
   * @var EulerDescriptor::ZXY
   * Predefined array with 0-based indices to represent the Euler sequence Z-X-Y
   */
  /**
   * @var EulerDescriptor::XZY
   * Predefined array with 0-based indices to represent the Euler sequence X-Z-Y
   */
  /**
   * @var EulerDescriptor::ZYX
   * Predefined array with 0-based indices to represent the Euler sequence Z-Y-X
   */
  /**
   * @var EulerDescriptor::YXZ
   * Predefined array with 0-based indices to represent the Euler sequence Y-X-Z
   */
  /**
   * @var EulerDescriptor::ZXZ
   * Predefined array with 0-based indices to represent the Euler sequence Z-X-Z
   */
  /**
   * @var EulerDescriptor::XYX
   * Predefined array with 0-based indices to represent the Euler sequence X-Y-X
   */
  /**
   * @var EulerDescriptor::YZY
   * Predefined array with 0-based indices to represent the Euler sequence Y-Z-Y
   */
  /**
   * @var EulerDescriptor::ZYZ
   * Predefined array with 0-based indices to represent the Euler sequence Z-Y-Z
   */
  /**
   * @var EulerDescriptor::XZX
   * Predefined array with 0-based indices to represent the Euler sequence X-Z-X
   */
  /**
   * @var EulerDescriptor::YXY
   * Predefined array with 0-based indices to represent the Euler sequence Y-X-Y
   */
  
  /**
   * Create an Euler descriptor with the given @a name and @a sequence. The scale coefficients possibly used in the process() method are set to 1.0.
   * You can use the predefined arrays (XYZ, etc.) to simply the definition of the sequence.
   * @note In case you pass an sequence not available in the 12 possible sequences, no error will be triggered until you use the evaluate() method.
   */
  EulerDescriptor::EulerDescriptor(const std::string& name, const std::array<int,3>& sequence, Node* parent)
  : EulerDescriptor(name, sequence, {{1.0,1.0,1.0}}, {{0.,0.,0.}}, parent)
  {};
  
  /**
   * Create an Euler descriptor with the given @a name and @a sequence. The scale coefficients possibly used in the process() method are set to @a scale. The offsets are set their default value: 0.
   * You can use the predefined arrays (XYZ, etc.) to simply the definition of the sequence.
   * @note In case you pass an sequence not available in the 12 possible sequences, no error will be triggered until you use the evaluate() method.
   */
  EulerDescriptor::EulerDescriptor(const std::string& name, const std::array<int,3>& sequence, const std::array<double,3>& scale, Node* parent)
  : EulerDescriptor(name, sequence, scale, {{0.,0.,0.}}, parent)
  {};
  
  /**
   * Create an Euler descriptor with the given @a name and @a sequence. The scale and offset coefficients possibly used in the process() method are set to @a scale and @a offset respectively.
   * You can use the predefined arrays (XYZ, etc.) to simply the definition of the sequence.
   * @note In case you pass an sequence not available in the 12 possible sequences, no error will be triggered until you use the evaluate() method.
   */
  EulerDescriptor::EulerDescriptor(const std::string& name, const std::array<int,3>& sequence, const std::array<double,3>& scale, const std::array<double,3>& offset, Node* parent)
  : Descriptor(*new EulerDescriptorPrivate(this,name,sequence,scale,offset),parent)
  {};
  
  /**
   * Destructor
   */
  EulerDescriptor::~EulerDescriptor() _OPENMA_NOEXCEPT = default;

  /**
   * Create a deep copy of the object and return it as another object.
   */
  EulerDescriptor* EulerDescriptor::clone(Node* parent) const
  {
    auto dest = new EulerDescriptor(this->name(), {{-1,-1,-1}});
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void EulerDescriptor::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const EulerDescriptor*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Descriptor::copy(src);
    optr->Sequence = optr_src->Sequence;
    optr->Scale = optr_src->Scale;
  };
  
  /**
   * Adapt the @a input in function of its type and the @a options passed.
   * The code in this method does the following:
   *  - Check the type of the @a input that must be a Joint or a Segment.
   *  - In case of a Joint
   *    - Get proximal and distal Segment
   *    - For each segment, look for a TimeSequence (type Pose) child node (see below for the rules to choose TimeSequence nodes)
   *    - Compute the transformation pose that express the pose of the distal segment into the proximal segment. If this is a global joint (i.e. proximal segment is null), the content of the distal TimeSequence is directly copied.
   *  - In case of a Segment
   *    - Look for a TimeSequence child node (type Pose) 
   *    - Copy its content
   *
   * @par Rules to choose associated time sequence
   * - Extracted TimeSequence is found in the children of a Segment. The Segment can be directly the input or a child of the input (i.e. in case the input is a Joint)
   * - The name of this child is based on the name of the Segment combined with an optional suffix.
   * - The suffix has to be passed by the @a options.
   *   - Each segment (ie. joint proximal/distal segment or direct segment input) has a specific suffix.
   *     - The option 'suffix' is used when the input is directly a Segment object.
   *     - The option 'suffixProximal' is used for the proximal segment when the input is a Joint object.
   *     - The option 'suffixDistal' is used for the distal segment when the input is a Joint object.
   * For example:
   * @code{.unparsed}
   * // Node used for the angle output;
   * ma::Node output("analysis");
   * // Create segments used by the joint
   * auto thigh = new ma::body::Segment("L.Thigh", ma::body::Part::Thigh, ma::body::Side::Left);
   * auto shank = new ma::body::Segment("L.Shank", ma::body::Part::Shank, ma::body::Side::Left);
   * // Create the joint
   * ma::body::Joint knee("L.Knee", thigh, knee);
   * // Create the descriptr and parent it with the joint. 
   * // The parent has no importance. However, you can set it to the elements to describe. It gives you
   * new ma::body::EulerDescriptor("L.Knee.Angle", ma::body::EulerDescriptor::YXZ, knee);
   * // Function which generate a TimeSequence for each segment. 
   * // The name of each time sequence corresponds to the name of the segment with the suffix ".SCS" (Segment Coordinate System).
   * // In this example, this means L.Thigh.SCS and L.Shank.SCS.
   * create_timesequence_for_segments(thigh, shank);
   * // Retrieve the descriptor (this is usefull if the descriptor was created in another function to set up the model)
   * auto descriptor = knee.findChild<ma::body::EulerDescriptor*>();
   * // Evaluate the description of the given input (knee) and add it to the output. Due to the options pass, the method will used the time sequences L.Thigh.SCS and L.Shank.SCS. The unit of the result will be the degree.
   * descriptor->evaluate(output, knee, {{"suffixProximal",".SCS"},{"suffixDistal",".SCS"},{"enableDegreeConversion",true}});
   * // The next line will display true.
   * std::cout << std::boolalpha << output.findChild<ma::TimeSequence*>("L.Knee.Angle") != nullptr << std::endl;
   * @endcode
   *
   * The available options for this methods are:
   *  - suffix: string (for Segment input only)
   *  - suffixProximal: string (for Joint input only)
   *  - suffixDistal: string (for Joint input only)
   */
  bool EulerDescriptor::prepare(const Node* input, const std::unordered_map<std::string, Any>& options)
  {
    auto optr = this->pimpl();
    if ((optr->Sequence[0] == optr->Sequence[1]) || (optr->Sequence[1] == optr->Sequence[2]))
    {
      error("The requested sequence does not exist. Impossible to describe the movement of the input %s", input->name().c_str());
      return false;
    }
    const Joint* joint = nullptr;
    const Segment* segment = nullptr;
    if ((joint = node_cast<const Joint*>(input)) != nullptr)
    {
      std::string suffixProximal, suffixDistal;
      auto it = options.cend();
      if ((it = options.find("suffixProximal")) != options.cend())
      {
        suffixProximal = it->second.cast<std::string>();
      }
      if ((it = options.find("suffixDistal")) != options.cend())
      {
        suffixDistal = it->second.cast<std::string>();
      }
      Segment *proximal = joint->proximalSegment(), *distal = joint->distalSegment();
      if ((proximal != nullptr) && (distal != nullptr))
      {
        auto proximalTimeSequence = proximal->findChild<const TimeSequence*>(proximal->name() + suffixProximal, {{"type", TimeSequence::Pose}});
        auto distalTimeSequence = distal->findChild<const TimeSequence*>(distal->name() + suffixDistal, {{"type", TimeSequence::Pose}});
        if ((proximalTimeSequence == nullptr) || (distalTimeSequence == nullptr))
        {
          error("Impossible to find at least one of the time sequences required. Impossible to describe the movement of the joint %s", joint->name().c_str());
          return false;
        }
        if (proximalTimeSequence->samples() != distalTimeSequence->samples())
        {
          error("The number of samples is not the same in the time sequences. Impossible to describe the movement of the joint %s", joint->name().c_str());
          return false;
        }
        optr->OutputSampleRate = proximalTimeSequence->sampleRate();
        optr->OutputStartTime = proximalTimeSequence->startTime();
        if (std::fabs(distalTimeSequence->sampleRate() - optr->OutputSampleRate) > std::numeric_limits<float>::epsilon())
        {
          error("The sample rate of used time sequences is not the same. Impossible to describe the movement of the joint %s", joint->name().c_str());
          return false;
        }
        if (std::fabs(distalTimeSequence->startTime() - optr->OutputStartTime) > std::numeric_limits<float>::epsilon())
        {
          error("The start time of used time sequences is not the same. Impossible to describe the movement of the joint %s", joint->name().c_str());
          return false;
        }
        auto proximalPose = math::to_pose(proximalTimeSequence);
        auto distalPose = math::to_pose(distalTimeSequence);
        optr->BufferData = proximalPose.inverse().transform(distalPose);
      }
      else if ((proximal == nullptr) && (distal != nullptr))
      {
        auto distalTimeSequence = distal->findChild<const TimeSequence*>(distal->name() + suffixDistal);
        if (distalTimeSequence == nullptr)
        {
          error("Impossible to find the time sequence associated with the distal segment. Impossible to describe the movement of the joint %s", joint->name().c_str());
          return false;
        }
        optr->OutputSampleRate = distalTimeSequence->sampleRate();
        optr->OutputStartTime = distalTimeSequence->startTime();
        optr->BufferData = math::to_pose(distalTimeSequence);
      }
      else if ((proximal != nullptr) && (distal == nullptr))
      {
        error("Distal segment is null. Impossible to describe the movement of the joint %s", joint->name().c_str());
        return false;
      }
      else
      {
        error("Both segments are null. Impossible to describe the movement of the joint %s", joint->name().c_str());
        return false;
      }
    }
    else if ((segment = node_cast<const Segment*>(input)) != nullptr)
    {
      std::string suffix;
      auto it = options.cend();
      if ((it = options.find("suffix")) != options.cend())
      {
        suffix = it->second.cast<std::string>();
      }
      auto ts = segment->findChild<const TimeSequence*>(segment->name() + suffix);
      if (ts == nullptr)
      {
        error("Impossible to find the time sequence associated with the segment. Impossible to describe the movement of the segment %s", segment->name().c_str());
        return false;
      }
      optr->OutputSampleRate = ts->sampleRate();
      optr->OutputStartTime = ts->startTime();
      optr->BufferData = math::to_pose(ts);
    }
    else
    {
      error("The input is not a Joint or Segment object. Impossible to describe another kind of object");
      return false;
    }
    return true;
  };
  
  /**
   * Transform orientations into Euler angles. By default, the unit used is the radian and no adaptation scale is used.
   * To modify this behaviour, you can use the options:
   *  - enableDegreeConversion: boolean value
   *  - adaptForInterpretation: boolean value
   */
  bool EulerDescriptor::process(const std::unordered_map<std::string, Any>& options)
  {
    auto optr = this->pimpl();
    
    std::array<double,3> scale{{1.0,1.0,1.0}}; // unit conversion 
    auto offset = optr->Offset;
    auto it = options.cend();
    // Option enableDegreeConversion activated?
    if (((it = options.find("enableDegreeConversion")) != options.cend()) && (it->second.cast<bool>()))
    {
      _OPENMA_CONSTEXPR double rad2deg = 180.0 / M_PI;
      scale[0] *= rad2deg;
      scale[1] *= rad2deg;
      scale[2] *= rad2deg;
      offset[0] *= rad2deg;
      offset[1] *= rad2deg;
      offset[2] *= rad2deg;
      optr->OutputUnit = "deg";
    }
    else
    {
      optr->OutputUnit = "rad";
    }
    // Option adaptForInterpretation activated?
    if (((it = options.find("adaptForInterpretation")) != options.cend()) && (it->second.cast<bool>()))
    {
      scale[0] *= optr->Scale[0];
      scale[1] *= optr->Scale[1];
      scale[2] *= optr->Scale[2];
    }
    // Let's compute the output
    optr->OutputData = optr->BufferData.eulerAngles(optr->Sequence[0], optr->Sequence[1], optr->Sequence[2]);
    optr->OutputData.values().col(0) *= scale[0];
    optr->OutputData.values().col(0) += offset[0];
    optr->OutputData.values().col(1) *= scale[1];
    optr->OutputData.values().col(1) += offset[1];
    optr->OutputData.values().col(2) *= scale[2];
    optr->OutputData.values().col(2) += offset[2];
    return true;
  };
  
  /**
   * Export the Euler angles as TimeSequence. The name of the TimeSequence corresponds to the name of the Descriptor. No option is available for this method.
   */
  bool EulerDescriptor::finalize(Node* output, const std::unordered_map<std::string, Any>& options)
  {
    OPENMA_UNUSED(options);
    auto optr = this->pimpl();
    math::to_timesequence(optr->OutputData, this->name(), optr->OutputSampleRate, optr->OutputStartTime, TimeSequence::Angle, optr->OutputUnit, output);
    return true;
  };
 };
};