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

#ifndef __openma_body_plugingait_p_h
#define __openma_body_plugingait_p_h

/*
 * WARNING: This file and its content are not included in the public API and 
 * can change drastically from one release to another.
 */

#include "openma/body/skeletonhelper_p.h"
#include "openma/body/eulerdescriptor.h"
#include "openma/body/point.h"
#include "openma/base/property.h"
#include "openma/base/macros.h" // _OPENMA_CONSTEXPR
#include "openma/math.h"

#include <Eigen/Geometry>
#include <Eigen_openma/Utils/sign.h>

#include <unordered_map>

namespace ma
{
  class Trial;
}

namespace ma
{
namespace body
{
  class Model;
  class PluginGait;
  
  using ummp = std::unordered_map<std::string,math::Map<math::Position>>;
  
  class PluginGaitPrivate : public SkeletonHelperPrivate
  {
    OPENMA_DECLARE_PINT_ACCESSOR(PluginGait)
    
    OPENMA_DECLARE_STATIC_PROPERTIES_DERIVED(PluginGait, SkeletonHelper,
      // Get / Set
      Property<PluginGait, double, &PluginGait::markerDiameter,                  &PluginGait::setMarkerDiameter>                {"markerDiameter"},
      Property<PluginGait, bool  , &PluginGait::headOffsetEnabled,               &PluginGait::setHeadOffsetEnabled>             {"headOffsetEnabled"},
      Property<PluginGait, double, &PluginGait::rightShoulderOffset,             &PluginGait::setRightShoulderOffset>           {"rightShoulderOffset"},
      Property<PluginGait, double, &PluginGait::leftShoulderOffset,              &PluginGait::setLeftShoulderOffset>            {"leftShoulderOffset"},
      Property<PluginGait, double, &PluginGait::rightElbowWidth,                 &PluginGait::setRightElbowWidth>               {"rightElbowWidth"},
      Property<PluginGait, double, &PluginGait::leftElbowWidth,                  &PluginGait::setLeftElbowWidth>                {"leftElbowWidth"},
      Property<PluginGait, double, &PluginGait::rightWristWidth,                 &PluginGait::setRightWristWidth>               {"rightWristWidth"},
      Property<PluginGait, double, &PluginGait::leftWristWidth,                  &PluginGait::setLeftWristWidth>                {"leftWristWidth"},
      Property<PluginGait, double, &PluginGait::rightHandThickness,              &PluginGait::setRightHandThickness>            {"rightHandThickness"},
      Property<PluginGait, double, &PluginGait::leftHandThickness,               &PluginGait::setLeftHandThickness>             {"leftHandThickness"},
      Property<PluginGait, double, &PluginGait::interAsisDistance,               &PluginGait::setInterAsisDistance>             {"interAsisDistance"},
      Property<PluginGait, double, &PluginGait::rightLegLength,                  &PluginGait::setRightLegLength>                {"rightLegLength"},
      Property<PluginGait, double, &PluginGait::leftLegLength,                   &PluginGait::setLeftLegLength>                 {"leftLegLength"},
      Property<PluginGait, double, &PluginGait::rightAsisTrochanterAPDistance,   &PluginGait::setRightAsisTrochanterAPDistance> {"rightAsisTrochanterAPDistance"},
      Property<PluginGait, double, &PluginGait::leftAsisTrochanterAPDistance,    &PluginGait::setLeftAsisTrochanterAPDistance>  {"leftAsisTrochanterAPDistance"},
      Property<PluginGait, double, &PluginGait::rightTibialTorsionOffset,        &PluginGait::setRightTibialTorsionOffset>      {"rightTibialTorsionOffset"},
      Property<PluginGait, double, &PluginGait::leftTibialTorsionOffset,         &PluginGait::setLeftTibialTorsionOffset>       {"leftTibialTorsionOffset"},
      Property<PluginGait, double, &PluginGait::rightThighRotationOffset,        &PluginGait::setRightThighRotationOffset>      {"rightThighRotationOffset"},
      Property<PluginGait, double, &PluginGait::leftThighRotationOffset,         &PluginGait::setLeftThighRotationOffset>       {"leftThighRotationOffset"},
      Property<PluginGait, double, &PluginGait::rightShankRotationOffset,        &PluginGait::setRightShankRotationOffset>      {"rightShankRotationOffset"},
      Property<PluginGait, double, &PluginGait::leftShankRotationOffset,         &PluginGait::setLeftShankRotationOffset>       {"leftShankRotationOffset"},
      Property<PluginGait, double, &PluginGait::rightKneeWidth,                  &PluginGait::setRightKneeWidth>                {"rightKneeWidth"},
      Property<PluginGait, double, &PluginGait::leftKneeWidth,                   &PluginGait::setLeftKneeWidth>                 {"leftKneeWidth"},
      Property<PluginGait, double, &PluginGait::rightAnkleWidth,                 &PluginGait::setRightAnkleWidth>               {"rightAnkleWidth"},
      Property<PluginGait, double, &PluginGait::leftAnkleWidth,                  &PluginGait::setLeftAnkleWidth>                {"leftAnkleWidth"},
      Property<PluginGait, bool,   &PluginGait::rightFootFlatEnabled,            &PluginGait::setRightFootFlatEnabled>          {"rightFootFlatEnabled"},
      Property<PluginGait, bool,   &PluginGait::leftFootFlatEnabled,             &PluginGait::setLeftFootFlatEnabled>           {"leftFootFlatEnabled"},
      // Get only
      Property<PluginGait, double, &PluginGait::headOffset>                                                                     {"headOffset"},
      Property<PluginGait, double, &PluginGait::rightStaticPlantarFlexionOffset>                                                {"rightStaticPlantarFlexionOffset"},
      Property<PluginGait, double, &PluginGait::rightStaticRotationOffset>                                                      {"rightStaticRotationOffset"},
      Property<PluginGait, double, &PluginGait::leftStaticPlantarFlexionOffset>                                                 {"leftStaticPlantarFlexionOffset"},
      Property<PluginGait, double, &PluginGait::leftStaticRotationOffset>                                                       {"leftStaticRotationOffset"},
      Property<PluginGait, double, &PluginGait::rightAnkleAbAddOffset>                                                          {"rightAnkleAbAddOffset"},
      Property<PluginGait, double, &PluginGait::leftAnkleAbAddOffset>                                                           {"leftAnkleAbAddOffset"}
    )
    
  public:
    PluginGaitPrivate(PluginGait* pint, const std::string& name, int region, int side, int variant);
    ~PluginGaitPrivate() _OPENMA_NOEXCEPT;
    
    void computeHipJointCentre(double* HJC, double S, double C, double xdis) const _OPENMA_NOEXCEPT;
    bool calibrateLowerLimb(int side, const math::Position* HJC, ummp* landmarks) _OPENMA_NOEXCEPT;
    bool reconstructUpperLimb(Model* model, Trial* trial, int side, const math::Vector* u_torso, const math::Vector* o_torso, ummp* landmarks, double sampleRate, double startTime) const _OPENMA_NOEXCEPT;
    bool reconstructLowerLimb(Model* model, Trial* trial, int side, const math::Vector* HJC, ummp* landmarks, double sampleRate, double startTime) const _OPENMA_NOEXCEPT;
    
    int Variant;
    
    double MarkerDiameter;
    
    bool HeadOffsetEnabled;
    
    double RightShoulderOffset;
    double LeftShoulderOffset;
    double RightElbowWidth;
    double LeftElbowWidth;
    double RightWristWidth;
    double LeftWristWidth;
    double RightHandThickness;
    double LeftHandThickness;
    
    double InterAsisDistance;
    double RightLegLength;
    double LeftLegLength;
    double RightAsisTrochanterAPDistance; 
    double LeftAsisTrochanterAPDistance;
    double RightTibialTorsionOffset;
    double LeftTibialTorsionOffset;
    double RightThighRotationOffset;
    double LeftThighRotationOffset;
    double RightShankRotationOffset;
    double LeftShankRotationOffset;
    double RightKneeWidth;
    double LeftKneeWidth;
    double RightAnkleWidth;
    double LeftAnkleWidth;
    
    bool RightFootFlatEnabled;
    bool LeftFootFlatEnabled;
    double HeadOffset;
    double RightStaticPlantarFlexionOffset;
    double RightStaticRotationOffset;
    double LeftStaticPlantarFlexionOffset;
    double LeftStaticRotationOffset;
    double RightAnkleAbAddOffset;
    double LeftAnkleAbAddOffset;
    
    using CalibrateJointFuncPtr = bool (*)(math::Position* , std::vector<double*>& , PluginGaitPrivate* , ummp* , const std::string& , double , double , const math::Position* );
    CalibrateJointFuncPtr CalibrateKneeJointCentre;
    CalibrateJointFuncPtr CalibrateAnkleJointCentre;
    
    using ConstructSegmentPosePtr = void (*)(math::Vector* , math::Vector* , math::Vector* , const math::Map<math::Position>* , const math::Map<math::Position>* , const math::Position* , const math::Position* , double );
    ConstructSegmentPosePtr ConstructThighPose;
    ConstructSegmentPosePtr ConstructShankPose;
  };
  
  inline void PluginGaitPrivate::computeHipJointCentre(double* HJC, double S, double C, double xdis) const _OPENMA_NOEXCEPT
  {
    //const double theta = 0.49567350756639; // 28.4 * M_PI / 180.0;
    //const double beta = 0.314159265358979; // 18.0 * M_PI / 180.0;
    const double theta = 0.5;
    const double beta = 0.314;
    const double r = this->MarkerDiameter / 2.0;
    HJC[0] = C * cos(theta) * sin(beta) - (xdis + r) * cos(beta),
    HJC[1] = S * (C * sin(theta) - this->InterAsisDistance / 2.0),
    HJC[2] = -1.0 * (C * cos(theta) * cos(beta) + (xdis + r) * sin(beta));
  };
  
  // ----------------------------------------------------------------------- //
  
  class PluginGaitLeftShoulderDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitLeftShoulderDescriptor, EulerDescriptor)
      
  public:
    PluginGaitLeftShoulderDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitRightShoulderDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitRightShoulderDescriptor, EulerDescriptor)
      
  public:
    PluginGaitRightShoulderDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitNeckDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitNeckDescriptor, EulerDescriptor)
      
  public:
    PluginGaitNeckDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitLeftAnkleDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitLeftAnkleDescriptor, EulerDescriptor)
      
  public:
    PluginGaitLeftAnkleDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitRightAnkleDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitRightAnkleDescriptor, EulerDescriptor)
      
  public:
    PluginGaitRightAnkleDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitSpineDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitSpineDescriptor, EulerDescriptor)
      
  public:
    PluginGaitSpineDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitPelvisDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitPelvisDescriptor, EulerDescriptor)
      
  public:
    PluginGaitPelvisDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitThoraxDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitThoraxDescriptor, EulerDescriptor)
      
  public:
    PluginGaitThoraxDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitHeadDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitHeadDescriptor, EulerDescriptor)
      
  public:
    PluginGaitHeadDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitRightFootDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitRightFootDescriptor, EulerDescriptor)
      
  public:
    PluginGaitRightFootDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
  
  class PluginGaitLeftFootDescriptor : public EulerDescriptor
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(EulerDescriptor)
    OPENMA_DECLARE_NODEID(PluginGaitLeftFootDescriptor, EulerDescriptor)
      
  public:
    PluginGaitLeftFootDescriptor(Node* parent);
  protected:
    virtual bool finalize(Node* output, const std::unordered_map<std::string, Any>& options) override;
  };
};
};

// ------------------------------------------------------------------------- //

namespace Eigen
{
namespace internal
{
  // ----------------------------------------------------------------------- //
  //                         ChordOpValues return value
  // ----------------------------------------------------------------------- //
  
  struct ChordOpValues;

  template <>
  struct traits<ChordOpValues>
  {
    using ReturnType = ma::math::Traits<ma::math::Array<3>>::Values;
  };
  
  struct ChordOpValues : public Eigen::ReturnByValue<ChordOpValues>
  {
    using StorageType = traits<ChordOpValues>::ReturnType;
    using Index = StorageType::Index;
    StorageType m_V;
  public:
    template <typename V> ChordOpValues(const V& v) : m_V(v) {};
    template <typename R> inline void evalTo(R& result) const {result.lazyAssign(this->m_V);};
    Index rows() const {return this->m_V.rows();};
    Index cols() const {return 3;};
  };
};
};

// ------------------------------------------------------------------------- //
  
namespace ma
{
namespace math
{
  template <typename XprOne, typename XprTwo, typename XprThree> class ChordOp;
  
  template <typename XprOne, typename XprTwo, typename XprThree>
  struct Traits<ChordOp<XprOne,XprTwo,XprThree>>
  {
    using Values = typename Array<3>::Values;
    using Residuals = typename Array<3>::Residuals;
    using Index = typename Values::Index;
    static _OPENMA_CONSTEXPR int ColsAtCompileTime = 3;
    static _OPENMA_CONSTEXPR int Processing = Full;
  };
  
  template <typename XprOne, typename XprTwo, typename XprThree>
  struct Nested<ChordOp<XprOne, XprTwo, XprThree>>
  {
    using type = const ChordOp<XprOne, XprTwo, XprThree>; // Plain object
  };
  
  template <typename XprOne, typename XprTwo, typename XprThree>
  class ChordOp : public math::XprBase<ChordOp<XprOne, XprTwo, XprThree>>
  {
    static_assert(XprOne::ColsAtCompileTime == XprTwo::ColsAtCompileTime && XprOne::ColsAtCompileTime == XprThree::ColsAtCompileTime, "The number of columns must be the same.");
    static_assert(XprOne::ColsAtCompileTime == 3 && XprTwo::ColsAtCompileTime == 3 && XprThree::ColsAtCompileTime == 3, "The chord operation is only available for array with 3 columns.");
    
    using Index = typename Traits<ChordOp<XprOne,XprTwo,XprThree>>::Index;
    
    typename Nested<XprOne>::type m_Xpr1;
    typename Nested<XprTwo>::type m_Xpr2;
    typename Nested<XprThree>::type m_Xpr3;
    double Offset;
    double Beta;
    
  public:
    ChordOp(double offset, const XprBase<XprOne>& x1, const XprBase<XprTwo>& x2, const XprBase<XprThree>& x3, double beta)
    : m_Xpr1(x1), m_Xpr2(x2), m_Xpr3(x3), Offset(offset), Beta(beta)
    {
      assert(this->m_Xpr1.rows() == this->m_Xpr2.rows());
      assert(this->m_Xpr2.rows() == this->m_Xpr3.rows());
      assert(this->m_Xpr1.rows() > 0);
      assert(this->m_Xpr2.rows() > 0);
      assert(this->m_Xpr3.rows() > 0);
    };
    
    const ChordOp& derived() const _OPENMA_NOEXCEPT {return *this;};
    
    Index rows() const _OPENMA_NOEXCEPT {return this->m_Xpr1.rows();};
    
    const Eigen::internal::ChordOpValues values() const _OPENMA_NOEXCEPT
    {
      const auto& I = this->m_Xpr1;
      const auto& J = this->m_Xpr2;
      const auto& K = this->m_Xpr3;
      // Construct the frame associated with I, J, K
      const Vector v = (J - I).normalized();
      const Vector u = v.cross(K - I).normalized();
      const Pose local(u, v, u.cross(v), (J + I) / 2.0);
      // Compute the angle to project I along v
      const Scalar d = (I - J).norm();
      const auto theta = (d.residuals() >= 0).select((this->Offset / d.values()).asin()*2.0, 0.0);
      // Do this projection in the local frame
      Vector t = local.inverse().transform(I);
      const Scalar::Values _tempX = t.values().middleCols<1>(1);
      const Scalar::Values _tempY = t.values().rightCols<1>();
      t.values().leftCols<1>().setZero();
      t.values().middleCols<1>(1) = _tempX * theta.cos() - _tempY * theta.sin() ;
      t.values().rightCols<1>() = _tempX * theta.sin() + _tempY * theta.cos() ;
      // Transform back the projected point and return the result
      Vector P = local.transform(t);
      // Is there a second condition that the projected point need to meet?
      // In case the beta angle is not null, an iterative solution is used to
      // Find the best projected point with the specified offset and the angle.
      // NOTE: THE PART BELOW IS AN ADAPTION OF THE CODE PROPOSED IN THE PYCGA PROJECT WHICH TAKES IT FROM MORGAN SANGEUX
      if (fabs(this->Beta) > std::numeric_limits<double>::epsilon())
      {
        
        // Next part is iterative and realized for each sample
        const double eps = 1e-5;
        for (Index i = 0, len = P.rows() ; i < len ; ++i)
        {
          if (P.residuals().coeff(i) < 0.0)
            continue;
          int count = 0;
          double sAlpha = 0, iAlpha = this->Beta, dBeta = fabs(this->Beta);
          // Angle 
          double ca = cos(theta.coeff(i) / 2.0);
          // Distance where should be the new project point
          double r = this->Offset * ca;
          // Compute the projection of K on IJ using the Pythagorean theorem
          const Eigen::Matrix<double,1,3> O = J.values().row(i) - v.values().row(i) * sqrt(d.values().coeff(i) * d.values().coeff(i) - this->Offset * this->Offset * (1 + ca * ca));
          // Compute the local frame used to find the new projected point satisfaying an angle close to beta
          Eigen::Matrix<double,1,3> Pi = P.values().row(i);
          const Eigen::Matrix<double,1,3> w2 = v.values().row(i);
          const Eigen::Matrix<double,1,3> v2 = w2.cross(Pi-O).normalized();
          const Eigen::Matrix<double,1,3> u2 = v2.cross(w2).normalized();
          // const Pose local2(v,v2,v2.cross(v).normalized(),O);
          while ((dBeta > eps) && (count < 1000))
          {
            sAlpha += iAlpha;
            // Compute the new projected point
            double c = r * cos(sAlpha);
            double s = r * sin(sAlpha);
            P.values().coeffRef(i,0) = u2.coeff(0) * c + v2.coeff(0) * s + O.coeff(0);
            P.values().coeffRef(i,1) = u2.coeff(1) * c + v2.coeff(1) * s + O.coeff(1);
            P.values().coeffRef(i,2) = u2.coeff(2) * c + v2.coeff(2) * s + O.coeff(2);
            // Verify if the new point projected on the plane corresponds to the angle beta
            Pi = P.values().row(i);
            const Eigen::Matrix<double,1,3> Ii = I.values().row(i);
            const Eigen::Matrix<double,1,3> Ji = J.values().row(i);
            const Eigen::Matrix<double,1,3> Ki = K.values().row(i);
            Eigen::Matrix<double,1,3> nBone = Ji - Pi;
            Eigen::Matrix<double,1,3> projK = nBone.cross((Ki - Pi).cross(nBone));
            Eigen::Matrix<double,1,3> projI = nBone.cross((Ii - Pi).cross(nBone));
            double iBeta = sign((projK.cross(projI) * nBone.transpose()).coeff(0)) * acos((projK * projI.transpose() / projK.norm() / projI.norm()).coeff(0));
            double odBeta = dBeta;
            dBeta = fabs(this->Beta - iBeta);
            // Look for the direction and magnitude used by the next iteration
            if ((dBeta - odBeta) > 0.0)
            {
              if (count == 0)
              {
                sAlpha -= iAlpha;
                iAlpha *= -1.0;
              }
              else
              {
                iAlpha /= -2.0; 
              }
            }
            ++count;
          }
        }
      }
      // Pass the result to be assigned in the result of the operation chord.
      return Eigen::internal::ChordOpValues(P.values());
    };
  
    auto residuals() const _OPENMA_NOEXCEPT -> decltype(generate_residuals((OPENMA_MATHS_DECLVAL_NESTED(XprOne).residuals() >= 0.0) && (OPENMA_MATHS_DECLVAL_NESTED(XprTwo).residuals() >= 0.0) && (OPENMA_MATHS_DECLVAL_NESTED(XprThree).residuals() >= 0.0)))
    {
      return generate_residuals((this->m_Xpr1.residuals() >= 0.0) && (this->m_Xpr2.residuals() >= 0.0) && (this->m_Xpr3.residuals() >= 0.0));
    };
  };
  
  template <typename XprOne, typename XprTwo, typename XprThree>
  static inline ChordOp<XprOne,XprTwo,XprThree> compute_chord(double offset, const XprBase<XprOne>& I, const XprBase<XprTwo>& J, const XprBase<XprThree>& K, double beta = 0.0)
  {
    return ChordOp<XprOne,XprTwo,XprThree>(offset,I,J,K,beta);
  };
};
};

#endif // __openma_body_plugingait_p_h
