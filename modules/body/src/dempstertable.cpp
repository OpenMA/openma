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

#include "openma/body/dempstertable.h"
#include "openma/body/model.h"
#include "openma/body/segment.h"
#include "openma/body/inertialparameters.h"
#include "openma/body/referenceframe.h"
#include "openma/body/enums.h"
#include "openma/base/logger.h"

#include <utility> // std::swap
#include <cmath> // std::nan

// NOTE: The sign is related to the direction of the Body inertial Coordinate System (BCS)
static const double _ma_body_dempster_table[9*3] = {
//   mass      long.    gyration
//            CoM pos  radius (Kcg)
//    (%)      (%)        (%)  
     8.10,     50.0,     49.5,         // 0. Head: C7-T1 to ear canal (Rprox)
     35.5,    -63.0,     std::nan(""), // 1. Thorax & abdomen: C7-T1 to T12-L1 (Rprox). NO GYRATION RADIUS!
     14.2,     89.5,     31.0,         // 2. Pelvis: L4-L5 to trochanter (Rdist)
     2.80,    -43.6,     32.2,         // 3. Upper arm: glenohumeral joint to elbow axis (Rprox)
     1.60,    -43.0,     30.3,         // 4. Forearm: elbow axis to ulnar styloid (Rprox)
     0.60,    -50.6,     29.8,         // 5. Hand: wrist axis to knuckle II third finger (Rprox)
     10.0,    -43.3,     32.3,         // 6. Thigh: greater trochanter to femoral condyles (Rprox)
     4.65,    -43.3,     30.2,         // 7. Shank: femoral condyles to lateral maleolus (Rprox)
     1.45,     50.0,     47.5, };      // 8. Foot: lateral malleolus to head metatarsal II (Rprox)

ma::body::InertialParameters* _ma_body_create_dempster_bsip(const std::string& name, double modelMass, const ma::Any& segLengthProp, size_t segIdx, ma::Node* parent)
{
  if (!segLengthProp.isValid())
  {
    ma::error("Missing segment length. Impossible to compute BSIP for the segment '%s'", name.c_str());
    return nullptr;
  }
  double segLength = segLengthProp.cast<double>();
  enum { Mass = 0, CoM = 1, Radius = 2 };
  auto bsip = new ma::body::InertialParameters(name+".BSIP", parent);
  bsip->setMass(modelMass * _ma_body_dempster_table[segIdx*3+Mass] / 100.0);
  double com[3] = {0.0, 0.0, segLength * _ma_body_dempster_table[segIdx*3+CoM] / 100.0};
  bsip->setCenterOfMass(com);
  double ml2 = bsip->mass() * segLength * segLength;
  // IMPORTANT: THE MOMENT OF INERTIA HAS TO BE COMPUTED AT THE CENTER OF MASS!
  double I = ml2 * (_ma_body_dempster_table[segIdx*3+Radius] * _ma_body_dempster_table[segIdx*3+Radius]) / 10000.0;
  double inertia[9] = {I,0.,0.,0.,I,0.,0.,0.,0.};
  bsip->setInertia(inertia);
  return bsip;
}

OPENMA_INSTANCE_STATIC_TYPEID(ma::body::DempsterTable);

namespace ma
{
namespace body
{
  /**
   * @class DempsterTable "openma/body/dempstertable.h"
   * @brief Table of body segment inertial parameters based on Dempster's studies
   * The computation of the mass, center of mass, and inertia tensor is based on linear regression equations as provided in
   *  - David A. Winter. (2009). <em>Biomechanics and Motor Control of Human Movement</em>, Fourth Edition. Published by John Wiley & Sons, New York. ISBN 978-0-470-39818-0.
   *
   * It is important to notice that:
   *  - The mass of the subject is required.
   *  - The length of each segment is required.
   *  - This table uses the same parameters for male and female subjects. 
   *  - The segments are modelised as rods. Thus, the tensor of inertia has non-null values only for the elements Ixx and Iyy.
   *  - The body inertial reference frame is assumed to be defined as:
   *     - u: Pointing to the right direction (looking from the back)
   *     - v: Pointing forward
   *     - w: Pointing upward
   *     - o: Selected endpoint as defined in the table below
   *
   * The following segments are defined in the table below. The length of each segment should be defined by the given endpoints.
   * Segment |      Endpoints    | Orgin 
   *         | Proximal | Distal |
   * --------| -------- | ------ | ----- 
   * Head | C7-T1 | ear canal | C7-T1
   * Torso (Thorax & abdomen) | C7-T1 | T12-L1 | C7-T1
   * Pelvis | L4-L5 | trochanter | trochanter
   * Arm (Upper arm) | glenohumeral joint | elbow axis | glenohumeral joint
   * Forearm | elbow axis | ulnar styloid | elbow axis
   * Hand:| wrist axis | knuckle II third finger | wrist axis
   * Thigh | greater trochanter | femoral condyles | greater trochanter
   * Shank | femoral condyles | lateral maleolus | femoral condyles
   * Foot | lateral malleolus | head metatarsal II | lateral malleolus
   *
   * An error message will be displayed for any other segments
   *
   * @important There is no gyration radius for the segment "thorax & abdomen"!
   *
   * @note The values use comes from the following sources:
   *  1. http://health.uottawa.ca/biomech/csb/Archives/dempster.pdf
   *  2. http://www.irc-web.co.jp/vicon_web/news_bn/PIGManualver1.pdf
   * The second source was used only for:
   *  - The center of mass of the head
   *  - The radius of gyration of the pelvis
   *
   * @author Fabien Leboeuf
   * @author Arnaud Barré
   */

  /**
   * Constructor
   */
  DempsterTable::DempsterTable(Node* parent)
  : InertialParametersEstimator("DempsterTable", parent)
  {};
  
  /**
   * Destructor (default)
   */
  DempsterTable::~DempsterTable() _OPENMA_NOEXCEPT = default;
  
  /**
   * Generate the body segment inertial parameters for each model passed. 
   * Internally, this method looks for direct descendents of @a inout that are Model object.
   * Then, for each model, all segments stored in ma::body::Model::segments() are extracted.
   * For each segment found, the mass, center of mass and tensor of inertia are computed. They are stored in a
   * ma::body::InertialParameters object attached to the segment and named as the segment plus the suffix ".BSIP".
   */
  bool DempsterTable::run(Node* inout)
  {
    auto models = inout->findChildren<Model*>({},{},false);
    for (auto& model : models)
    {
      const Any& massProp = model->property("mass");
      if (!massProp.isValid())
      {
        error("Missing mass information for the model '%s'. Impossible to compute the Dempster BSIPs", model->name().c_str());
        continue;
      }
      const double modelMass = massProp.cast<double>();
      if (std::isnan(modelMass))
      {
        error("Mass information is not a number (NaN) and is invalid for the model '%s'. Impossible to compute the Dempster BSIPs", model->name().c_str());
        continue;
      }
      auto segments = model->segments()->findChildren<Segment*>({},{},false);
      for (auto& seg : segments)
      {
        Node* parent = seg->findChild<ReferenceFrame*>(seg->name() + ".BCS");
        if (parent == nullptr)
          parent = seg;
        if (seg->part() == Part::Head)
          _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 0, parent);
        else if (seg->part() == Part::Torso)
          _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 1, parent);
        else if (seg->part() == Part::Arm)
          _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 3, parent);
        else if (seg->part() == Part::Forearm)
          _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 4, parent);
        else if (seg->part() == Part::Hand)
          _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 5, parent);
        else if (seg->part() == Part::Pelvis)
          _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 2, parent);
        else if (seg->part() == Part::Thigh)
          _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 6, parent);
        else if (seg->part() == Part::Shank)
          _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 7, parent);
        else if (seg->part() == Part::Foot)
        {
          // The CoM of the foot is along the forward BCS axis.
          auto bsip = _ma_body_create_dempster_bsip(seg->name(), modelMass, seg->property("length"), 8, parent);
          if (bsip != nullptr)
          {  
            const double* tmp = bsip->centerOfMass();
            double com[3] = {tmp[0],tmp[2],tmp[1]};
            bsip->setCenterOfMass(com);
          }
        }
      }
    }
    return true;
  };
};
};