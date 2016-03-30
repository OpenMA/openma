/* 
 * Open Source Movement Analysis Library
 * Copyskeletonhelperht (C) 2016, Moveck Solution Inc., all skeletonhelperhts reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *     * Redistributions of source code must retain the above
 *       copyskeletonhelperht notice, this list of conditions and the following
 *       disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyskeletonhelperht notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials
 *       provided with the distribution.
 *     * Neither the name(s) of the copyskeletonhelperht holders nor the names
 *       of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written
 *       permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DINode* trialsRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __openma_body_skeletonhelper_h
#define __openma_body_skeletonhelper_h

#include "openma/body_export.h"
#include "openma/base/node.h"
#include "openma/base/macros.h" // _OPENMA_NOEXCEPT

#include <string>

OPENMA_EXPORT_NODE_CAST_2(ma, body, SkeletonHelper, OPENMA_BODY_EXPORT);

namespace ma
{
  class Subject;
  class Trial;
};

namespace ma
{
namespace body
{
  class LandmarksTranslator;
  class Model;
  
  class SkeletonHelperPrivate;
  
  class OPENMA_BODY_EXPORT SkeletonHelper : public Node
  {
    OPENMA_DECLARE_PIMPL_ACCESSOR(SkeletonHelper)
    OPENMA_DECLARE_NODEID(SkeletonHelper, Node)
    
  public:
    ~SkeletonHelper() _OPENMA_NOEXCEPT;
    
    SkeletonHelper(const SkeletonHelper& ) = delete;
    SkeletonHelper(SkeletonHelper&& ) _OPENMA_NOEXCEPT = delete;
    SkeletonHelper& operator=(const SkeletonHelper& ) = delete;
    SkeletonHelper& operator=(SkeletonHelper&& ) _OPENMA_NOEXCEPT = delete;
    
    virtual bool calibrate(Node* trials, Subject* subject) = 0;
    bool reconstruct(Node* output, Node* trials);
    virtual LandmarksTranslator* defaultLandmarksTranslator() = 0;
        
    virtual void copy(const Node* source) _OPENMA_NOEXCEPT override;
    
  protected:
    SkeletonHelper(const std::string& name, int region = 0, int side = 0, Node* parent = nullptr);
    SkeletonHelper(SkeletonHelperPrivate& pimpl, Node* parent) _OPENMA_NOEXCEPT;
    
    virtual bool setupModel(Model* model) const = 0;
    virtual bool reconstructModel(Model* model, Trial* trial) = 0;
  };
};
};

#endif // __openma_body_skeletonhelper_h