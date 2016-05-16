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
  ForcePlatePrivate::ForcePlatePrivate(ForcePlate* pint, const std::string& name, int type, std::vector<std::string>&& labels, int rows, int cols)
  : HardwarePrivate(pint, name, std::move(labels)),
    Type(type),
    ReferenceFrame{1.,0.,0.,0.,1.,0.,0.,0.,1.,0.,0.,0.},
    SurfaceCorners{0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.,0.},
    RelativeSurfaceOrigin{0.,0.,0.},
    CalibrationMatrixDimensions{rows,cols},
    CalibrationMatrixData(nullptr),
    LastWrenchComputation(0ul)
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
  void ForcePlate::setGeometry(double rso[3], double sc1[3], double sc2[3], double sc3[3], double sc4[3])
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
  const int* ForcePlate::calibrationMatrixDimensions() const _OPENMA_NOEXCEPT
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
  void ForcePlate::setCalibrationMatrixData(int rows, int cols, double* data)
  {
    auto optr = this->pimpl();
    if ((optr->CalibrationMatrixDimensions[0] != rows) || optr->CalibrationMatrixDimensions[1] != cols)
    {
      error("The dimensions of the given data does not fit with the stored calibration matrix");
      return;
    }
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
};
};
