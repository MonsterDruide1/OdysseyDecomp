#include "Boss/BossRaid/BossRaidWheel.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/ParabolicPath.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(BossRaidWheel, Land);
NERVE_IMPL(BossRaidWheel, Run);
NERVES_MAKE_NOSTRUCT(BossRaidWheel, Land, Run);
}  // namespace

BossRaidWheel::BossRaidWheel(const char* name) : al::LiveActor(name) {}

void BossRaidWheel::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "BossRaidWheel", nullptr);
    al::initNerve(this, &Land, 0);
    al::initJointControllerKeeper(this, 2);
    al::initJointLocalXRotator(this, &mRotateAngle, "Rotate");
    al::initJointLocalZRotator(this, &mRollAngle, "JointRoot");
    al::setEffectFollowPosPtr(this, "FallSign", &mFallSignPos);
    al::createAndSetColliderSpecialAndIgnoreOptionalPurpose(this, "BossRaidWheel",
                                                            "NoBossRaidWheel");
    mPath = new al::ParabolicPath();
    makeActorDead();
}

void BossRaidWheel::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    sead::Vector3f sideDir;
    al::calcJointSideDir(&sideDir, this, "Rotate");
    if (al::isHitPlaneSensor(other, self, sideDir, 5.0f))
        al::sendMsgEnemyAttack(other, self);
}

bool BossRaidWheel::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other,
                               al::HitSensor* self) {
    return rs::isMsgPlayerDisregardHomingAttack(msg);
}

void BossRaidWheel::control() {
    mRotateAngle = al::modf(mRotateAngle + mRotateSpeed + 360.0f, 360.0f) + 0.0f;
}

void BossRaidWheel::exeLand() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
        mRotateSpeed = 6.0f;
    }
    if (al::isActionEnd(this))
        al::setNerve(this, &Run);
}

void BossRaidWheel::exeRun() {
    if (al::isFirstStep(this))
        al::startAction(this, "Run");

    f32 turnDeg;
    if (al::isLessStep(this, 120 - mRunOffset))
        turnDeg = al::calcNerveValue(this, 30 - mRunOffset, 90 - mRunOffset, 0.0f, 1.1f);
    else
        turnDeg = al::calcNerveValue(this, 120 - mRunOffset, 180 - mRunOffset, 1.1f, 0.0f);

    sead::Vector3f frontDir;
    al::calcFrontDir(&frontDir, this);

    sead::Vector3f targetPos = al::getPlayerPos(this, 0);
    al::turnQuatFrontToPosDegreeH(this, targetPos, turnDeg);

    sead::Vector3f newFrontDir;
    al::calcFrontDir(&newFrontDir, this);

    f32 angle = al::calcAngleOnPlaneDegree(frontDir, newFrontDir, sead::Vector3f::ey);
    mRollAngle = al::lerpValue(mRollAngle, angle * -16.0f, 0.05f);

    al::addVelocityToFront(this, 3.5f);
    al::addVelocityToGravity(this, 1.0f);
    al::scaleVelocityHV(this, 0.91f, 0.995f);
    al::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (al::isOnGroundNoVelocity(this, 10))
        al::tryStartActionIfNotPlaying(this, "Run");
    else
        al::tryStartActionIfNotPlaying(this, "Fall");

    if (al::isCollidedWall(this)) {
        al::startHitReaction(this, "破壊");
        makeActorDead();
    }

    if (al::isGreaterEqualStep(this, 300))
        makeActorDead();
}

void BossRaidWheel::shotGround(const sead::Vector3f& pos, const sead::Vector3f& front, f32 ratio) {
    mRotateAngle = 0.0f;
    mRollAngle = 0.0f;
    mRotateSpeed = 5.0f;
    mRunOffset = (s32)al::lerpValue(30.0f, 0.0f, ratio);

    sead::Quatf quat;
    al::makeQuatUpFront(&quat, sead::Vector3f::ey, front);
    al::resetQuatPosition(this, quat, pos);
    al::setVelocityZero(this);
    al::setNerve(this, &Land);
    makeActorAlive();
}
