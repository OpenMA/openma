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

#include "openma/instrument/forceplate.h"
#include "openma/instrument/forceplate_p.h"
#include "openma/base/timesequence.h"
#include "openma/base/logger.h"
#include "openma/math.h"

#include <Eigen/Geometry>

#include <algorithm> // std::copy
#include <cassert>

// -------------------------------------------------------------------------- //
//                                 PRIVATE API                                //
// -------------------------------------------------------------------------- //

#ifndef DOXYGEN_SHOULD_SKIP_THIS

namespace ma
{
namespace instrument
{
  ForcePlatePrivate::ForcePlatePrivate(ForcePlate* pint, const std::string& name, int type, std::vector<std::string>&& labels, unsigned rows, unsigned cols)
  : HardwarePrivate(pint, name, std::move(labels)),
    Type(type),
    ReferenceFrame{1.,0.,0.,0.,1.,0.,0.,0.,1.,0.,0.,0.},
    SurfaceCorners{0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.},
    RelativeSurfaceOrigin{0.,0.,0.},
    CalibrationMatrixDimensions{rows,cols},
    CalibrationMatrixData(nullptr)
  {
    if ((rows != 0) && (cols != 0))
    {
      size_t num = rows * cols;
      this->CalibrationMatrixData = new double[num];
      for (size_t i = 0 ; i < num ; ++i)
        this->CalibrationMatrixData[i] = 0.;
    }
  };
  
  ForcePlatePrivate::ForcePlatePrivate(ForcePlate* pint, const std::string& name, int type, std::vector<std::string>&& labels)
  : ForcePlatePrivate(pint, name, type, std::move(labels), 0, 0)
  {};
  
  ForcePlatePrivate::~ForcePlatePrivate() _OPENMA_NOEXCEPT
  {
    delete[] this->CalibrationMatrixData;
  };
};
};

#endif

// -------------------------------------------------------------------------- //
//                                 PUBLIC API                                 //
// -------------------------------------------------------------------------- //

OPENMA_INSTANCE_STATIC_TYPEID(instrument::ForcePlate);

namespace ma
{
namespace instrument
{
#ifdef DOXYGEN_SHOULD_TAKE_THIS
  /** * @brief Fake structure to create node's properties */
  struct ForcePlate::__Doxygen_Properties
  {
  /**
   * This property return the internal code used by this kind of force plate.
   * Historically, each type came from the C3D format specification
   * @sa type()
   */
  int Type;
  };
#endif  
  
  /**
   *
   */
  int ForcePlate::type() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->Type;
  };
  
  /**
   *
   */
  void ForcePlate::setGeometry(const double rso[3], const double sc1[3], const double sc2[3], const double sc3[3], const double sc4[3])
  {
    auto optr = this->pimpl();
    // Copy directly the coordinates of the relative surface origin
    std::copy(rso, rso+3, optr->RelativeSurfaceOrigin);
    // Copy directly the coordinates of the relative surface origin
    std::copy(sc1, sc1+3, optr->SurfaceCorners);
    std::copy(sc2, sc2+3, optr->SurfaceCorners+3);
    std::copy(sc3, sc3+3, optr->SurfaceCorners+6);
    std::copy(sc4, sc4+3, optr->SurfaceCorners+9);
    // Compute the reference frame based on given information
    Eigen::Map<Eigen::Matrix<double,3,1>> off(optr->RelativeSurfaceOrigin);
    Eigen::Map<Eigen::Matrix<double,3,4>> SC(optr->SurfaceCorners);
    Eigen::Map<Eigen::Matrix<double,3,4>> T(optr->ReferenceFrame);
    T.col(0) = (SC.col(0) - SC.col(1)).normalized();
    T.col(2) = T.col(0).cross(SC.col(0) - SC.col(3)).normalized();
    T.col(1) = T.col(2).cross(T.col(0));
    Eigen::Matrix<double,3,1> so = (SC.col(0) + SC.col(2)) / 2.0;
    T.col(3) = T.bottomLeftCorner<3,3>() * -off + so;
  };
  
  
  /**
   * @important Expressed in the PARENT reference frame (i.e. generally the reference frame associated with the laboratory - also knwon as the global frame)
   */
  const double* ForcePlate::referenceFrame() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->ReferenceFrame;
  };
  
  // /**
  //  * @warning Must be expressed in the PARENT reference frame (i.e. generally the reference frame associated with the laboratory - also knwon as the global frame)
  //  */
  // void ForcePlate::setReferenceFrame(double u[3], double v[3], double w[3], double o[3])
  // {
  //   auto optr = this->pimpl();
  //   if ( (optr->ReferenceFrame[0] == u[0]) && (optr->ReferenceFrame[1] == u[1]) && (optr->ReferenceFrame[2] == u[2])
  //     && (optr->ReferenceFrame[3] == v[0]) && (optr->ReferenceFrame[4] == v[1]) && (optr->ReferenceFrame[5] == v[2])
  //     && (optr->ReferenceFrame[6] == w[0]) && (optr->ReferenceFrame[7] == w[1]) && (optr->ReferenceFrame[8] == w[2])
  //     && (optr->ReferenceFrame[9] == o[0]) && (optr->ReferenceFrame[10]== o[1]) && (optr->ReferenceFrame[11]== o[2]))
  //     return;
  //   std::copy(u, u+3, optr->ReferenceFrame);
  //   std::copy(v, v+3, optr->ReferenceFrame+3);
  //   std::copy(w, w+3, optr->ReferenceFrame+6);
  //   std::copy(o, o+3, optr->ReferenceFrame+9);
  //   this->modified();
  // };
  
  /**
   * @important Expressed in the PARENT reference frame (i.e. generally the reference frame associated with the laboratory - also knwon as the global frame)
   */
  const double* ForcePlate::surfaceCorners() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->SurfaceCorners;
  };
  
  // /**
  //  * @warning Must be expressed in the LOCAL reference frame (i.e. the reference frame associated with the force plate)
  //  */
  // void ForcePlate::setSurfaceSurfaceCorners(double c1[3], double c2[3], double c3[3], double c4[3])
  // {
  //   auto optr = this->pimpl();
  //   if ( (optr->SurfaceCorners[0] == c1[0]) && (optr->SurfaceCorners[1] == c1[1]) && (optr->SurfaceCorners[2] == c1[2])
  //     && (optr->SurfaceCorners[3] == c2[0]) && (optr->SurfaceCorners[4] == c2[1]) && (optr->SurfaceCorners[5] == c2[2])
  //     && (optr->SurfaceCorners[6] == c3[0]) && (optr->SurfaceCorners[7] == c3[1]) && (optr->SurfaceCorners[8] == c3[2])
  //     && (optr->SurfaceCorners[9] == c4[0]) && (optr->SurfaceCorners[10]== c4[1]) && (optr->SurfaceCorners[11]== c4[2]))
  //     return;
  //   std::copy(c1, c1+3, optr->SurfaceCorners);
  //   std::copy(c2, c2+3, optr->SurfaceCorners+3);
  //   std::copy(c3, c3+3, optr->SurfaceCorners+6);
  //   std::copy(c4, c4+3, optr->SurfaceCorners+9);
  // };
  
  /**
   * @important Expressed in the LOCAL frame (i.e. the frame associated with the force plate)
   */
  const double* ForcePlate::relativeSurfaceOrigin() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->RelativeSurfaceOrigin;
  };
  
  // /**
  //  * @important Must be expressed in the LOCAL frame (i.e. the frame associated with the force plate)
  //  */
  // void ForcePlate::setRelativeSurfaceOrigin(double off[3])
  // {
  //
  // };
  
  /**
   * 
   */
  const unsigned* ForcePlate::calibrationMatrixDimensions() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->CalibrationMatrixDimensions;
  };
  
  /**
   *
   */
  double* ForcePlate::calibrationMatrixData() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    return optr->CalibrationMatrixData;
  };
  
  /**
   *
   */
  void ForcePlate::setCalibrationMatrixData(const double* data)
  {
    auto optr = this->pimpl();
    int num = optr->CalibrationMatrixDimensions[0] * optr->CalibrationMatrixDimensions[1];
    bool isEqual = true;
    for (int i = 0 ; i < num ; ++i)
    {
      if (optr->CalibrationMatrixData[i] != data[i])
      {
        isEqual = false;
        break;
      }
    }
    if (!isEqual)
    {
      std::copy(data, data+num, optr->CalibrationMatrixData);
      this->modified();
    }
  };
  
  /**
   *
   */
  TimeSequence* ForcePlate::wrench(Location loc, bool global, double threshold)
  {
    auto channels = this->retrieveChannels();
    if (channels.empty())
    {
      error("At least one channel for the force plate '%s' was not found. Impossible to do wrench computation.", this->name().c_str());
      return nullptr;
    }
    double sampleRate = 0., startTime = 0.;
    unsigned samples = 0;
    if (!compare_timesequences_properties(channels, sampleRate, startTime, samples))
    {
      error("At least one channel for the force plate '%s' does not have the same sample rate, start time, or number of samples than the other. Impossible to do wrench computation.", this->name().c_str());
      return nullptr;
    }
    std::string name = this->name() + ".Wrench." + (global ? "Global." : "Local.") + this->stringifyLocation(loc);
    auto w = this->outputs()->findChild<TimeSequence*>(name,{{"type",ma::TimeSequence::Wrench},{"components",10}},false);
    if (w == nullptr)
      w = new TimeSequence(name, 10, samples, sampleRate, startTime, ma::TimeSequence::Wrench,"",this->outputs());
    // Is it necessary to do the computation or the cache is still up to date?
    if (w->timestamp() < this->timestamp())
    {
      if (!this->computeWrenchAtOrigin(w)
          || !this->computePosition(w, loc, threshold)
            || (global && !this->transformToGlobal(w)))
      {
        // An error message should aready be displayed by the other methods
        delete w;
        w = nullptr;
      }
    }
    return w;
  };

  /**
   *
   */
  void ForcePlate::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const ForcePlate*>(source);
    if (src == nullptr)
      return;
    auto optr = this->pimpl();
    auto optr_src = src->pimpl();
    assert(optr_src->CalibrationMatrixDimensions[0] == optr->CalibrationMatrixDimensions[0]);
    assert(optr_src->CalibrationMatrixDimensions[1] == optr->CalibrationMatrixDimensions[1]);
    assert(((optr_src->CalibrationMatrixData != nullptr) && (optr->CalibrationMatrixData != nullptr))
        || ((optr_src->CalibrationMatrixData == nullptr) && (optr->CalibrationMatrixData == nullptr)));
    this->Hardware::copy(src);
    optr->Type = optr_src->Type;
    std::copy(optr_src->ReferenceFrame, optr_src->ReferenceFrame+12, optr->ReferenceFrame);
    std::copy(optr_src->SurfaceCorners, optr_src->SurfaceCorners+12, optr->SurfaceCorners);
    std::copy(optr_src->RelativeSurfaceOrigin, optr_src->RelativeSurfaceOrigin+3, optr->RelativeSurfaceOrigin);
    if (optr_src->CalibrationMatrixData != nullptr)
      std::copy(optr_src->CalibrationMatrixData, optr_src->CalibrationMatrixData+(optr->CalibrationMatrixDimensions[0]*optr->CalibrationMatrixDimensions[1]), optr->CalibrationMatrixData);
  };
  
  ForcePlate::ForcePlate(ForcePlatePrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT
  : Hardware(pimpl, parent)
  {};
  
  std::string ForcePlate::stringifyLocation(Location loc) const _OPENMA_NOEXCEPT
  {
    std::string str;
    switch(loc)
    {
    case Location::Origin:
      str = "O";
      break;
    case Location::SurfaceOrigin:
      str = "SO";
      break;
    case Location::CentreOfPressure:
      str = "COP";
      break;
    case Location::PointOfApplication:
      str = "PWA";
      break;
    default:
      break;
    }
    return str;
  };
  
  std::vector<TimeSequence*> ForcePlate::retrieveChannels() const _OPENMA_NOEXCEPT
  {
    auto optr = this->pimpl();
    std::vector<TimeSequence*> channels;
    channels.reserve(optr->CalibrationMatrixDimensions[1]);
    for (const auto& mappedChannel: optr->MappedChannels)
    {
      if (mappedChannel.second == nullptr)
      {
        channels.clear();
        break;
      }
      channels.push_back(mappedChannel.second);
    }
    return channels;
  };
  
  bool ForcePlate::computePosition(TimeSequence* w, Location loc, double threshold)
  {
    auto W = math::to_wrench(w);
    auto o = this->relativeSurfaceOrigin();
    // Adaptation of the moment and position depending of the specified location
    auto Fx = W.values().col(0);
    auto Fy = W.values().col(1);
    auto Fz = W.values().col(2);
    auto Mx = W.values().col(3);
    auto My = W.values().col(4);
    auto Mz = W.values().col(5);
    auto Px = W.values().col(6);
    auto Py = W.values().col(7);
    auto Pz = W.values().col(8);
    auto res = W.residuals();
    if (loc == Location::Origin)
    {
      Px.setZero();
      Py.setZero();
      Pz.setZero();
      res.setZero();
    }
    else
    {
      // Position
      Px.setConstant(o[0]);
      Py.setConstant(o[1]);
      Pz.setConstant(o[2]);
      // Moment
      Mx += Fy * o[2] - o[1] * Fz;
      My += Fz * o[0] - o[2] * Fx;
      Mz += Fx * o[1] - o[0] * Fy;
      // Residuals
      res.setZero();
    }
    if (loc == Location::CentreOfPressure)
    {
      Px = - My / Fz;
      Py =   Mx / Fz;
    }
    else if (loc == Location::PointOfApplication)
    {
      // For explanations of the PWA calculation, see Shimba T. (1984), 
      // "An estimation of center of gravity from force platform data", 
      // Journal of Biomechanics 17(1), 53–60.
      math::Array<1>::Values sNF = W.values().block(0,0,W.rows(),3).square().rowwise().sum().sqrt();
      Px = (Fy * Mz - Fz * My) / sNF - (Fx.square() * My - Fx * (Fy * Mx)) / (sNF * Fz);
      Py = (Fz * Mx - Fx * Mz) / sNF - (Fx * (Fy * My) - Fy.square() * Mx) / (sNF * Fz);
    }
    if ((loc == Location::CentreOfPressure) || (loc == Location::PointOfApplication))
    {
      // Moment
      Mx += Fy * Pz - Py * Fz;
      My += Fz * Px - Pz * Fx;
      Mz += Fx * Py - Px * Fy;
      // In case the threshold is activated, set the residuals in consequence
      res = (Fz.abs() < threshold).select(-1.0, res);
    }
    return true;
  };
  
  bool ForcePlate::transformToGlobal(TimeSequence* w)
  {
    auto W = math::to_wrench(w);
    Eigen::Map<const Eigen::Matrix<double, 3, 3>> R(this->referenceFrame(),  3,3);
    Eigen::Map<const Eigen::Array<double, 1, 3>> t(this->referenceFrame()+9,1,3);
    // Forces rotation
    W.values().block(0,0,W.rows(),3).matrix() *= R.transpose();
    // Moments rotation
    W.values().block(0,3,W.rows(),3).matrix() *= R.transpose();
    // Position rotation
    W.values().block(0,6,W.rows(),3).matrix() *= R.transpose();
    // Position translation
    W.values().block(0,6,W.rows(),3) += t.replicate(W.rows(),1);
    return true;
  }
};
};
