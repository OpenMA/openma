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

#include "openma/body/anchor.h"
#include "openma/base/node_p.h"
#include "openma/body/segment.h"
#include "openma/body/point.h"
#include "openma/body/referenceframe.h"
#include "openma/body/utils.h"
#include "openma/math.h"
#include "openma/base/timesequence.h"
#include "openma/base/logger.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  class AnchorPrivate : public NodePrivate
  {
  protected:
    OPENMA_DECLARE_PINT_ACCESSOR(Anchor)
      
  public:
    AnchorPrivate(Anchor* pint, Segment* source, const std::string& rel);
    virtual ~AnchorPrivate();
    // virtual AnchorPrivate* clone() const = 0;
    virtual TimeSequence* computePosition() = 0;
    Segment* Source;
    std::string Relative;
  };
  
  AnchorPrivate::AnchorPrivate(Anchor* pint, Segment* source, const std::string& rel)
  : NodePrivate(pint,{}), Source(source), Relative(rel)
  {};
  
  AnchorPrivate::~AnchorPrivate() = default;
  
  // ----------------------------------------------------------------------- //
  
  class AnchorPointPrivate : public AnchorPrivate
  {
  public:
    AnchorPointPrivate(Anchor* pint, Segment* source, const std::string& relpoint);
    ~AnchorPointPrivate();
    // virtual AnchorPrivate* clone() const final;
    virtual TimeSequence* computePosition() final;
  };
  
  AnchorPointPrivate::AnchorPointPrivate(Anchor* pint, Segment* source, const std::string& relpoint)
  : AnchorPrivate(pint, source, relpoint)
  {};
  
  AnchorPointPrivate::~AnchorPointPrivate() = default;
  //
  // AnchorPrivate* AnchorPointPrivate::clone() const
  // {
  //   return new AnchorPointPrivate(this->Source, this->Relative);
  // };
  
  TimeSequence* AnchorPointPrivate::computePosition()
  {
    // TODO Implement the cache mechanism
    Point* rel = nullptr;
    if ((rel = this->Source->findChild<Point*>(this->Relative)) == nullptr)
    {
      error("No relative point '%s' found within the segment '%s'. Impossible to compute anchor's position", this->Relative.c_str(), this->Source->name().c_str());
      return nullptr;
    }
    auto ts = this->Source->pose();
    if (ts == nullptr)
    {
      error("No time sequence attached to the source '%s'.", this->Source->name().c_str());
      return nullptr;
    }
    auto pos = transform_relative_point(rel, this->Source, math::to_pose(ts));
    if (!pos.isValid())
    {
      error("Error during the computation of the anchor's position. Verify if the segment '%s' has a non null pose associated with it.", this->Source->name().c_str());
      return nullptr;
    }
    // NOTE The unit must be set later
    return math::to_timesequence(pos, this->pint()->name()+".Position.Cache", ts->sampleRate(), ts->startTime(), TimeSequence::Position, "", this->pint());
  };
  
  // ----------------------------------------------------------------------- //
  
  class AnchorOriginPrivate : public AnchorPrivate
  {
  public:
    AnchorOriginPrivate(Anchor* pint, Segment* source, const std::string& relpoint);
    ~AnchorOriginPrivate();
    // virtual AnchorPrivate* clone() const final;
    virtual TimeSequence* computePosition() final;
  };
  
  AnchorOriginPrivate::AnchorOriginPrivate(Anchor* pint, Segment* source, const std::string& relpoint)
  : AnchorPrivate(pint, source, relpoint)
  {};
  
  AnchorOriginPrivate::~AnchorOriginPrivate() = default;
  
  // AnchorPrivate* AnchorOriginPrivate::clone() const
  // {
  //   return new AnchorOriginPrivate(this->Source, this->Relative);
  // };
  
  TimeSequence* AnchorOriginPrivate::computePosition()
  {
    // TODO Implement the cache mechanism
    auto ts = this->Source->pose();
    math::Position pos;
    if (!this->Relative.empty())
    {
      ReferenceFrame* rel = nullptr;
      if ((rel = this->Source->findChild<ReferenceFrame*>(this->Relative)) == nullptr)
      {
        error("No relative reference frame '%s' found within the segment '%s'. Impossible to compute anchor's position", this->Relative.c_str(), this->Source->name().c_str());
        return nullptr;
      }
      pos = transform_relative_frame(rel, this->Source, math::to_pose(ts)).block<3>(9);
    }
    else
    {
      pos = math::to_pose(ts).block<3>(9);
    }
    if (!pos.isValid())
    {
      error("Error during the computation of the anchor's position. Verify if the segment '%s' has a non null pose associated with it.", this->Source->name().c_str());
      return nullptr;
    }
    // NOTE The unit must be set later
    return math::to_timesequence(pos, this->pint()->name()+".Position.Cache", ts->sampleRate(), ts->startTime(), TimeSequence::Position, "", this->pint());
  }
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::Anchor);

namespace ma
{
namespace body
{ 
  /**
   * @class Anchor "openma/body/anchor.h"
   * @brief Special point on a segment.
   *
   * An Anchor is used in the definition of a Joint. This is particulary important when you want to compute rigid body dynamics as you need to use the proximal and distal endpoints of each segment. 
   * There are different ways to construct an anchor.
   *  - Define an anchor corresponding to a relative point attached to a segment: @see Anchor::point(const std::string&, Segment*)
   *  - Define an anchor corresponding to the origin of a relative frame attached to a segment: @see Anchor::origin(const std::string&, Segment*)
   *  - Define an anchor corresponding to the origin of the first frame describing the movement of the segment: @see Anchor::origin(Segment*)
   *
   * The method position() computes the position of the anchor based on the movement of the source and the relative element given (i.e a relative point or the origin of a relative reference frame).
   *
   * @sa Chain, Joint, Point, ReferenceFrame, Segment
   * @ingroup openma_body
   */
  
  /**
   * Create an Anchor defined by a @a source and the relative point @a relpoint.
   * The relative point must be a child of the @a source but not necessarily a direct descendant. This relative point can be for example expressed in another relative reference frame (e.g the hip joint center relative to the segmental frame, itself expressed in the technical frame).
   * @important This is the responsability of the developer to delete this pointer (or to attach it to a parent). In the case of the Joint class, the later takes the ownership of the anchor and will destroy when necessary
   */
  Anchor* Anchor::point(const std::string& relpoint, Segment* source)
  {
    return new Anchor(relpoint, source);
  };
  
  /**
   * Create an Anchor defined by a @a source and the origin of the relative reference frame @a relframe.
   * The relative frame must be a child of the @a source but not necessarily a direct descendant. This relative frame can be for example expressed in another relative reference frame.
   * @important This is the responsability of the developer to delete this pointer (or to attach it to a parent). In the case of the Joint class, the later takes the ownership of the anchor and will destroy when necessary
   */
  Anchor* Anchor::origin(const std::string& relframe, Segment* source)
  {
    return new Anchor(source, relframe);
  };
  
  /**
   * Create an Anchor defined by a @a source and the origin of the frame defining its movement (e.g. Segment::pose()).
   * @important This is the responsability of the developer to delete this pointer (or to attach it to a parent). In the case of the Joint class, the later takes the ownership of the anchor and will destroy when necessary
   */
  Anchor* Anchor::origin(Segment* source)
  {
    return Anchor::origin({}, source);
  };
  
  /**
   * Constructor used with a relative point
   */
  Anchor::Anchor(const std::string& relpoint, Segment* source)
  : Node(*new AnchorPointPrivate(this, source, relpoint), nullptr)
  {};
  
  /**
   * Constructor used with a relative reference frame
   */
  Anchor::Anchor(Segment* source, const std::string& relframe)
  : Node(*new AnchorOriginPrivate(this, source, relframe), nullptr)
  {};
  
  /**
   * Destructor (defaut)
   */
  Anchor::~Anchor() _OPENMA_NOEXCEPT = default;
 
  /**
   * Sets the source associated with this anchor
   * @important This method should be used only for internal purposes. Once an anchor is set in a model, this one should not be modified
   */
  void Anchor::setSource(Segment* source) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->Source = source;
  };
  
  /**
   * Returns the source associated with this anchor
   */
  Segment* Anchor::source() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Source;
  };
  
  /**
   * Sets the name of the relative element associated with the source. This name should correspond to the name of a Node. Currently, The types of Node supported are Point and ReferenceFrame.
   * @important This method should be used only for internal purposes. Once an anchor is set in a model, this one should not be modified.
   */
  void Anchor::setRelative(const std::string& rel) _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    optr->Relative = rel;
  };
  
  /**
   * Returns the name of the relative element associated with the source.
   */
  const std::string& Anchor::relative() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Relative;
  };
  
  /**
   * Computes the position of the anchor based on the movement of the source and the relative element given.
   * In case no source was set or the relative element was not found, this method returns a null pointer.
   * @important The unit for the TimeSequence object returned by this method is not set. It is the responsability to the developer to set it afterwards.
   */
  TimeSequence* Anchor::position()
  {
    auto optr = this->pimpl();
    if (optr->Source == nullptr)
    {
      error("No anchor's source. Impossible to compute anchor's position");
      return nullptr;
    }
    return optr->computePosition();
  };
};
};