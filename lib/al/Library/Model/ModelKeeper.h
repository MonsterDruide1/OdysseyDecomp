#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class ActorResource;
class AnimPlayerMat;
class AnimPlayerSimple;
class AnimPlayerSkl;
class AnimPlayerVis;
class ModelCtrl;
class DitherAnimator;
class ModelLodCtrl;
class GpuMemAllocator;
class ModelShaderHolder;
class ModelOcclusionCullingDirector;
class ShadowDirector;
class PrepassTriangleCulling;
class Resource;

class ModelKeeper : public HioNode {
public:
    ModelKeeper();

    virtual ~ModelKeeper();

    void initResource(const ActorResource*);
    void createMatAnimForProgram(s32);
    void setDisplayRootJointMtxPtr(const sead::Matrix34f* mtx);
    void setModelLodCtrl(ModelLodCtrl* modelLodCtrl);
    void setDitherAnimator(DitherAnimator* ditherAnimator);
    void initModel(s32, GpuMemAllocator*, ModelShaderHolder*, ModelOcclusionCullingDirector*,
                   ShadowDirector*, PrepassTriangleCulling*);
    void show();
    void hide();
    void update();
    void updateLast();
    void calc(const sead::Matrix34f&, const sead::Vector3f&);
    const sead::Matrix34f* getBaseMtx() const;
    sead::Matrix34f* getWorldMtxPtrByIndex(s32 index) const;
    Resource* getAnimResource() const;
    Resource* getModelResource() const;

    const char* getName() const { return mName; }

    ModelCtrl* getModelCtrl() const { return mModelCtrl; }

    AnimPlayerSkl* getAnimSkl() const { return mAnimSkl; }

    AnimPlayerMat* getAnimMtp() const { return mAnimMtp; }

    AnimPlayerMat* getAnimMts() const { return mAnimMts; }

    AnimPlayerMat* getAnimMcl() const { return mAnimMcl; }

    AnimPlayerMat* getAnimMat() const { return mAnimMat; }

    AnimPlayerVis* getAnimVis() const { return mAnimVis; }

    AnimPlayerVis* getAnimVisForAction() const { return mAnimVisForAction; }

    bool isFixedModel() const { return mIsFixedModel; }

    bool isIgnoreUpdateDrawClipping() const { return mIsIgnoreUpdateDrawClipping; }

    bool isNeedSetBaseMtxAndCalcAnim() const { return mIsNeedSetBaseMtxAndCalcAnim; }

private:
    const char* mName;
    ModelCtrl* mModelCtrl;
    ActorResource* mActorRes;
    AnimPlayerSkl* mAnimSkl;
    AnimPlayerMat* mAnimMtp;
    AnimPlayerMat* mAnimMts;
    AnimPlayerMat* mAnimMcl;
    AnimPlayerMat* mAnimMat;
    AnimPlayerVis* mAnimVisForAction;
    AnimPlayerVis* mAnimVis;
    char gap_58[9];
    bool mIsFixedModel;
    bool mIsIgnoreUpdateDrawClipping;
    bool mIsNeedSetBaseMtxAndCalcAnim;
};

}  // namespace al
