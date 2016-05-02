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

#include "openma/instrument/forceplatetype5.h"
#include "openma/base/timesequence.h"
#include "openma/math.h"

namespace ma
{
namespace instrument
{

  /**
   * @class
   * 8 channels: FZ1, FZ2, FZ3, FZ4, FX12, FX34, FY14, FY23 + calibration matrix 6 (columns) by 8 (rows)
   *
   */

  /**
   *
   */
  ForcePlateType5::ForcePlateType5(const std::string& name, Node* parent)
  : ForcePlate(*new ForcePlatePrivate(this, name, 5, {{"Fz1","Fz2","Fz3","Fz4","Fx12","Fx34","Fy14","Fy23"}}, 6, 8), parent)
  {};
  
  TimeSequence* ForcePlateType5::wrench(Location loc, bool global, double threshold)
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
      // Compute the force at the FP origin
      auto cFz1  = this->channel("Fz1")->data();
      auto cFz2  = this->channel("Fz2")->data();
      auto cFz3  = this->channel("Fz3")->data();
      auto cFz4  = this->channel("Fz4")->data();
      auto cFx12 = this->channel("Fx12")->data();
      auto cFx34 = this->channel("Fx34")->data();
      auto cFy14 = this->channel("Fy14")->data();
      auto cFy23 = this->channel("Fy23")->data();
      Eigen::Map<Eigen::Array<double,Eigen::Dynamic,10>> W(w->data(),samples,10);
      Eigen::Map<Eigen::Matrix<double,6,8>> X(this->calibrationMatrixData());
      Eigen::Matrix<double,8,1> c;
      auto o = this->relativeSurfaceOrigin();
      for (unsigned i = 0 ; i < samples ; ++i)
      {
        c.coeffRef(0) = cFz1[i];
        c.coeffRef(1) = cFz2[i];
        c.coeffRef(2) = cFz3[i];
        c.coeffRef(3) = cFz4[i];
        c.coeffRef(4) = cFx12[i];
        c.coeffRef(5) = cFx34[i];
        c.coeffRef(6) = cFy14[i];
        c.coeffRef(7) = cFy23[i];
        W.block<1,6>(i,0) = X * c;
      }
      // Adaptation of the moment and position depending of the specified location
      auto Fx = W.col(0);
      auto Fy = W.col(1);
      auto Fz = W.col(2);
      auto Mx = W.col(3);
      auto My = W.col(4);
      auto Mz = W.col(5);
      auto Px = W.col(6);
      auto Py = W.col(7);
      auto Pz = W.col(8);
      auto res = W.col(9);
      if (loc == Location::Origin)
      {
        W.block(0,6,samples,4).setZero();
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
        Pz.setZero();
      }
      else if (loc == Location::PointOfApplication)
      {
        // For explanations of the PWA calculation, see Shimba T. (1984), 
        // "An estimation of center of gravity from force platform data", 
        // Journal of Biomechanics 17(1), 53–60.
        Eigen::Array<double, Eigen::Dynamic, 1> sNF = W.block(0,0,samples,3).square().rowwise().sum().sqrt();
        Px = (Fy * Mz - Fz * My) / sNF - (Fx.square() * My - Fx * (Fy * Mx)) / (sNF * Fz);
        Py = (Fz * Mx - Fx * Mz) / sNF - (Fx * (Fy * My) - Fy.square() * Mx) / (sNF * Fz);
        Pz.setZero();
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
    }
    return w;
  };

  /**
   *
   */
  ForcePlateType5* ForcePlateType5::clone(Node* parent) const
  {
    auto dest = new ForcePlateType5(this->name());
    dest->copy(this);
    dest->addParent(parent);
    return dest;
  };

  /**
   *
   */
  void ForcePlateType5::copy(const Node* source) _OPENMA_NOEXCEPT
  {
    auto src = node_cast<const ForcePlateType5*>(source);
    if (src == nullptr)
      return;
    this->Node::copy(src);
  };
};
};