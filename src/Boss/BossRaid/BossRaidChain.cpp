#include "Boss/BossRaid/BossRaidChain.h"

#include <math/seadMathCalcCommon.h>
#include <math/seadVector.h>

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(BossRaidChain, Wait);
NERVE_IMPL(BossRaidChain, BlowDown);
NERVE_IMPL(BossRaidChain, Deactive);
NERVE_IMPL(BossRaidChain, Demo);
NERVES_MAKE_NOSTRUCT(BossRaidChain, Wait, BlowDown, Deactive, Demo);
}  // namespace

BossRaidChain::BossRaidChain(const char* name, const char* archiveName, const char* suffix,
                             f32 minDist, f32 maxDist)
    : al::LiveActor(name), mArchiveName(archiveName), mSuffix(suffix), mMinDist(minDist),
      mMaxDist(maxDist) {}

void BossRaidChain::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, mArchiveName, mSuffix);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

void BossRaidChain::setPrevChain(BossRaidChain* chain) {
    mPrevChain = chain;
}

void BossRaidChain::setNextChain(BossRaidChain* chain) {
    mNextChain = chain;
}

void BossRaidChain::setDemoFollowMtxPtr(const sead::Matrix34f* mtxPtr) {
    mDemoFollowMtxPtr = mtxPtr;
}

void BossRaidChain::setFix() {
    mIsFix = true;
}

void BossRaidChain::addVelocityChain(const sead::Vector3f& velocity) {
    if (!mIsFix)
        al::addVelocity(this, velocity);
}

void BossRaidChain::updateVelocity(BossRaidChain* other) {
    if (!other)
        return;

    sead::Vector3f diff = al::getTrans(other) - al::getTrans(this);
    f32 dist = diff.length();

    sead::Vector3f vel = sead::Vector3f::zero;
    if (al::isNearZero(dist)) {
        al::getRandomDir(&vel);
    } else {
        if (dist < mMinDist) {
            f32 scale = (dist - mMinDist) * -0.2f / (dist * mMinDist);
            vel.setScale(diff, scale);
        }
        if (dist > mMaxDist) {
            f32 scale = (dist - mMaxDist) / dist * 0.4f;
            vel.setScale(diff, scale);
        }
    }

    addVelocityChain(vel);
    other->addVelocityChain(-vel);
}

void BossRaidChain::updateDirection(f32 yRate, f32 zRate) {
    sead::Vector3f dir = sead::Vector3f::zero;

    if (mPrevChain)
        dir += al::getTrans(this) - al::getTrans(mPrevChain);

    if (mNextChain)
        dir += al::getTrans(mNextChain) - al::getTrans(this);

    sead::Vector3f up = sead::Vector3f::ey;
    if (mPrevChain)
        al::calcUpDir(&up, mPrevChain);

    sead::Quatf* quat = al::getQuatPtr(this);
    al::turnQuatYDirRate(quat, *quat, up, yRate);

    if (al::tryNormalizeOrZero(&dir)) {
        sead::Quatf* quat2 = al::getQuatPtr(this);
        al::turnQuatZDirRate(quat2, *quat2, dir, zRate);
    }
}

void BossRaidChain::resetDirection() {
    updateDirection(1.0f, 1.0f);
}

void BossRaidChain::startBlowDown() {
    al::setNerve(this, &BlowDown);
}

void BossRaidChain::active() {
    if (al::isNerve(this, &Deactive)) {
        al::setNerve(this, &Wait);
        al::onCalcAnim(this);
    }
}

void BossRaidChain::deactive() {
    if (al::isNerve(this, &Wait)) {
        al::setNerve(this, &Deactive);
        al::setVelocityZero(this);
        al::offCalcAnim(this);
    }
}

void BossRaidChain::setUpDemo() {
    al::setNerve(this, &Demo);
    al::setVelocityZero(this);
}

void BossRaidChain::reset() {
    al::setNerve(this, &Wait);
    al::onCalcAnim(this);
    al::setVelocityZero(this);
    al::resetPosition(this);
    makeActorAlive();
}

void BossRaidChain::exeDeactive() {}

void BossRaidChain::exeDemo() {
    if (mDemoFollowMtxPtr)
        al::updatePoseMtx(this, mDemoFollowMtxPtr);
}

void BossRaidChain::exeWait() {
    updateVelocity(mPrevChain);
    updateVelocity(mNextChain);
    updateDirection(0.15f, 0.4f);
    if (!mIsFix) {
        al::addVelocityToGravity(this, 2.0f);
        al::scaleVelocity(this, 0.9f);
    }
}

void BossRaidChain::exeBlowDown() {
    if (al::isFirstStep(this)) {
        sead::Vector3f randomDir;
        al::getRandomDirH(&randomDir, al::getGravity(this));
        mBlowAxis.setCross(randomDir, al::getGravity(this));
        al::setVelocitySeparateHV(this, randomDir, 25.0f, 35.0f);
    }
    sead::Quatf* quat = al::getQuatPtr(this);
    al::rotateQuatRadian(quat, *quat, mBlowAxis, sead::Mathf::deg2rad(23.0f));
    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.99f);
    if (al::isGreaterEqualStep(this, 180))
        kill();
}
