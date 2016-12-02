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

#include "openma/processing.h"
#include "openma/base/timesequence.h"
#include "openma/base/logger.h"
#include "openma/math.h" // ma::math::prepare_window_processing

#include <iostream> // Used by IIRFilterDesign
#include <Eigen_openma/SignalProcessing/IIRFilterDesign.h>
#include <Eigen_openma/SignalProcessing/FiltFilt.h>

void _ma_processing_butterworth_zero_lag_filter(ma::TimeSequence* ts, const Eigen::Matrix<double, Eigen::Dynamic, 1>& b, const Eigen::Matrix<double, Eigen::Dynamic, 1>& a)
{
  Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>> data(ts->data(), ts->samples(), ts->components());
  data = Eigen::filtfilt(b,a,data);
};

void _ma_processing_butterworth_zero_lag_filter_windowed(ma::TimeSequence* ts, const Eigen::Matrix<double, Eigen::Dynamic, 1>& b, const Eigen::Matrix<double, Eigen::Dynamic, 1>& a)
{
  int cpts = ts->components()-1;
  Eigen::Map<Eigen::Array<double,Eigen::Dynamic,1>> resin(ts->data()+cpts*ts->samples(), ts->samples(), 1);
  Eigen::Array<double,Eigen::Dynamic,1> resout;
  unsigned mwlen = 3 * std::max(a.rows(),b.rows()) - 1;
  std::vector<std::array<unsigned,2>> windows;
  ma::math::prepare_window_processing(resout, windows, resin, mwlen);
  Eigen::Map<Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic>> data(ts->data(), ts->samples(), ts->components());
  Eigen::Matrix<double,Eigen::Dynamic,1> temp = Eigen::Matrix<double,Eigen::Dynamic,1>::Zero(data.rows(),1);
  for (int i = 0 ; i < cpts ; ++i)
  {
    for (const auto& w : windows)
      temp.segment(w[0],w[1]) = Eigen::filtfilt(b,a,data.block(w[0],i,w[1],1));
    data.col(i) = temp;
  }
  resin = resout;
};

namespace ma
{
namespace processing
{
  /**
   * Convenient function to simplify the data filtering of time sequences with a zero-lag Butterworth filter
   * Internally, this function does two filtering passes (one forward followed by one backward). To respect the given final cut-off frequency (@a fc) and the final order (@a fn), these ones are adapted in consequence (see Robertson & Dowling [1] and Winter [2]).
   * This function manage each component independently. In case the type of a time sequence is tagged ma::TimeSequence::Reconstructed, the filtering is managed in consequence and realized by window.
   *
   * @important This is the responsability of the developer to use correctly this function. It is not intended to verify if the passed time sequence(s) is(are) physically adapted to this filter. For example, the filter is adapted for time sequence with the types TimeSequence::Marker and TimeSequence::Analog but might not with the types TimeSequence::Pose and TimeSequence::Wrench. 
   *
   * @par References
   * 1. Robertson D.G., Dowling J.J., <em>Design and responses of Butterworth and critically damped digital filters</em>, Journal of Electromyography and Kinesiology, 2003.
   * 2. Winter D.A., <em>Biomechanics and Motor Control of Human Movement</em>, John Wiley & Sons, Inc., 2009, paragraph 3.4.4.2.
   *
   * @ingroup openma_processing
   */
  bool filter_butterworth_zero_lag(const std::vector<TimeSequence*>& tss, Response type, double fc, int fn)
  {
    if (fc <= 0.0)
    {
      error("Impossible to have a cut-off frequency equal or lower than 0.0.");
      return false;
    }
    if (fn <= 1)
    {
      error("Impossible to have an order equal or lower than 1.");
      return false;
    }
    Eigen::BandType t = Eigen::LowPass;
    switch (type)
    {
    case Response::LowPass:
      break;
    case Response::HighPass:
      t = Eigen::HighPass;
      break;
    case Response::BandPass:
      t = Eigen::BandPass;
      break;
    case Response::BandStop:
      t = Eigen::BandStop;
      break;
    default:
      error("Unknown band type for the Butterworth filter. Filtering aborted.");
      return false;
    }
    for (const auto& ts : tss)
    {
      if (ts->sampleRate() == 0.0)
      {
        error("The time sequence '%s' has a null sample rate.", ts->name().c_str());
        continue;
      }
      double wn = fc / (ts->sampleRate() / 2.0);
      int n = fn;
      Eigen::adjustZeroLagButterworth(n, wn);
      Eigen::Matrix<double, Eigen::Dynamic, 1> a, b;
      Eigen::butter(&b, &a, n, wn, t);
      if ((ts->type() & TimeSequence::Reconstructed) == TimeSequence::Reconstructed)
        _ma_processing_butterworth_zero_lag_filter_windowed(ts,b,a);
      else
        _ma_processing_butterworth_zero_lag_filter(ts,b,a);
    }
    return true;
  };
};
};