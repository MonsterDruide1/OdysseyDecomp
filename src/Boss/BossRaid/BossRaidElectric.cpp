#include "Boss/BossRaid/BossRaidElectric.h"

#include <math/seadMathCalcCommon.h>
#include <math/seadVector.h>

#include "Library/Area/AreaObjUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorGroup.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(BossRaidElectric, Wait);
NERVE_IMPL(BossRaidElectric, Disappear);
NERVE_IMPL(BossRaidElectric, Hide);
NERVES_MAKE_NOSTRUCT(BossRaidElectric, Wait, Disappear, Hide);
}  // namespace

BossRaidElectric::BossRaidElectric(const char* name) : al::LiveActor(name) {}

void BossRaidElectric::makeActorDead() {
    al::LiveActor::makeActorDead();
    if (mNextBullet != nullptr) {
        mNextBullet->mPrevBullet = nullptr;
        mNextBullet = nullptr;
    }
    if (mPrevBullet != nullptr) {
        mPrevBullet->mNextBullet = nullptr;
        mPrevBullet = nullptr;
    }
    if (mActorGroup != nullptr) {
        mActorGroup->removeActor(this);
        mActorGroup = nullptr;
    }
}

void BossRaidElectric::setPrevBullet(BossRaidElectric* bullet) {
    mPrevBullet = bullet;
}

void BossRaidElectric::setNextBullet(BossRaidElectric* bullet) {
    mNextBullet = bullet;
}

void BossRaidElectric::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BossRaidElectric", nullptr);
    al::initNerve(this, &Wait, 0);
    al::invalidateClipping(this);
    al::offCollide(this);
    al::initJointControllerKeeper(this, 1);
    al::initJointLocalTransControllerX(this, &_144, "Electric2");
    al::setHitSensorPosPtr(this, "Attack", &_12c);
    al::setSensorRadius(this, "Attack", 50.0f);
    makeActorDead();
}

void BossRaidElectric::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isNerve(this, &Wait))
        return;
    if (al::isHitCylinderSensor(other, _12c, mFrontDir, 50.0f))
        al::sendMsgEnemyAttack(other, self);
}

void BossRaidElectric::shot(const sead::Vector3f& pos, const sead::Vector3f& dir,
                            BossRaidElectric* prevBullet,
                            al::DeriveActorGroup<BossRaidElectric>* actorGroup) {
    al::setModelAlphaMask(this, 1.0f);
    al::showModelIfHide(this);
    al::resetPosition(this, pos);
    mMoveDir.set(dir);
    mNextBullet = prevBullet;
    if (prevBullet != nullptr) {
        al::showModelIfHide(this);
        prevBullet->mPrevBullet = this;
    } else {
        al::hideModelIfShow(this);
    }
    mActorGroup = actorGroup;
    actorGroup->registerActor(this);
    _148 = true;
    al::setNerve(this, &Wait);
    makeActorAlive();
}

void BossRaidElectric::updatePosition() {
    sead::Vector3f* trans = al::getTransPtr(this);
    *trans += mMoveDir;
}

void BossRaidElectric::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, mNextBullet != nullptr ? "Wait" : "Hide");

    updateAnimAndJoint();
    if (_148) {
        if (al::isInAreaObj(this, "BossRaidElectricArea", al::getTrans(this))) {
            if (_148)
                return;
        } else {
            _148 = false;
        }
    }
    if (isAirAll())
        al::setNerve(this, &Disappear);
}

void BossRaidElectric::updateAnimAndJoint() {
    _144 = -500.0f;
    if (mNextBullet == nullptr) {
        _12c = al::getTrans(this);
        al::setSensorRadius(this, "Attack", 50.0f);

        sead::Vector3f scale(1.0f, 1.0f, 1.0f);
        al::setEffectEmitterVolumeScale(this, "Spark", scale);
        al::setEffectParticleScale(this, "Body", scale);

        mFrontDir.set(sead::Vector3f::ez);
        return;
    }

    sead::Vector3f diff = al::getTrans(this) - al::getTrans(mNextBullet);
    f32 length = diff.length();
    if (!al::tryNormalizeOrZero(&mFrontDir, diff))
        mFrontDir.set(sead::Vector3f::ez);

    _144 = length - 500.0f;

    if (sead::Mathf::abs(mFrontDir.y) > 0.98f)
        al::makeQuatFrontUp(al::getQuatPtr(this), mFrontDir, sead::Vector3f::ex);
    else
        al::makeQuatFrontUp(al::getQuatPtr(this), mFrontDir, sead::Vector3f::ey);

    al::lerpVec(&_12c, al::getTrans(this), al::getTrans(mNextBullet), 0.5f);
    al::setSensorRadius(this, "Attack", length * 0.5f + 50.0f);
    updateEffectScale(length);
}

bool BossRaidElectric::isAirAll() const {
    if (_148)
        return false;
    if (mNextBullet && mNextBullet->_148)
        return false;
    return true;
}

void BossRaidElectric::exeDisappear() {
    updateAnimAndJoint();
    f32 alpha = al::calcNerveEaseInOutValue(this, 39, 1.0f, 0.0f);
    al::setModelAlphaMask(this, alpha);

    if (al::isGreaterEqualStep(this, 40)) {
        al::hideModelIfShow(this);
        al::setNerve(this, &Hide);
    }
}

void BossRaidElectric::exeHide() {
    if (!al::isNerve(this, &Hide))
        return;

    BossRaidElectric* next = mNextBullet;
    if (next == nullptr || al::isNerve(next, &Hide)) {
        BossRaidElectric* prev = mPrevBullet;
        if (prev == nullptr) {
            makeActorDead();
            return;
        }
        if (al::isNerve(prev, &Hide))
            makeActorDead();
    }
}

bool BossRaidElectric::isHideAll() const {
    if (!isHide())
        return false;

    BossRaidElectric* next = mNextBullet;
    if (next != nullptr && !next->isHide())
        return false;

    BossRaidElectric* prev = mPrevBullet;
    if (prev != nullptr && !prev->isHide())
        return false;
    return true;
}

bool BossRaidElectric::isHide() const {
    return al::isNerve(this, &Hide);
}

void BossRaidElectric::updateEffectScale(f32 scale) {
    sead::Vector3f v(1.0f, scale / 100.0f, 1.0f);
    al::setEffectEmitterVolumeScale(this, "Spark", v);
    al::setEffectParticleScale(this, "Body", v);
}

void BossRaidElectric::calcNearPos(sead::Vector3f* outPos, const sead::Vector3f& targetPos) const {
    BossRaidElectric* next = mNextBullet;
    const sead::Vector3f& thisTrans = al::getTrans(this);

    if (next != nullptr) {
        const sead::Vector3f& nextTrans = al::getTrans(mNextBullet);
        al::calcClosestSegmentPoint(outPos, thisTrans, nextTrans, targetPos);
    } else {
        outPos->set(thisTrans);
    }
}
