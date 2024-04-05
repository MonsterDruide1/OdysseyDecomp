#pragma once

#include "Library/HostIO/HioNode.h"

namespace al {
class ActorResource;
class AnimPlayerMat;
class AnimPlayerSimple;
class AnimPlayerSkl;
class AnimPlayerVis;
class ModelCtrl;

class ModelKeeper : public HioNode {
public:
    ModelKeeper();

    virtual ~ModelKeeper();

    void initResource();
    void createMatAnimForProgram(int);

    ModelCtrl* getModelCtrl() { return mModelCtrl; }
    AnimPlayerSkl* getAnimSkl() { return mAnimSkl; }
    AnimPlayerMat* getAnimMtp() { return mAnimMtp; }
    AnimPlayerMat* getAnimMts() { return mAnimMts; }
    AnimPlayerMat* getAnimMcl() { return mAnimMcl; }
    AnimPlayerMat* getAnimMat() { return mAnimMat; }
    AnimPlayerVis* getAnimVis() { return mAnimVis; }
    AnimPlayerVis* getAnimVisForAction() { return mAnimVisForAction; }

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
