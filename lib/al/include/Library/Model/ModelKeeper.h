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
    const char* mName;                 // 8
    ModelCtrl* mModelCtrl;             // 10
    ActorResource* mActorRes;          // 18
    AnimPlayerSkl* mAnimSkl;           // 20
    AnimPlayerMat* mAnimMtp;           // 28
    AnimPlayerMat* mAnimMts;           // 30
    AnimPlayerMat* mAnimMcl;           // 38
    AnimPlayerMat* mAnimMat;           // 40
    AnimPlayerVis* mAnimVisForAction;  // 48
    AnimPlayerVis* mAnimVis;           // 50
};

}  // namespace al
