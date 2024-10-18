#include "Enemy/SenobiLeaf.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Math/MathRandomUtil.h"
#include "Library/Math/VectorUtil.h"

SenobiLeaf::SenobiLeaf(const char* actorName) : al::LiveActor(actorName) {}

void SenobiLeaf::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "SenobiLeaf", 0);
    al::initJointControllerKeeper(this, 1);
    al::initJointLocalZRotator(this, &mLocalZRotator, "Leaf1");
    makeActorDead();
}

void SenobiLeaf::calcAnim() {
    updatePose();
    al::LiveActor::calcAnim();
}

void SenobiLeaf::updatePose() {
    sead::Vector3f frontDir;
    al::calcJointFrontDir(&frontDir, mHostActor, "AllRoot");

    sead::Quatf targetQuat = sead::Quatf::unit;
    al::makeQuatFrontUp(&targetQuat, mUp, frontDir);
    al::rotateQuatRadian(&targetQuat, targetQuat, mUp, sead::Mathf::deg2rad(mYDegree));
    al::updatePoseQuat(this, targetQuat);

    sead::Vector3f newFrontDir;
    al::calcQuatFront(&newFrontDir, targetQuat);
    al::updatePoseTrans(this, al::getTrans(this) - newFrontDir * 15.0f);
}

// NON_MATCHING: regswap when adding
void SenobiLeaf::registerToHost(al::LiveActor* host, bool flip) {
    mHostActor = host;
    getName();  // unused
    al::invalidateClipping(this);
    al::registerSubActorSyncClipping(host, this);
    al::onSyncHideSubActor(host, this);
    mYDegree = al::getRandom(-60.0f, 60.0f) + (flip ? -90.0f : 90.0f);
}
