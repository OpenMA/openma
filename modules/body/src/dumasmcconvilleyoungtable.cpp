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
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
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

#include "openma/body/dumasmcconvilleyoungtable.h"
#include "openma/body/model.h"
#include "openma/body/segment.h"
#include "openma/body/inertialparameters.h"
#include "openma/body/referenceframe.h"
#include "openma/base/enums.h"
#include "openma/base/logger.h"

#include <utility> // std::swap
#include <cmath> // std::nan

// NOTE: The sign is related to the direction of the Body inertial Coordinate System (BCS)
static const double _ma_body_dumas_mcconville_young_table_male[] = {
};

static const double _ma_body_dumas_mcconville_young_table_female[] = {
};

ma::body::InertialParameters* _ma_body_create_dumas_mcconville_young_bsip(ma::Sex sex, const std::string& name, double modelMass, const ma::Any& segLengthProp, size_t segIdx, ma::Node* parent)
{
  return nullptr;
};

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::DumasMcConvilleYoungTable);

namespace ma
{
namespace body
{
  /**
   * @class DumasMcConvilleYoungTable "openma/body/dumasmcconvilleyoungtable.h"
   * @brief Table of body segment inertial parameters based on the study of Dumas et al. (2007)
   * @author Arnaud Barré
   */

  /**
   * Constructor
   */
  DumasMcConvilleYoungTable::DumasMcConvilleYoungTable(Node* parent)
  : InertialParametersEstimator("DumasMcConvilleYoungTable", parent)
  {};
  
  /**
   * Destructor (default)
   */
  DumasMcConvilleYoungTable::~DumasMcConvilleYoungTable() _OPENMA_NOEXCEPT = default;
  
  /**
   * Generate the body segment inertial parameters for each model passed. 
   * Internally, this method look for direct descendents of @a inout that are Model object.
   * Then, for each model, all segments stored in ma::body::Model::segments() are extrected.
   * For each segment found, the mass, center of mass and tensor of inertia are computed. They are stored in a
   * ma::body::InertialParameters object attached to the segment and named as the segment plus the suffix ".BSIP".
   */
  bool DumasMcConvilleYoungTable::generate(Node* inout)
  {
    return true;
  };
};
};