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

#include "openma/body/inertialparameters.h"
#include "openma/body/inertialparameters_p.h"

#include <algorithm> // std::copy

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  InertialParametersPrivate::InertialParametersPrivate(InertialParameters* pint, const std::string& name)
  : NodePrivate(pint,name), Mass(0.), CenterOfMass{0.,0.,0.}, Inertia{0.,0.,0.,0.,0.,0.,0.,0.,0.}
  {};
  
  InertialParametersPrivate::~InertialParametersPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(body::InertialParameters);

namespace ma
{
namespace body
{
  /**
   * @class InertialParameters openma/body/inertialparameters.h
   * @brief Representation of body segment inertial parameters
   * Body segment inertial paramters (BSIP) correspond to:
   *  - A mass
   *  - The coordinates of the centre of mass
   *  - A tensor of inertia 
   *
   * The coordinates of the center of mass are stored as a relative 3D vector [px, py, pz].
   * The tensor of inertia is stored as a relative 3x3 matrix [Ixx, Iyx, Izx ; Iyx, Iyy, Iyz ; Izx, Izy, Izz]
   *
   * The choice of the relative reference frame is up to the developer.
   *
   * While InertialParameters object can be set manually, most of the model should use InertialParametersEstimator derived classes (e.g. DempsterTable).
   *
   * @ingroup openma_body
   * @sa Segment
   */
  
  /**
   * Default constructor. You have to use the methods setMass(), setCenterOfMass(), and setInertia() to set the corresponding parameters.
   * By default, these parameters are set to:
   *  - Mass: 0
   *  - Centre of Mass: 0, 0, 0.
   *  - Tensor of inertia: 0, 0, 0, 0, 0, 0, 0, 0, 0.
   */
  InertialParameters::InertialParameters(const std::string& name, Node* parent)
  : Node(*new InertialParametersPrivate(this,name), parent)
  {};
  
  /**
   * Complete constructor which set the @a mass, the coordinates of the center of mass (@a com), and the tensor of @a inertia.
   * The @a com and @a intertial must be non-null, otherwise, the behaviour is not known.
   */
  InertialParameters::InertialParameters(const std::string& name, double mass, const double com[3], const double inertia[9], Node* parent)
  : InertialParameters(name, parent) 
  {
    auto optr = this->pimpl();
    optr->Mass = mass;
    std::copy_n(com, 3, optr->CenterOfMass);
    std::copy_n(inertia, 9, optr->Inertia);
  };

  /**
   * Destructor
   */
  InertialParameters::~InertialParameters() _OPENMA_NOEXCEPT = default;
  
  /**
   * Returns the mass
   */
  double InertialParameters::mass() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Mass;
  };
  
  /**
   * Sets the mass. If the passed @a value is different than the current one, the state of the object is modified.
   */
  void InertialParameters::setMass(double value)
  {
    auto optr = this->pimpl();
    if (optr->Mass == value)
      return;
    optr->Mass = value;
    this->modified();
  };
  
  /**
   * Returns the coordinates of the center of mass.
   */
  const double* InertialParameters::centerOfMass() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->CenterOfMass;
  };
  
  /**
   * Sets the coordinates of the center of mass. If the passed @a value is different than the current one, the state of the object is modified.
   */
  void InertialParameters::setCenterOfMass(const double value[3])
  {
    auto optr = this->pimpl();
    if ((optr->CenterOfMass[0] == value[0]) && (optr->CenterOfMass[1] == value[1]) && (optr->CenterOfMass[2] == value[2]))
      return;
    std::copy_n(value, 3, optr->CenterOfMass);
    this->modified();
  };
  
  /**
   * Returns the tensor of inertia
   */
  const double* InertialParameters::inertia() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Inertia;
  };
  
  /**
   * Sets the tensor of inertia. If the passed @a value is different than the current one, the state of the object is modified.
   */
  void InertialParameters::setInertia(const double value[9])
  {
    auto optr = this->pimpl();
    if ( (optr->Inertia[0] == value[0]) && (optr->Inertia[1] == value[1]) && (optr->Inertia[2] == value[2])
      && (optr->Inertia[3] == value[3]) && (optr->Inertia[4] == value[4]) && (optr->Inertia[5] == value[5])
      && (optr->Inertia[6] == value[6]) && (optr->Inertia[7] == value[7]) && (optr->Inertia[8] == value[8]) )
      return;
    std::copy_n(value, 9, optr->Inertia);
    this->modified();
  };
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  InertialParameters* InertialParameters::clone(Node* parent) const
  {
    auto dest = new InertialParameters(this->name());
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void InertialParameters::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const InertialParameters*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    this->Node::copy(src);
    optr->Mass = optr_src->Mass;
    std::copy(optr_src->CenterOfMass, optr_src->CenterOfMass+3, optr->CenterOfMass);
    std::copy(optr_src->Inertia, optr_src->Inertia+9, optr->Inertia);
 };
};
};