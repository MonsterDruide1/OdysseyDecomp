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

class ModelKeeper : public HioNode {
public:
    ModelKeeper();

    virtual ~ModelKeeper();

    void calc(const sead::Matrix34f&, const sead::Vector3f&);
    void initResource();
    void createMatAnimForProgram(s32);
    void setDitherAnimator(DitherAnimator* ditherAnimator);
    void setDisplayRootJointMtxPtr(const sead::Matrix34f* mtx);
    void setModelLodCtrl(ModelLodCtrl* modelLodCtrl);

    ModelCtrl* getModelCtrl() const { return mModelCtrl; }

    AnimPlayerSkl* getAnimSkl() const { return mAnimSkl; }

    AnimPlayerMat* getAnimMtp() const { return mAnimMtp; }

    AnimPlayerMat* getAnimMts() const { return mAnimMts; }

    AnimPlayerMat* getAnimMcl() const { return mAnimMcl; }

    AnimPlayerMat* getAnimMat() const { return mAnimMat; }

    AnimPlayerVis* getAnimVis() const { return mAnimVis; }

    AnimPlayerVis* getAnimVisForAction() const { return mAnimVisForAction; }

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
};

}  // namespace al
