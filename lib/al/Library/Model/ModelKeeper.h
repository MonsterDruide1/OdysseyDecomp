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

class ModelKeeper : public HioNode {
public:
    ModelKeeper();

    virtual ~ModelKeeper();

    void calc(const sead::Matrix34f&, const sead::Vector3f&);
    void initResource();
    void createMatAnimForProgram(s32);

    void update();
    void updateLast();
    sead::Matrix34f* getBaseMtx();

    ModelCtrl* getModelCtrl() const { return mModelCtrl; }
    AnimPlayerSkl* getAnimSkl() const { return mAnimSkl; }
    AnimPlayerMat* getAnimMtp() const { return mAnimMtp; }
    AnimPlayerMat* getAnimMts() const { return mAnimMts; }
    AnimPlayerMat* getAnimMcl() const { return mAnimMcl; }
    AnimPlayerMat* getAnimMat() const { return mAnimMat; }
    AnimPlayerVis* getAnimVis() const { return mAnimVis; }
    AnimPlayerVis* getAnimVisForAction() const { return mAnimVisForAction; }

public:
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
    void* size1;
    bool flag1;
    bool flag2;
    bool flag3;
    bool flag4;
    bool pad[4];
};

}  // namespace al
