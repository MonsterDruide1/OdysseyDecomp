#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Player/IUsePlayerCollision.h"
#include "math/seadVector.h"

class ExternalForceKeeper;
class PlayerExternalVelocity {
public:
    PlayerExternalVelocity(al::LiveActor const*,IUsePlayerCollision const*,sead::Vector3f const*);
    void requestApplyLastGroundInertia();
    void cancelAndFeedbackLastGroundInertia(al::LiveActor *,float,bool);
    void update();
    bool isExistForce() const;
    bool isExistSnapForce() const;
public:
    ExternalForceKeeper *mExternalForceKeeper;
    sead::Vector3f someForce1;
    sead::Vector3f someForce2;
    sead::Vector3f someVec2;
    int pad;
    const al::LiveActor *mActor;
    const IUsePlayerCollision *mCollision;
    const sead::Vector3f *someVec;
    bool someFlag;
    bool gap2[3];
    sead::Vector3f someForce3;
    float someMultipliersDependingOnGroundOrAir[6];
    sead::Vector3f mSnapForce;
    int mApplyLastGroundInertia;
    sead::Vector3f someForceRegardingGround;
    sead::Vector3f someForceRegardingGround2;
};
