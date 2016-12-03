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

#ifndef __openma_body_plugingait_h
#define __openma_body_plugingait_h

#include "openma/body_export.h"
#include "openma/body/skeletonhelper.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

namespace ma
{
namespace body
{
  class PluginGaitPrivate;
  
  class OPENMA_BODY_EXPORT PluginGait : public SkeletonHelper
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(PluginGait)
    OPENMA_DECLARE_NODEID(PluginGait, SkeletonHelper)
    
  public:
    PluginGait(int region, int side, Node* parent = nullptr);
    
    double markerDiameter() const _OPENMA_NOEXCEPT;
    void setMarkerDiameter(double value) _OPENMA_NOEXCEPT;
    bool headOffsetEnabled() const _OPENMA_NOEXCEPT;
    void setHeadOffsetEnabled(bool value) _OPENMA_NOEXCEPT;
    double rightShoulderOffset() const _OPENMA_NOEXCEPT;
    void setRightShoulderOffset(double value) _OPENMA_NOEXCEPT;
    double leftShoulderOffset() const _OPENMA_NOEXCEPT;
    void setLeftShoulderOffset(double value) _OPENMA_NOEXCEPT;
    double rightElbowWidth() const _OPENMA_NOEXCEPT;
    void setRightElbowWidth(double value) _OPENMA_NOEXCEPT;
    double leftElbowWidth() const _OPENMA_NOEXCEPT;
    void setLeftElbowWidth(double value) _OPENMA_NOEXCEPT;
    double rightWristWidth() const _OPENMA_NOEXCEPT;
    void setRightWristWidth(double value) _OPENMA_NOEXCEPT;
    double leftWristWidth() const _OPENMA_NOEXCEPT;
    void setLeftWristWidth(double value) _OPENMA_NOEXCEPT;
    double rightHandThickness() const _OPENMA_NOEXCEPT;
    void setRightHandThickness(double value) _OPENMA_NOEXCEPT;
    double leftHandThickness() const _OPENMA_NOEXCEPT;
    void setLeftHandThickness(double value) _OPENMA_NOEXCEPT;
    double interAsisDistance() const _OPENMA_NOEXCEPT;
    void setInterAsisDistance(double value) _OPENMA_NOEXCEPT;
    double rightLegLength() const _OPENMA_NOEXCEPT;
    void setRightLegLength(double value) _OPENMA_NOEXCEPT;
    double leftLegLength() const _OPENMA_NOEXCEPT;
    void setLeftLegLength(double value) _OPENMA_NOEXCEPT;
    double rightAsisTrochanterAPDistance() const _OPENMA_NOEXCEPT;
    void setRightAsisTrochanterAPDistance(double value) _OPENMA_NOEXCEPT;
    double leftAsisTrochanterAPDistance() const _OPENMA_NOEXCEPT;
    void setLeftAsisTrochanterAPDistance(double value) _OPENMA_NOEXCEPT;
    double rightKneeWidth() const _OPENMA_NOEXCEPT;
    void setRightKneeWidth(double value) _OPENMA_NOEXCEPT;
    double leftKneeWidth() const _OPENMA_NOEXCEPT;
    void setLeftKneeWidth(double value) _OPENMA_NOEXCEPT;
    double rightAnkleWidth() const _OPENMA_NOEXCEPT;
    void setRightAnkleWidth(double value) _OPENMA_NOEXCEPT;
    double leftAnkleWidth() const _OPENMA_NOEXCEPT;
    void setLeftAnkleWidth(double value) _OPENMA_NOEXCEPT;
    bool rightFootFlatEnabled() const _OPENMA_NOEXCEPT;
    void setRightFootFlatEnabled(bool value) _OPENMA_NOEXCEPT;
    bool leftFootFlatEnabled() const _OPENMA_NOEXCEPT;
    void setLeftFootFlatEnabled(bool value) _OPENMA_NOEXCEPT;
    double headOffset() const _OPENMA_NOEXCEPT;
    double rightStaticPlantarFlexionOffset() const _OPENMA_NOEXCEPT;
    double rightStaticRotationOffset() const _OPENMA_NOEXCEPT;
    double leftStaticPlantarFlexionOffset() const _OPENMA_NOEXCEPT;
    double leftStaticRotationOffset() const _OPENMA_NOEXCEPT;
    
    virtual bool calibrate(Node* trials, Subject* subject) override;
    virtual LandmarksTranslator* defaultLandmarksTranslator() override;
    virtual InertialParametersEstimator* defaultInertialParametersEstimator() override;
    virtual ExternalWrenchAssigner* defaultExternalWrenchAssigner() override;
    virtual InverseDynamicProcessor* defaultInverseDynamicProcessor() override;
    
    virtual PluginGait* clone(Node* parent = nullptr) const override;
    virtual void copy(const Node* source) _OPENMA_NOEXCEPT override;
    
  protected:
    virtual bool setupModel(Model* model) const override;
    virtual bool reconstructModel(Model* model, Trial* trial) override;
  };
};
};

OPENMA_EXPORT_STATIC_TYPEID(ma::body::PluginGait, OPENMA_BODY_EXPORT);

#endif // __openma_body_plugingait_h