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

#include "openma/body/inversedynamicsmatrix.h"
#include "openma/body/inversedynamicsprocessor_p.h"

#include "openma/body/anchor.h"
#include "openma/body/chain.h"
#include "openma/body/inertialparameters.h"
#include "openma/body/joint.h"
#include "openma/body/model.h"
#include "openma/body/segment.h"
#include "openma/body/utils.h"
#include "openma/base/trial.h"
#include "openma/instrument/forceplate.h"
#include "openma/math.h"

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace body
{
  class InverseDynamicMatrixPrivate : public InverseDynamicProcessorPrivate
  {
    OPENMA_DECLARE_PINT_ACCESSOR(InverseDynamicMatrix)
      
  public:
    InverseDynamicMatrixPrivate(InverseDynamicMatrix* pint);
    ~InverseDynamicMatrixPrivate();
  };
  
  InverseDynamicMatrixPrivate::InverseDynamicMatrixPrivate(InverseDynamicMatrix* pint)
  : InverseDynamicProcessorPrivate(pint,"InverseDynamicMatrix")
  {};
  
  InverseDynamicMatrixPrivate::~InverseDynamicMatrixPrivate() = default;
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::InverseDynamicMatrix);

namespace ma
{
namespace body
{
  /**
   * @class InverseDynamicMatrix openma/body/matrixinversedynamics.h
   * @brief Algorithm to compute joint kinetics expressed in the global frame
   * 
   * @TODO Explain in details the required element to use this algorithm (model configure, external wrench, etc.).
   *
   * @ingroup openma_body
   */
  
  /**
   * Constructor
   */
  InverseDynamicMatrix::InverseDynamicMatrix(Node* parent)
  : InverseDynamicProcessor(*new InverseDynamicMatrixPrivate(this), parent)
  {};
  
  /**
   * Destructor
   */
  InverseDynamicMatrix::~InverseDynamicMatrix() _OPENMA_NOEXCEPT = default;
  
  /**
   * Internally this method is a combination of two algorithms published in the litterature [1,2].
   * Both of these algorithms are known to be less sensitive to noise [3]. The reason to use this is only to reduce computation time.
   *  - No need to convert to quaternion
   *  - No need to create sparse 6x6 matrices
   *  - Angular velocity and angular acceleration computed directly from rotation matrices
   *  - All the results are express in the inertial (global) coordinate system
   *
   * @par References
   *  1. Dumas et al., 2004, A 3D generic inverse dynamic method using wrench notation and quaternion algebra, Computer methods in Biomechanics and Biomedical Engineering, 7 (3), pp. 159-166
   *  2. Doriot & Chèze, 2004, A Three-Dimensional Kinematic and Dynamic Study of the Lower Limb During the Stance Phase of Gait Using an Homogeneous Matrix Approach, IEEE Transactions on Biomedical Engineering, 51 (1), pp. 21-27
   *  3. Dumas et al., 2007, Influence of the 3D Inverse Dynamic Method on the Joint Forces and Moments During Gait, ￼Journal of￼Biomechanical￼Engineering, 129, pp. 786-790
   */
  bool InverseDynamicMatrix::run(Node* inout)
  {
#if !defined(_MSC_VER)
#warning WE NEED TO MANAGE UNITS FOR FORCES AND MOMENTS
#endif
    
    auto models = inout->findChildren<Model*>({},{},false);
    for (auto& model : models)
    {
      auto chains = model->chains()->findChildren<Chain*>({},{},false);
      for (auto& chain : chains)
      {
        auto joints = chain->findChildren<Joint*>({},{},false);
        if (joints.empty())
        {
          warning("No joint found in the chain '%s'. Inverse dynamics for this chain aborted.", chain->name().c_str());
          continue;
        }
        // Properties check
        std::vector<TimeSequence*> tss(joints.size());
        unsigned inc = 0;
        for (const auto& joint : joints)
          tss[inc++] = joint->distalSegment()->pose();
        double rate = 0.0, start = 0.0;
        unsigned samples = 0;
        if (!compare_timesequences_properties(tss, rate, start, samples))
        {
          error("At least one segment's pose does not have the same sample rate, start time or number of samples in the chain '%s'.", chain->name().c_str());
          continue;
        }
        assert(rate > 0.);
        double dt = 1. / rate;
        double gres = 0.;
        math::Vector Fd, Md, pd;
        auto g = math::Map<const math::Vector>(1, model->gravity(), &gres).replicate(samples);
        // Iteration computation of joint kinetics
        for (auto itJ = joints.rbegin() ; itJ != joints.rend() ; ++itJ)
        {
          auto jnt = *itJ;
          auto seg = jnt->distalSegment();
          auto bsip = seg->findChild<InertialParameters*>();
          if (bsip == nullptr)
          {
            error("The segment '%s' does not have body segment inertial parameters. Inverse dynamics for the chain '%s' aborted.", seg->name().c_str(), chain->name().c_str());
            break;
          }
          
          // Internal variables used
          // -----------------------
          
          // - Pose of the segment
          auto pose = math::to_pose(seg->pose());
          // - Rotation component of the pose in the Inertial Coordinate System (ICS)
          auto R = pose.block<9>(0);
          // - Tensor of inertia expressed in the ICS
          math::Array<9> I = transform_relative_inertia(bsip, seg, pose);
          // - Centre of mass expressed in the ICS
          math::Position com = transform_relative_com(bsip, seg, pose);
          // - Mass of the segment
          double m = bsip->mass();
          // - Proximal end point of the segment
          auto pp = math::to_position(jnt->proximalAnchor()->position());
          if (!pp.isValid())
          {
            error("Unexpected error in the computation of proximal anchor position for the joint '%s'. Inverse dynamics for the chain '%s' aborted.", jnt->name().c_str(), chain->name().c_str());
            break;
          }
          // - Lever arm between the proximal end point and the centre of mass
          math::Position c = com - pp;
          
          // Derivatives computation
          // -----------------------
          // TODO Would it be possible to reduce the computation time by compute the angular velocity for the lower triangle only (same for the angular acceleration)
                    
          // - Angular velocity of the segment in the ICS
          math::Vector omega = R.derivative<1>(dt).transform(R.transpose()).skewRedux();
          // - Angular acceleration of the segment in the ICS
          math::Vector alpha = R.derivative<2>(dt).transform(R.transpose()).skewRedux();
          // - Linear acceleration of the CoM in the ICS
          auto a = com.derivative<2>(dt);
          
          // Forces and moments computed at the proximal end point
          // -----------------------------------------------------
          
          // - External contact (ground, etc.)
          math::Vector Fext(samples); Fext.values().setZero(); Fext.residuals().setZero();
          math::Vector Mext(samples); Mext.values().setZero(); Mext.residuals().setZero();
          auto externals = seg->findChildren<const TimeSequence*>({}, {{"type", TimeSequence::Wrench}});
          for (const auto& external : externals)
          {
            auto wrench = math::to_wrench(external);
            if (!wrench.isValid())
            {
              warning("The external wrench '%s' is not valid or does not have the same sample rate, start time or number of sample than the rest of the chain '%s'.", external->name().c_str(), chain->name().c_str());
              continue;
            }
            Fext += wrench.block<3>(0);
            Mext += wrench.block<3>(3) + (wrench.block<3>(6) - pp).cross(wrench.block<3>(0));
          }
          // - Add the forces and moments of distal joint (if any)
          // NOTE: The opposite sign (-=) is because of the use of the action forces and moments computed for the previous segment. In the current segment, they represent reaction forces and moments.
          if (pd.isValid())
          {
            Fext -= Fd;
            Mext -= Md + (pd - pp).cross(Fd);
          }
          // - Weight
          auto Fwei = m * g / 1000.0;
          auto Mwei = c.cross(m * g / 1000.0);
          // - Dynamics
          auto Fdyn = m * a / 1000.0;
          auto Mdyn = (I.transform(alpha) + omega.cross(I.transform(omega))) / 1000.0 + c.cross(m * a / 1000.0);
          // - Proximal joint (result)
          math::Vector Fp = Fdyn - Fwei - Fext;
          math::Vector Mp = Mdyn - Mwei - Mext;
          math::to_timesequence(Fp, jnt->name() + ".Force", rate, start, TimeSequence::Force, "N", jnt);
          math::to_timesequence(Mp, jnt->name() + ".Moment", rate, start, TimeSequence::Moment, "Nmm" , jnt);
          
          // Set the next (reaction) distal joint variables
          Fd = Fp;
          Md = Mp;
          pd = pp;
        }
      }
    }
    return true;
  };
  
  /**
   * Create a deep copy of the object and return it as another object.
   */
  InverseDynamicMatrix* InverseDynamicMatrix::clone(Node* parent) const
  {
    auto dest = new InverseDynamicMatrix;
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };
  
  /**
   * Do a deep copy of the the given @a source. The previous content is replaced.
   */
  void InverseDynamicMatrix::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const InverseDynamicMatrix*>(source);
    if (src == nullptr)
      return;
    this->InverseDynamicProcessor::copy(src);
  };

 };
};