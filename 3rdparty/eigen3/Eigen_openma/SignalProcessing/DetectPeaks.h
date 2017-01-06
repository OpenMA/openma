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

#ifndef __eigen_openma_DetectPeaks_h
#define __eigen_openma_DetectPeaks_h

#include <Eigen/Core>

namespace Eigen
{
  enum class Edge : char {None = 0x01, Rising = 0x02, Falling = 0x04, Both = Rising | Falling};
  /**
   * @enum Edge
   * Enumerations related to the type of edge of interest when a flat peak is detected.
   */
  /**
   * @var Edge Edge::None
   * No edge is kept.
  */
  /**
   * @var Edge Edge::Rising
   * Only the rising edge is kept.
  */
  /**
   * @var Edge Edge::Falling
   * Only the falling edge is kept.
  */
  /**
   * @var Edge Edge::Both
   * Both edges (rising and falling) are kept.
  */
  
  /**
   * Bitwise and operator for Edge enumeration
   */
  inline Edge operator& (Edge lhs, Edge rhs)
  {
    return static_cast<Edge>(static_cast<int>(lhs) & static_cast<int>(rhs));
  };
  
  /**
   * Detect peaks in data based on their amplitude and other features
   *
   * @param data Data where peaks (valleys) are detected
   * @param mph Detect peaks that are greater than minimum peak height (default = 0).
   * @param mpd detect peaks that are at least separated by minimum peak distance (in
        number of data) (default = 1).
   * @param threshold detect peaks (valleys) that are greater (smaller) than @c threshold in relation to their immediate neighbors (default = 0).
   * @param edge for a flat peak, keep only the rising edge (@c Edge::Rising), only the
        falling edge (@c Edge::Falling), both edges (@c Edge::Both), or don't detect a
        flat peak (@c Edge::None) (default = @c Edge::Rising).
   * @param kpsh keep peaks with same height even if they are closer than @c mpd (default = False).
   * @param valley if set to true, detect valleys (local minima) instead of peaks.
   * 
   * Important, the function can handle NaN's
   *
   * @note This function is largely inspired by the Python function detect_peaks [1]. This function was choosed (compared to others) after the reading of this blog [2]
   *
   * [1] http://nbviewer.jupyter.org/github/demotu/BMC/blob/master/notebooks/DetectPeaks.ipynb
   * [2] https://blog.ytotech.com/2015/11/01/findpeaks-in-python/
   */
  template <typename T>
  inline Array<typename T::Index,Eigen::Dynamic,1> detect_peaks(const T& data, double mph = std::nan(""), int mpd = 1, double threshold = 0., Edge edge = Edge::Rising, bool kpsh = false, bool valley = false)
  {
    static_assert(T::ColsAtCompileTime == 1, "The function accepts only column vector.");
    using Index = typename T::Index;
    Array<Index,Eigen::Dynamic,1> ind;
    if (data.rows() < 3)
      return ind;
    T x = data;
    if (valley)
      x *= -1.0;
    // Find indices of all peaks
    Array<double,Eigen::Dynamic,1> dx = x.tail(x.rows()-1) - x.head(x.rows()-1);
    // Remove NaN's
    std::vector<typename T::Index> indnan_;
    size_t gg  = x.rows() / 2; // gross guess of possible nan
    indnan_.reserve(gg);
    for (Index i = 0 ; i < x.rows() ; ++i)
    {
      if (std::isnan(x.coeff(i)))
        indnan_.push_back(i);
    }
    if (!indnan_.empty())
    {
      for (const auto i : indnan_)
      {
        x.coeffRef(i) = std::numeric_limits<double>::infinity();
      }
      for (Index i = 0 ; i < dx.rows() ; ++i)
      {
        if (std::isnan(dx.coeff(i)))
          dx.coeffRef(i) = std::numeric_limits<double>::infinity();
      }
    }
    // Manage flat peaks
    std::vector<Index> ind_;
    ind_.reserve(gg);
    if ((edge & Edge::None) == Edge::None)
    {
      // np.where((np.hstack((dx, 0)) < 0) & (np.hstack((0, dx)) > 0))
      for (Index i = 0, len = dx.rows()-1 ; i < len ; ++i)
      {
        if ((dx.coeff(i+1) < 0.) && (dx.coeff(i) > 0.))
          ind_.push_back(i);
      }
      
    }
    if ((edge & Edge::Rising) == Edge::Rising)
    {
      // np.where((np.hstack((dx, 0)) <= 0) & (np.hstack((0, dx)) > 0))
      Index last = dx.rows();
      for (Index i = 1 ; i < last ; ++i)
      {
        if ((dx.coeff(i) <= 0.) && (dx.coeff(i-1) > 0.))
          ind_.push_back(i);
      }
      if (dx.coeff(last-1) > 0.)
        ind_.push_back(last);
    }
    if ((edge & Edge::Falling) == Edge::Falling)
    {
      // np.where((np.hstack((dx, 0)) < 0) & (np.hstack((0, dx)) >= 0))
      if (dx.coeff(0) < 0.)
        ind_.insert(ind_.begin(), 0);
      auto it = ind_.begin();
      for (Index i = 1 ; i < dx.rows() ; ++i)
      {
        if ((dx.coeff(i) < 0.) && (dx.coeff(i-1) >= 0.))
        {
          while ((it != ind_.end()) && (*it < i))
            ++it;
          if (*it > i)
            it = ind_.insert(it, i);
          else if (it == ind_.end())
            ind_.push_back(i);
        }
      }
    }
    // NaN's and values close to NaN's cannot be peaks
    if (!ind_.empty() && !indnan_.empty())
    {
      auto begin = ind_.begin();
      auto end = ind_.end();
      for (const auto i : indnan_)
      {
        while ((begin != end) && (*begin < (i-1)))
          ++begin;
        end = std::remove_if(begin, end, [i](const Index a){return ((a >= (i-1)) && (a <= (i+1)));});
      }
      ind_.erase(end, ind_.end());
    }
    // First and last values of x cannot be peaks
    if (!ind_.empty() && (ind_.front() == 0))
      ind_.erase(ind_.begin());
    if (!ind_.empty() && (ind_.back() == (x.rows()-1)))
      ind_.pop_back();
    // Remove peaks < minimum peak height
    if (!ind_.empty() && !std::isnan(mph))
    {
      ind_.erase(
        std::remove_if(ind_.begin(), ind_.end(), [mph, &x](const Index a){return (x[a] < mph);}),
        ind_.end()
      );
    }
    // Remove peaks - neighbors < threshold
    if (!ind_.empty() && (threshold > 0.))
    {
      ind_.erase(
        std::remove_if(ind_.begin(), ind_.end(), [threshold, &x](const Index a){return (std::min( x[a] - x[a-1], x[a] - x[a+1] ) < threshold);}),
        ind_.end()
      );
    }
    // Detect small peaks closer than minimum peak distance
    if (!ind_.empty() && (mpd > 1))
    {
      std::sort(ind_.begin(), ind_.end(), [&x](const Index a, const Index b){return (x[a] > x[b]);});
      std::vector<bool> idel_(ind_.size(), false);
      for (size_t i = 0 ; i < ind_.size() ; ++i)
      {
        if (!idel_[i])
        {
          // Keep peaks with the same height if kpsh is True
          // idel = idel | (ind >= ind[i] - mpd) & (ind <= ind[i] + mpd) \
          //        & (x[ind[i]] > x[ind] if kpsh else True)
          for (size_t j = 0 ; j < idel_.size() ; ++j)
            idel_[j] = idel_[j] | (ind_[j] >= (ind_[i] - mpd)) & (ind_[j] <= (ind_[i] + mpd)) & (!kpsh ? true : x[ind_[i]] > x[ind_[j]]);
          // Keep current peak
          idel_[i] = false;
        }
      }
      // Remove the small peaks and sort back the indices by their occurrence
      auto it = ind_.begin();
      for (size_t i = 0 ; i < idel_.size() ; ++i)
      {
        if (idel_[i])
          it = ind_.erase(it);
        else
          ++it;
      }
      std::sort(ind_.begin(), ind_.end());
    }
    // Generate the output
    ind = Eigen::Map<Array<typename T::Index,Eigen::Dynamic,1>>(ind_.data(),ind_.size(),1);
    return ind;
  };
};

#endif // __eigen_openma_DetectPeaks_h