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

namespace ma
{
namespace body
{ 
  SWIG_TYPEMAP_NODE_OUT(ma::body, PluginGait)
  SWIG_CREATE_TEMPLATE_HELPER_2(ma, body, PluginGait, SWIGTYPE)
  
  %nodefaultctor;
  class PluginGait : public SkeletonHelper
  {
  public:
    SWIG_EXTEND_CAST_CONSTRUCTOR(ma::body, PluginGait, SWIGTYPE)
    SWIG_EXTEND_DEEPCOPY(ma::body, PluginGait)

    PluginGait(int region, int side, Node* parent = nullptr);
    ~PluginGait();
    
    double markerDiameter() const;
    void setMarkerDiameter(double value);
    double rightShoulderOffset() const;
    void setRightShoulderOffset(double value);
    double leftShoulderOffset() const;
    void setLeftShoulderOffset(double value);
    double rightElbowWidth() const;
    void setRightElbowWidth(double value);
    double leftElbowWidth() const;
    void setLeftElbowWidth(double value);
    double rightWristWidth() const;
    void setRightWristWidth(double value);
    double leftWristWidth() const;
    void setLeftWristWidth(double value);
    double rightHandThickness() const;
    void setRightHandThickness(double value);
    double leftHandThickness() const;
    void setLeftHandThickness(double value);
    double interAsisDistance() const;
    void setInterAsisDistance(double value);
    double rightLegLength() const;
    void setRightLegLength(double value);
    double leftLegLength() const;
    void setLeftLegLength(double value);
    double rightAsisTrochanterAPDistance() const;
    void setRightAsisTrochanterAPDistance(double value);
    double leftAsisTrochanterAPDistance() const;
    void setLeftAsisTrochanterAPDistance(double value);
    double rightKneeWidth() const;
    void setRightKneeWidth(double value);
    double leftKneeWidth() const;
    void setLeftKneeWidth(double value);
    double rightAnkleWidth() const;
    void setRightAnkleWidth(double value);
    double leftAnkleWidth() const;
    void setLeftAnkleWidth(double value);
    bool rightFootFlatEnabled() const;
    void setRightFootFlatEnabled(bool value);
    bool leftFootFlatEnabled() const;
    void setLeftFootFlatEnabled(bool value);
    double headOffset() const;
    double rightStaticPlantarFlexionOffset() const;
    double rightStaticRotationOffset() const;
    double leftStaticPlantarFlexionOffset() const;
    double leftStaticRotationOffset() const;
  };
  %clearnodefaultctor;
};
};