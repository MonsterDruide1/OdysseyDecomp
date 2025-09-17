#include "Npc/VolleyballBall.h"

#include "Library/Item/ItemUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/ParabolicPath.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Npc/VolleyballNpc.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(VolleyballBall, Wait)
NERVE_IMPL(VolleyballBall, Attack)
NERVE_END_IMPL(VolleyballBall, OnGround)
NERVE_IMPL(VolleyballBall, Return)
NERVE_IMPL(VolleyballBall, ReturnSmash)
NERVE_IMPL(VolleyballBall, ReturnEnd)
NERVE_IMPL(VolleyballBall, Miss)
NERVE_IMPL(VolleyballBall, MissReaction)
NERVE_IMPL(VolleyballBall, Toss)
NERVE_IMPL(VolleyballBall, TossEnd)
NERVE_IMPL(VolleyballBall, Retry)
NERVE_IMPL(VolleyballBall, RetryEnd)

NERVES_MAKE_NOSTRUCT(VolleyballBall, ReturnEnd, Toss, TossEnd)
NERVES_MAKE_STRUCT(VolleyballBall, Wait, Return, ReturnSmash, Attack, Miss, MissReaction, Retry,
                   OnGround, RetryEnd)
}  // namespace

const sead::Vector3f sCoinDropOffset = {0.0f, 200.0f, 200.0f};

VolleyballBall::VolleyballBall(const char* name) : al::LiveActor(name) {}

void VolleyballBall::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "VolleyballBall", nullptr);
    al::initNerve(this, &NrvVolleyballBall.Wait, 0);
    mAttackPath = new al::ParabolicPath();
    mReturnPath = new al::ParabolicPath();
    al::initJointControllerKeeper(this, 1);
    al::initJointGlobalQuatController(this, &mOrientation, "AllRoot");
    makeActorDead();
}

void VolleyballBall::control() {}

bool VolleyballBall::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (rs::isMsgSeedReflect(message)) {
        rs::requestHitReactionToAttacker(message, self, other);
        return true;
    }

    if (isMiss()) {
        if ((rs::isMsgCapAttack(message) || al::isMsgPlayerSpinAttack(message)) && mMissDelay < 1) {
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &NrvVolleyballBall.Retry);
            return true;
        }
        if (rs::isMsgCapAttack(message) || al::isMsgPlayerSpinAttack(message) ||
            rs::checkMsgNpcTrampleReactionAll(message, other, self, false) ||
            rs::isMsgHosuiAttack(message) || rs::isMsgHosuiTrample(message)) {
            rs::requestHitReactionToAttacker(message, self, other);
            if (mMissReactionDelay < 1)
                al::setNerve(this, &NrvVolleyballBall.MissReaction);
            mMissReactionDelay = 3;
            return true;
        }
    }

    if (mIsEndGame)
        return false;

    if (al::isSensorSimple(self))
        return false;

    if (!al::isNerve(this, &NrvVolleyballBall.Attack) &&
        !al::isNerve(this, &NrvVolleyballBall.OnGround) &&
        !al::isNerve(this, &NrvVolleyballBall.Return))
        return false;

    if (al::isNerve(this, &NrvVolleyballBall.Attack) && al::isLessStep(this, 20))
        return false;

    if (al::isMsgPlayerKick(message) || rs::isMsgUpperPunchAll(message) ||
        rs::isMsgCapAttack(message) || rs::isMsgPlayerBallToss(message) ||
        al::isMsgPlayerSpinAttack(message)) {
        if (al::isNerve(this, &NrvVolleyballBall.OnGround) && al::isGreaterEqualStep(this, 15))
            return true;

        if (al::isSensorName(self, "Smash") && !al::isMsgPlayerSpinAttack(message))
            return false;

        if (al::isMsgPlayerSpinAttack(message) &&
            al::getTrans(mNpc).y <= al::getTrans(this).y + -100.0f &&
            al::getTrans(mNpc).y <= al::getSensorPos(other).y + -30.0f) {
            if (!al::isNerve(this, &NrvVolleyballBall.Return))
                mNpc->addSuccessCount();
            al::startHitReactionHitEffect(this, "スマッシュ", other, self);
            al::setNerve(this, &NrvVolleyballBall.ReturnSmash);
            return true;
        }

        if (!al::isNerve(this, &NrvVolleyballBall.Return)) {
            mNpc->addSuccessCount();
            rs::requestHitReactionToAttacker(message, self, other);
            al::startHitReactionHitEffect(this, "リターン", other, self);
            al::setNerve(this, &NrvVolleyballBall.Return);
            return true;
        }
    }
    return false;
}

void VolleyballBall::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvVolleyballBall.Return) ||
        al::isNerve(this, &NrvVolleyballBall.ReturnSmash) ||
        al::isNerve(this, &NrvVolleyballBall.Attack))
        return;

    if (al::isSensorName(self, "Push") && !rs::sendMsgPushToPlayer(other, self))
        al::sendMsgPush(other, self);
}

void VolleyballBall::attack(const sead::Vector3f& startPosition, const sead::Vector3f& endPosition,
                            f32 attackSpeed) {
    al::resetPosition(this, startPosition);
    mEndPosition.set(endPosition);
    mAttackPath->initFromUpVectorAddHeight(startPosition, mEndPosition, sead::Vector3f::ey, 500.0f);
    mPathTime = mAttackPath->calcPathTimeFromAverageSpeed(attackSpeed);
    mAttackSpeed = attackSpeed;
    al::invalidateClipping(this);
    al::setNerve(this, &NrvVolleyballBall.Attack);
    appear();
}

void VolleyballBall::toss(const sead::Vector3f& startPosition, const sead::Vector3f& endPosition) {
    al::resetPosition(this, startPosition);
    mEndPosition.set(endPosition);
    al::invalidateClipping(this);
    al::offCollide(this);
    al::setNerve(this, &Toss);
    appear();
}

void VolleyballBall::reset() {
    if (al::isAlive(this) && !al::isNerve(this, &NrvVolleyballBall.RetryEnd)) {
        al::startHitReaction(this, "消滅");
        kill();
    }
    al::validateClipping(this);
    mIsEndGame = false;
    al::setNerve(this, &NrvVolleyballBall.Wait);
}

bool VolleyballBall::isMiss() const {
    return al::isNerve(this, &NrvVolleyballBall.Miss) ||
           al::isNerve(this, &NrvVolleyballBall.MissReaction);
}

bool VolleyballBall::isReturnEnd() const {
    return al::isNerve(this, &ReturnEnd);
}

bool VolleyballBall::isTossEnd() const {
    return al::isNerve(this, &TossEnd);
}

bool VolleyballBall::isActive() const {
    return !al::isNerve(this, &NrvVolleyballBall.Wait);
}

bool VolleyballBall::isRetry() const {
    return al::isNerve(this, &NrvVolleyballBall.Retry);
}

bool VolleyballBall::isRetryEnd() const {
    return al::isNerve(this, &NrvVolleyballBall.RetryEnd);
}

void VolleyballBall::exeWait() {}

void VolleyballBall::exeAttack() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Reaction");
        al::startHitReactionHitEffect(this, "リターン",
                                      (al::getTrans(this) + al::getTrans(mNpc)) * 0.5f);
    }

    if (al::isActionPlaying(this, "Reaction") && al::isActionEnd(this))
        al::startAction(this, "FlyWait");

    sead::Vector3f prevPos = al::getTrans(this);
    sead::Vector3f newPos = sead::Vector3f::zero;
    mAttackPath->calcPosition(&newPos, al::getNerveStep(this) / (f32)mPathTime);
    al::resetPosition(this, newPos);

    sead::Quatf mOrientationCopy = mOrientation;
    al::rotateQuatRollBall(&mOrientation, mOrientationCopy, prevPos - newPos, al::getGravity(this),
                           90.0f);
    if (al::isGreaterEqualStep(this, mPathTime)) {
        al::resetPosition(this, mEndPosition);
        al::setNerve(this, &NrvVolleyballBall.OnGround);
    }
}

void VolleyballBall::exeOnGround() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::setVelocityJump(this, 10.0f);
        al::startHitReaction(this, "着地");
        al::onCollide(this);
    }

    al::addVelocityToGravity(this, 0.98f);
    al::scaleVelocity(this, 0.98f);

    if (al::isOnGround(this, 0)) {
        mMissDelay = 60;
        al::setNerve(this, &NrvVolleyballBall.Miss);
    }
}

void VolleyballBall::endOnGround() {
    al::setVelocityZero(this);
    al::offCollide(this);
}

void VolleyballBall::exeReturn() {
    if (al::isFirstStep(this)) {
        sead::Vector3f startPosition = mAttackPath->getStart();
        mReturnPath->initFromUpVectorAddHeight(al::getTrans(this), startPosition,
                                               sead::Vector3f::ey, 500.0f);
        al::startAction(this, "Reaction");
    }

    if (al::isActionPlaying(this, "Reaction") && al::isActionEnd(this))
        al::startAction(this, "FlyWait");

    sead::Vector3f prevPos = al::getTrans(this);
    sead::Vector3f newPos = sead::Vector3f::zero;
    mReturnPath->calcPosition(&newPos, al::getNerveStep(this) / (f32)mPathTime);
    al::resetPosition(this, newPos);

    sead::Quatf mOrientationCopy = mOrientation;
    al::rotateQuatRollBall(&mOrientation, mOrientationCopy, prevPos - newPos, al::getGravity(this),
                           90.0f);
    if (al::isGreaterEqualStep(this, mPathTime))
        al::setNerve(this, &ReturnEnd);
}

void VolleyballBall::exeReturnSmash() {
    sead::Vector3f targetVec = al::getTrans(mNpc) - al::getTrans(this);
    if (al::isFirstStep(this)) {
        sead::Vector3f direction = targetVec;
        al::normalize(&direction);
        al::setVelocityToDirection(this, direction, 50.0f);
        al::startAction(this, "Smash");
        al::makeQuatFrontNoSupport(&mOrientation, al::getVelocity(this));
    }

    f32 targetLen = targetVec.length();
    if (al::getVelocity(this).length() > targetLen) {
        s32 smashCoins = al::getRandom(1, 4);
        mLastSmashCoins = (mLastSmashCoins == 3 && smashCoins == 3) ? 1 : smashCoins;
        if (mLastSmashCoins > 0) {
            for (s32 i = mLastSmashCoins; i != 0; i--) {
                sead::Vector3f frontDir = mNpc->getFrontDir();
                al::rotateVectorDegreeY(&frontDir, al::getRandom(-80.0f, 80.0f));
                sead::Vector3f pose = sCoinDropOffset;
                al::multVecPose(&pose, mNpc, pose);
                al::appearItemTiming(mNpc, "スマッシュ", pose, frontDir, nullptr);
            }
        }
        al::resetPosition(this, mAttackPath->getStart());
        mNpc->startSmashReaction();
        al::setNerve(this, &ReturnEnd);
    }
}

void VolleyballBall::exeReturnEnd() {
    al::validateClipping(this);
    kill();
}

void VolleyballBall::exeMiss() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Wait");
        al::setVelocityZero(this);
    }
    mMissDelay--;
}

void VolleyballBall::exeMissReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");
    mMissReactionDelay--;
    mMissDelay--;
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvVolleyballBall.Miss);
}

void VolleyballBall::exeToss() {
    if (al::isFirstStep(this))
        al::setVelocityJump(this, 12.0f);
    al::addVelocityToGravity(this, 0.4f);
    if (al::getVelocity(this).y <= 0.0f && mEndPosition.y >= al::getTrans(this).y) {
        al::setVelocityZero(this);
        al::resetPosition(this, mEndPosition);
        al::setNerve(this, &TossEnd);
    }
}

void VolleyballBall::exeTossEnd() {}

void VolleyballBall::exeRetry() {
    if (al::isFirstStep(this)) {
        sead::Vector3f startPosition = mAttackPath->getStart();
        mReturnPath->initFromUpVectorAddHeight(al::getTrans(this), startPosition,
                                               sead::Vector3f::ey, 500.0f);
        al::startAction(this, "Reaction");
        mPathTime = mReturnPath->calcPathTimeFromAverageSpeed(12.0f);
    }

    if (al::isActionPlaying(this, "Reaction") && al::isActionEnd(this))
        al::startAction(this, "FlyWait");
    sead::Vector3f prevPos = al::getTrans(this);
    sead::Vector3f newPos = sead::Vector3f::zero;
    mReturnPath->calcPosition(&newPos, al::getNerveStep(this) / (f32)mPathTime);
    al::resetPosition(this, newPos);

    sead::Quatf mOrientationCopy = mOrientation;
    al::rotateQuatRollBall(&mOrientation, mOrientationCopy, prevPos - newPos, al::getGravity(this),
                           90.0f);
    if (al::isGreaterEqualStep(this, mPathTime))
        al::setNerve(this, &NrvVolleyballBall.RetryEnd);
}

void VolleyballBall::exeRetryEnd() {}
