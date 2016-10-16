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
#include "openma/body/enums.h"
#include "openma/body/model.h"
#include "openma/body/segment.h"
#include "openma/body/inertialparameters.h"
#include "openma/body/referenceframe.h"
#include "openma/base/enums.h"
#include "openma/base/logger.h"

#include <utility> // std::swap
#include <cmath> // std::nan

// NOTE: The sign is related to the direction of the Body inertial Coordinate System (BCS)
static const double _ma_body_dumas_mcconville_young_table_female[13*13] = {
//   mass  |        CoM position       |                      Tensor of inertia                           ¡   Coeff for the r²
//    (%)  |  X (%)    Y (%)    Z (%)  |  rxx (%)  ryy (%)  rzz (%)  rxy (%)      rxz (%)      ryz (%)    |  rxy²    rxz²   ryz²
      6.7,     1.6,    57.5,    0.1,       29.0,    23.0,    30.0,     4.0/*i*/,   1.0     ,    0.0/*i*/,   -1.0,    1.0,  -1.0,  // Head & Neck: CJC to HV ; Origin: CJC (Values from the corrigendum)
     30.4,    -1.6,   -43.6,   -0.6,       29.0,    27.0,    29.0,    22.0     ,   5.0     ,    5.0/*i*/,    1.0,    1.0,  -1.0,  // Torso: CJC to LJC ; Origin: CJC 
      2.2,    -7.3,   -45.4,   -2.8,       33.0,    17.0,    33.0,     3.0     ,   5.0/*i*/,   14.0     ,    1.0,   -1.0,   1.0,  // Arm: SJC to EJC ; Origin: SJC
      1.3,     2.1,   -41.1,    1.9,       26.0,    14.0,    25.0,    10.0     ,   4.0     ,   13.0/*i*/,    1.0,    1.0,  -1.0,  // Forearm: EJC to WJC ; Origin: EJC 
      0.5,     7.7,   -76.8,    4.8,       63.0,    43.0,    58.0,    29.0     ,  23.0     ,   28.0/*i*/,    1.0,    1.0,  -1.0,  // Hand: WJC to midpoint between MH2 and MH5 ; Origin: WJC 
     14.6,    -0.9,   -23.2,    0.2,       91.0,   100.0,    79.0,    34.0/*i*/,   1.0/*i*/,    1.0/*i*/,   -1.0,   -1.0,  -1.0,  // Pelvis: LJC to projection of HJC in sagittal plane ; Origin: LJC 
     14.6,    -7.7,   -37.7,    0.9,       31.0,    19.0,    32.0,     7.0     ,   2.0/*i*/,    7.0/*i*/,    1.0,   -1.0,  -1.0,  // Thigh: HJC to KJC ; Origin: HJC 
      4.5,    -4.9,   -40.4,    3.1,       28.0,    10.0,    28.0,     2.0     ,   1.0     ,    6.0     ,    1.0,    1.0,   1.0,  // Leg: KJC to AJ ; Origin:C KJC 
      1.0,    27.0,   -21.8,    3.9,       17.0,    36.0,    35.0,    10.0/*i*/,   6.0     ,    4.0/*i*/,   -1.0,    1.0,  -1.0,  // Foot: AJC to midpoint between MHI and MHV ; Origin: AJC 
// Alternative length and origin                                                                                               
     30.4,   -41.1,  -117.3,   -1.9,       98.0,    93.0,    98.0,    76.0     ,  16.0     ,   19.0/*i*/,    1.0,    1.0,  -1.0,  // Torso: C7 to SUP ; Origin: SUP 
      0.5,     3.3,   -32.7,    2.1,       27.0,    18.0,    25.0,    12.0     ,  10.0     ,   12.0/*i*/,    1.0,    1.0,  -1.0,  // Hand: WJC to FT3 ; Origin: WJC 
     14.6,   -37.1,    -5.0,    0.1,       41.0,    45.0,    36.0,    15.0/*i*/,   0.0     ,    0.0     ,   -1.0,    1.0,   1.0,  // Pelvis: Midpoint between RASIS to LASIS ; Origin: Middle of RASIS and LASIS
      1.0,    44.3,     4.4,   -2.5,       12.0,    25.0,    25.0,     7.0/*i*/,   5.0     ,    3.0/*i*/,   -1.0,    1.0,  -1.0   // Foot: CAL to TTII ; Origin: CAL
};

static const double _ma_body_dumas_mcconville_young_table_male[13*13] = {
//   mass  |        CoM position       |                      Tensor of inertia                           ¡   Coeff for the r²
//    (%)  |  X (%)    Y (%)    Z (%)  |  rxx (%)  ryy (%)  rzz (%)  rxy (%)      rxz (%)      ryz (%)    |  rxy²    rxz²   ryz²
      6.7,     2.0,    53.6,    0.1,       28.0,    21.0,    30.0,     7.0/*i*/,   2.0/*i*/,    3.0     ,   -1.0,   -1.0,   1.0,  // Head & Neck: CJC to HV ; Origin: CJC (Values from the corrigendum)
     33.3,    -3.6,   -42.0,   -0.2,       27.0,    25.0,    28.0,    18.0     ,   2.0     ,    4.0/*i*/,    1.0,    1.0,  -1.0,  // Torso: CJC to LJC ; Origin: CJC 
      2.4,     1.7,   -45.2,   -2.6,       31.0,    14.0,    32.0,     6.0     ,   5.0     ,    2.0     ,    1.0,    1.0,   1.0,  // Arm: SJC to EJC ; Origin: SJC
      1.7,     1.0,   -41.7,    1.4,       28.0,    11.0,    27.0,     3.0     ,   2.0     ,    8.0/*i*/,    1.0,    1.0,  -1.0,  // Forearm: EJC to WJC ; Origin: EJC 
      0.6,     8.2,   -83.9,    7.4,       61.0,    38.0,    56.0,    22.0     ,  15.0     ,   20.0/*i*/,    1.0,    1.0,  -1.0,  // Hand: WJC to midpoint between MH2 and MH5 ; Origin: WJC 
     14.2,     2.8,   -28.0,   -0.6,      101.0,   106.0,    95.0,    25.0/*i*/,  12.0/*i*/,    8.0/*i*/,   -1.0,   -1.0,  -1.0,  // Pelvis: LJC to projection of HJC in sagittal plane ; Origin: LJC 
     12.3,    -4.1,   -42.9,    3.3,       29.0,    15.0,    30.0,     7.0     ,   2.0/*i*/,    7.0/*i*/,    1.0,   -1.0,  -1.0,  // Thigh: HJC to KJC ; Origin: HJC 
      4.8,    -4.8,   -41.0,    0.7,       28.0,    10.0,    28.0,     4.0/*i*/,   2.0/*i*/,    5.0     ,   -1.0,   -1.0,   1.0,  // Leg: KJC to AJ ; Origin:C KJC 
      1.2,    38.2,   -15.1,    2.6,       17.0,    37.0,    36.0,    13.0     ,   8.0/*i*/,    0.0     ,    1.0,   -1.0,   1.0,  // Foot: AJC to midpoint between MHI and MHV ; Origin: AJC 
// Alternative length and origin                                                                                                  
     33.3,   -45.6,  -112.1,   -0.8,       93.0,    85.0,    96.0,    62.0     ,   7.0     ,   13.0/*i*/,    1.0,    1.0,  -1.0,  // Torso: C7 to SUP ; Origin: SUP 
      0.6,     3.5,   -35.7,    3.2,       26.0,    16.0,    24.0,     9.0     ,   7.0     ,    8.0/*i*/,    1.0,    1.0,  -1.0,  // Hand: WJC to FT3 ; Origin: WJC 
     14.2,   -33.6,   -14.9,   -0.3,       42.0,    44.0,    40.0,    10.0/*i*/,   5.0/*i*/,    3.0/*i*/,   -1.0,   -1.0,  -1.0,  // Pelvis: Midpoint between RASIS to LASIS ; Origin: Middle of RASIS and LASIS
      1.2,    43.6,    -2.5,   -0.7,       11.0,    25.0,    25.0,     9.0     ,   6.0/*i*/,    0.0     ,    1.0,   -1.0,   1.0   // Foot: CAL to TTII ; Origin: CAL 
};

ma::body::InertialParameters* _ma_body_create_dumas_mcconville_young_bsip(const double* table, const std::string& name, double modelMass, const ma::Any& segLengthProp, size_t segIdx, ma::Node* parent)
{
  if (!segLengthProp.isValid())
  {
    ma::error("Missing segment length. Impossible to compute BSIP for the segment '%s'", name.c_str());
    return nullptr;
  }
  double segLength = segLengthProp.cast<double>();
  enum {Mass = 0, CoMX, CoMY, CoMZ, RadiusXX, RadiusYY, RadiusZZ, RadiusXY, RadiusXZ, RadiusYZ, CoeffRadiusXYSquared, CoeffRadiusXZSquared, CoeffRadiusYZSquared};
  auto bsip = new ma::body::InertialParameters(name+".BSIP", parent);
  bsip->setMass(modelMass * table[segIdx*13+Mass] / 100.0);
  // IMPORTANT: COM and Inertia tensor have to be expressed in the BCS. Compared to original data, this means to create a transformation between both reference frames
  //            For the CoM, this means coordinates order has to change: [CoMz_scs, CoMx_scs, CoMy_scs].
  //            For the Inertial tensor, the order will be: [Ixz_scs, Ixx_scs, Ixy_scs, Iyz_scs, Iyx_scs, Iyy_scs, Izz_scs, Izx_scs, Izy_scs]
  double com[3] = {segLength * table[segIdx*13+CoMZ] / 100.0, segLength * table[segIdx*13+CoMX] / 100.0, segLength * table[segIdx*13+CoMY] / 100.0};
  bsip->setCenterOfMass(com);
  double ml2 = bsip->mass() * segLength * segLength;
  double Ixx = ml2 * table[segIdx*13+RadiusXX] * table[segIdx*13+RadiusXX] / 10000.0;
  double Iyy = ml2 * table[segIdx*13+RadiusYY] * table[segIdx*13+RadiusYY] / 10000.0;
  double Izz = ml2 * table[segIdx*13+RadiusZZ] * table[segIdx*13+RadiusZZ] / 10000.0;
  double Ixy = ml2 * table[segIdx*13+RadiusXY] * table[segIdx*13+RadiusXY] * table[segIdx*13+CoeffRadiusXYSquared] / 10000.0;
  double Ixz = ml2 * table[segIdx*13+RadiusXZ] * table[segIdx*13+RadiusXZ] * table[segIdx*13+CoeffRadiusXZSquared] / 10000.0;
  double Iyz = ml2 * table[segIdx*13+RadiusYZ] * table[segIdx*13+RadiusYZ] * table[segIdx*13+CoeffRadiusYZSquared] / 10000.0;
  double inertia[9] = {Ixz, Ixx, Ixy, Iyz, Ixy, Iyy, Izz, Ixz, Iyz};
  bsip->setInertia(inertia);
  return bsip;
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
   * Internally, this method looks for direct descendents of @a inout that are Model object.
   * Then, for each model, all segments stored in ma::body::Model::segments() are extracted.
   * For each segment found, the mass, center of mass and tensor of inertia are computed. They are stored in a
   * ma::body::InertialParameters object attached to the segment and named as the segment plus the suffix ".BSIP".
   * @important Compared to the data published in the original article, CoMs and Inertia tensors are expressed in the Body inertial Coordinate System (BCS) and not in the Segmental Coordinate System (SCS). The reason is to have BSP usable for any model.
   */
  bool DumasMcConvilleYoungTable::generate(Node* inout)
  {
    auto models = inout->findChildren<Model*>({},{},false);
    for (auto& model : models)
    {
      const Any& massProp = model->property("mass");
      if (!massProp.isValid())
      {
        error("Missing mass information for the model '%s'. Impossible to compute the Dumas-McConville-Young BSIPs", model->name().c_str());
        continue;
      }
      const Any& sexProp = model->property("sex");
      if (!sexProp.isValid())
      {
        error("Missing sex information for the model '%s'. Impossible to compute the Dumas-McConville-Young BSIPs", model->name().c_str());
        continue;
      }
      const double modelMass = massProp.cast<double>();
      const Sex modelSex = sexProp.cast<Sex>();
      auto segments = model->segments()->findChildren<Segment*>({},{},false);
      const double* bsiptable = (modelSex == Sex::Male) ? _ma_body_dumas_mcconville_young_table_male : _ma_body_dumas_mcconville_young_table_female;
      for (auto& seg : segments)
      {
        Node* parent = seg->findChild<ReferenceFrame*>(seg->name() + ".BCS");
        if (parent == nullptr)
          parent = seg;
        if (seg->part() == Part::Head)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 0, parent);
        else if (seg->part() == Part::Torso)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 1, parent);
        else if (seg->part() == Part::Arm)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 2, parent);
        else if (seg->part() == Part::Forearm)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 3, parent);
        else if (seg->part() == Part::Hand)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 4, parent);
        else if (seg->part() == Part::Pelvis)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 5, parent);
        else if (seg->part() == Part::Thigh)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 6, parent);
        else if (seg->part() == Part::Shank)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 7, parent);
        else if (seg->part() == Part::Foot)
          _ma_body_create_dumas_mcconville_young_bsip(bsiptable, seg->name(), modelMass, seg->property("length"), 8, parent);
      }
    }
    return true;
  };
};
};