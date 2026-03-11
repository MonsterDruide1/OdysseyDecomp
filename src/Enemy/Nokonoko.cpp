#include "Enemy/Nokonoko.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"

#include "Enemy/EnemyStateSwoon.h"
#include "Player/CapTargetInfo.h"
#include "Util/Hack.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Nokonoko, Wait);
NERVE_IMPL(Nokonoko, Swoon);
NERVE_IMPL(Nokonoko, CaptureSpin);
NERVE_IMPL_(Nokonoko, CaptureSpinCapRethrow, CaptureSpin);
NERVE_IMPL(Nokonoko, Die);
NERVE_IMPL(Nokonoko, CaptureStart);
NERVE_IMPL(Nokonoko, CaptureEnd);
NERVE_IMPL(Nokonoko, CaptureStartEnd);
NERVE_IMPL(Nokonoko, CaptureWait);
NERVE_IMPL_(Nokonoko, CaptureSwimWait, CaptureWait);
NERVE_IMPL(Nokonoko, CaptureSpinStandbyStart);
NERVE_IMPL(Nokonoko, CaptureJumpStart);
NERVE_IMPL_(Nokonoko, CaptureSwim, CaptureJump);
NERVE_IMPL(Nokonoko, CaptureWalk);
NERVE_IMPL_(Nokonoko, CaptureSwimWalk, CaptureWalk);
NERVE_IMPL(Nokonoko, CaptureJump);
NERVE_IMPL(Nokonoko, CaptureJumpEnd);
NERVE_IMPL_(Nokonoko, CaptureSpinDrift, CaptureSpin);
NERVE_IMPL(Nokonoko, CaptureSpinEnd);

NERVES_MAKE_NOSTRUCT(Nokonoko, CaptureStartEnd)
NERVES_MAKE_STRUCT(Nokonoko, Wait, Swoon, CaptureSpin, CaptureSpinCapRethrow, Die, CaptureStart,
                   CaptureEnd, CaptureWait, CaptureSwimWait, CaptureSpinStandbyStart,
                   CaptureJumpStart, CaptureSwim, CaptureWalk, CaptureSwimWalk, CaptureJump,
                   CaptureJumpEnd, CaptureSpinDrift, CaptureSpinEnd);
}  // namespace

const sead::Vector3f sShellLockOnOffset = {0.0f, 72.0f, 37.0f};
const sead::Vector3f sShellLockOnRotation = {-18.0f, 0.0f, 0.0f};

const sead::Vector3f sShellSpinLockOnOffset = {0.0f, 100.0f, 0.0f};
const sead::Vector3f sShellSpinLockOnRotation = {0.0f, 0.0f, 0.0f};

Nokonoko::Nokonoko(const char* name) : al::LiveActor(name) {}

void Nokonoko::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &NrvNokonoko.Wait, 2);

    al::tryGetArg((s32*)&mMoveType, info, "MoveType");

    if (al::isExistRail(info, "Rail"))
        al::setSyncRailToNearestPos(this);

    mWaterSurfaceFinder = new al::WaterSurfaceFinder(this);
    mCapTargetInfo = rs::createCapTargetInfo(this, nullptr);
    mStateSwoon = new EnemyStateSwoon(this, "SwoonStart", "Swoon", "SwoonEnd", false, false);

    al::initNerveState(this, mStateSwoon, &NrvNokonoko.Swoon, "気絶");
    makeActorAlive();
}

void Nokonoko::control() {}

void Nokonoko::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isNerve(this, &NrvNokonoko.Wait)) {
        if (al::isSensorName(self, "Attack")) {
            al::sendMsgEnemyAttack(other, self);
            return;
        }
    } else if (al::isNerve(this, &NrvNokonoko.Swoon) && al::isSensorEnemyBody(self)) {
        al::sendMsgPushAndKillVelocityToTarget(this, self, other);
    }

    if (!mHackActor)
        return;

    if (al::isSensorEnemyBody(self)) {
        if (rs::sendMsgHackerNoReaction(mHackActor, other, self))
            return;
        if (rs::sendMsgHackAttackMapObj(other, self))
            return;
    }

    if (al::isNerve(this, &NrvNokonoko.CaptureSpin) ||
        al::isNerve(this, &NrvNokonoko.CaptureSpinCapRethrow)) {
        rs::sendMsgCapItemGet(other, self);

        if (al::isSensorName(self, "AttackSpin")) {
            if (al::isSensorEnemyBody(other)) {
                if (rs::sendMsgHackAttack(other, self))
                    return;
                if (al::getVelocity(this).y < -1.0f &&
                    al::sendMsgPlayerAttackTrample(other, self, nullptr))
                    return;
            }
            if (al::isSensorMapObj(other))
                rs::sendMsgHackUpperPunch(other, self);
        }
    }
}

bool Nokonoko::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (rs::tryReceiveMsgInitCapTargetAndSetCapTargetInfo(message, mCapTargetInfo))
        return true;

    if (rs::isMsgHackMarioDead(message)) {
        endCapture();
        al::setNerve(this, &NrvNokonoko.Die);
        return true;
    }

    if (rs::isMsgHackMarioInWater(message))
        return true;

    if (al::isNerve(this, &NrvNokonoko.Wait)) {
        if (rs::isMsgHackAttack(message) || al::isMsgPlayerTrample(message) ||
            rs::isMsgPlayerAndCapObjHipDropAll(message)) {
            rs::requestHitReactionToAttacker(message, self, other);
            al::setNerve(this, &NrvNokonoko.Swoon);
            return true;
        }
        if (rs::isMsgCapEnableLockOn(message))
            return true;

        if (rs::isMsgStartHack(message)) {
            al::invalidateClipping(this);
            mHackActor = rs::startHack(self, other, nullptr);
            rs::startHackStartDemo(mHackActor, this);
            al::setNerve(this, &NrvNokonoko.CaptureStart);
            return true;
        }

        return false;
    }

    if (al::isNerve(this, &NrvNokonoko.Swoon)) {
        if (mStateSwoon->tryReceiveMsgEnableLockOn(message))
            return true;
        if (mStateSwoon->tryReceiveMsgEndSwoon(message))
            return true;
        if (mStateSwoon->tryReceiveMsgStartHack(message)) {
            al::invalidateClipping(this);
            mHackActor = rs::startHack(self, other, nullptr);
            rs::startHackStartDemo(mHackActor, this);
            al::setNerve(this, &NrvNokonoko.CaptureStart);
            return true;
        }
        return false;
    }

    if (al::isNerve(this, &NrvNokonoko.CaptureStart))
        return false;

    if (al::isNerve(this, &NrvNokonoko.CaptureWait) ||
        al::isNerve(this, &NrvNokonoko.CaptureSwimWait) ||
        al::isNerve(this, &NrvNokonoko.CaptureWalk) ||
        al::isNerve(this, &NrvNokonoko.CaptureSwimWalk) ||
        al::isNerve(this, &NrvNokonoko.CaptureJumpStart) ||
        al::isNerve(this, &NrvNokonoko.CaptureJump) ||
        al::isNerve(this, &NrvNokonoko.CaptureSwim) ||
        al::isNerve(this, &NrvNokonoko.CaptureJumpEnd) ||
        al::isNerve(this, &NrvNokonoko.CaptureSpinStandbyStart) ||
        al::isNerve(this, &NrvNokonoko.CaptureSpin) ||
        al::isNerve(this, &NrvNokonoko.CaptureSpinDrift) ||
        al::isNerve(this, &NrvNokonoko.CaptureSpinCapRethrow) ||
        al::isNerve(this, &NrvNokonoko.CaptureSpinEnd)) {
        if ((al::isNerve(this, &NrvNokonoko.CaptureSpin) ||
             al::isNerve(this, &NrvNokonoko.CaptureSpinCapRethrow)) &&
            rs::isMsgCapRethrow(message)) {
            rs::tryGetCapRethrowPos(al::getTransPtr(this), message);
            sead::Vector3f rethrowFront = sead::Vector3f(0.0f, 0.0f, 0.0f);
            if (rs::tryGetCapRethrowFront(&rethrowFront, message)) {
                al::makeQuatFrontUp(al::getQuatPtr(this), rethrowFront, sead::Vector3f::ey);
                al::setVelocity(this, rethrowFront * 80.0f);
                al::setNerve(this, &NrvNokonoko.CaptureSpinCapRethrow);
                return true;
            }
        }

        if (rs::isMsgCancelHack(message) ||
            ((al::isMsgEnemyAttack(message) || al::isMsgExplosion(message)) &&
             al::isSensorEnemyBody(self))) {
            endCapture();
            al::setNerve(this, &NrvNokonoko.CaptureEnd);
            return true;
        }
    }
    return false;
}

void Nokonoko::endCapture() {
    rs::endHack(&mHackActor);
    mCapTargetInfo->setFollowLockOnMtx("Head", sShellLockOnOffset, sShellLockOnRotation);
    al::validateClipping(this);
}

void updateNokonokoVelocity(al::LiveActor* self, al::WaterSurfaceFinder* waterSurfaceFinder) {
    if (!waterSurfaceFinder || !waterSurfaceFinder->isFoundSurface()) {
        if (al::isInWater(self)) {
            al::addVelocityToGravity(self, 0.1f);
            if (al::isNerve(self, &NrvNokonoko.CaptureSpin) ||
                al::isNerve(self, &NrvNokonoko.CaptureSpinDrift))
                al::addVelocityToGravity(self, -0.4f);
        } else {
            al::addVelocityToGravity(
                self, al::isNerve(self, &NrvNokonoko.CaptureSpinCapRethrow) ? 0.0f : 0.9f);
        }
    }
    if (waterSurfaceFinder && waterSurfaceFinder->isFoundSurface()) {
        al::scaleVelocity(self, 0.98f);
        return;
    }

    if (al::isInWater(self)) {
        al::scaleVelocity(self, al::isOnGround(self, 0) ? 0.9f : 0.98f);
        return;
    }

    if (al::isOnGround(self, 0)) {
        sead::Vector3f collidedGroundNormal = al::getCollidedGroundNormal(self);
        if (collidedGroundNormal.dot(al::getVelocity(self)) < 0.0f) {
            sead::Vector3f upVel;
            al::parallelizeVec(&upVel, collidedGroundNormal, al::getVelocity(self));
            al::addVelocity(self, collidedGroundNormal * (upVel.length() * 0.9f));
        }

        f32 scale;
        if (al::isNerve(self, &NrvNokonoko.CaptureSpin))
            scale = 0.98f;
        else if (al::isNerve(self, &NrvNokonoko.CaptureSpinDrift) ||
                 al::isNerve(self, &NrvNokonoko.CaptureSpinEnd))
            scale = 0.99f;
        else if (al::isNerve(self, &NrvNokonoko.CaptureSpinStandbyStart))
            scale = 0.95f;
        else
            scale = 0.9f;
        al::scaleVelocity(self, scale);
    } else
        al::scaleVelocity(self, 0.998f);
}

inline bool Nokonoko::updateAccelStick() {
    sead::Vector3f stickAccel;
    if (!rs::addHackActorAccelStick(this, mHackActor, &stickAccel,
                                    al::isOnGround(this, 0) ? 0.2f : 1.2f, sead::Vector3f::ey))
        return false;

    sead::Quatf targetQuat;
    al::makeQuatFrontUp(&targetQuat, stickAccel, sead::Vector3f::ey);
    al::slerpQuat(al::getQuatPtr(this), al::getQuat(this), targetQuat, 0.3f);
    return true;
}

void Nokonoko::exeWait() {
    if (al::isFirstStep(this)) {
        if (al::isExistRail(this))
            al::startAction(this, "Walk");
        else
            al::startAction(this, "Wait");
    }
    if (al::isExistRail(this)) {
        sead::Vector3f moveDirection;
        al::calcRailMoveDir(&moveDirection, this);

        switch (mMoveType) {
        case al::MoveType::Loop:
            al::moveSyncRailLoop(this, 5.0f);
            break;
        case al::MoveType::Turn: {
            sead::Vector3f frontDir;
            al::calcFrontDir(&frontDir, this);
            if (al::isParallelDirection(moveDirection, sead::Vector3f::ey, 0.01f) ||
                al::calcAngleDegree(frontDir, moveDirection) < 20.0f)
                al::moveSyncRailTurn(this, 5.0f);
            break;
        }
        default:
            al::moveSyncRail(this, 5.0f);
            break;
        }

        if (!al::isParallelDirection(moveDirection, sead::Vector3f::ey, 0.01f))
            al::turnToRailDir(this, al::calcNerveRate(this, 60) * 5.0f);
    } else
        updateNokonokoVelocity(this, nullptr);
}

void Nokonoko::exeSwoon() {
    if (al::updateNerveState(this))
        al::setNerve(this, &NrvNokonoko.Wait);
    else
        updateNokonokoVelocity(this, nullptr);
}

void Nokonoko::exeCaptureStart() {
    if (rs::isHackStartDemoEnterMario(mHackActor))
        al::setNerve(this, &CaptureStartEnd);
}

void Nokonoko::exeCaptureStartEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "HackStart");
    if (al::isActionEnd(this)) {
        rs::endHackStartDemo(mHackActor, this);
        al::setNerve(this, &NrvNokonoko.CaptureWait);
    }
}

void Nokonoko::exeCaptureEnd() {
    if (al::isFirstStep(this))
        al::setVelocityZero(this);
    if (al::isGreaterStep(this, 10))
        al::setNerve(this, &NrvNokonoko.Swoon);
}

void Nokonoko::exeCaptureWait() {
    if (al::isFirstStep(this))
        al::startAction(this, al::isNerve(this, &NrvNokonoko.CaptureWait) ? "Wait" : "SwimWait");

    if (al::isNerve(this, &NrvNokonoko.CaptureWait)) {
        if (al::isInWater(this)) {
            al::setNerve(this, &NrvNokonoko.CaptureSwimWait);
            return;
        }
    } else if (al::isNerve(this, &NrvNokonoko.CaptureSwimWait) && !al::isInWater(this)) {
        al::setNerve(this, &NrvNokonoko.CaptureWait);
        return;
    }

    updateNokonokoVelocity(this, nullptr);

    if (rs::isHoldHackAction(mHackActor)) {
        al::setNerve(this, &NrvNokonoko.CaptureSpinStandbyStart);
        return;
    }

    if (rs::isTriggerHackJump(mHackActor)) {
        if (al::isNerve(this, &NrvNokonoko.CaptureWait)) {
            if (al::isOnGround(this, 0)) {
                al::setNerve(this, &NrvNokonoko.CaptureJumpStart);
                return;
            }
        } else if (al::isNerve(this, &NrvNokonoko.CaptureSwimWait) && al::isInWater(this)) {
            al::addVelocityJump(this, 10.0f);
            al::setNerve(this, &NrvNokonoko.CaptureSwim);
            return;
        }
    }

    if (updateAccelStick() && al::isOnGround(this, 0)) {
        al::Nerve* nerve;

        if (al::isNerve(this, &NrvNokonoko.CaptureWait))
            nerve = &NrvNokonoko.CaptureWalk;
        else
            nerve = &NrvNokonoko.CaptureSwimWalk;

        al::setNerve(this, nerve);
    }
}

void Nokonoko::exeCaptureWalk() {
    if (al::isFirstStep(this))
        al::startAction(this, al::isNerve(this, &NrvNokonoko.CaptureWalk) ? "Walk" : "SwimWalk");

    updateNokonokoVelocity(this, nullptr);

    if (al::isNerve(this, &NrvNokonoko.CaptureWalk)) {
        if (al::isInWater(this)) {
            al::setNerve(this, &NrvNokonoko.CaptureSwimWalk);
            return;
        }
    } else if (al::isNerve(this, &NrvNokonoko.CaptureSwimWalk) && !al::isInWater(this)) {
        al::setNerve(this, &NrvNokonoko.CaptureWalk);
        return;
    }

    updateNokonokoVelocity(this, nullptr);

    if (rs::isHoldHackAction(mHackActor)) {
        al::setNerve(this, &NrvNokonoko.CaptureSpinStandbyStart);
        return;
    }

    if (rs::isTriggerHackJump(mHackActor)) {
        if (al::isNerve(this, &NrvNokonoko.CaptureWalk)) {
            if (al::isOnGround(this, 0)) {
                al::setNerve(this, &NrvNokonoko.CaptureJumpStart);
                return;
            }
        } else if (al::isNerve(this, &NrvNokonoko.CaptureSwimWalk)) {
            al::addVelocityJump(this, 10.0f);
            al::setNerve(this, &NrvNokonoko.CaptureSwim);
            return;
        }
    }

    if (!updateAccelStick()) {
        al::Nerve* nerve;

        if (al::isNerve(this, &NrvNokonoko.CaptureWalk))
            nerve = &NrvNokonoko.CaptureWait;
        else
            nerve = &NrvNokonoko.CaptureSwimWait;

        al::setNerve(this, nerve);
    }
}

void Nokonoko::exeCaptureJumpStart() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "JumpStart");
        al::addVelocityJump(this, 10.0f);
    }

    updateNokonokoVelocity(this, nullptr);
    updateAccelStick();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvNokonoko.CaptureJump);
}

void Nokonoko::exeCaptureJump() {
    if (al::isFirstStep(this))
        al::startAction(this, al::isNerve(this, &NrvNokonoko.CaptureJump) ? "Jump" : "Swim");

    updateNokonokoVelocity(this, nullptr);

    if (al::isNerve(this, &NrvNokonoko.CaptureJump)) {
        if (al::isInWater(this)) {
            al::setNerve(this, &NrvNokonoko.CaptureSwim);
            return;
        }
    } else if (al::isNerve(this, &NrvNokonoko.CaptureSwim) && !al::isInWater(this)) {
        al::setNerve(this, &NrvNokonoko.CaptureJump);
        return;
    }

    updateAccelStick();

    if (al::isNerve(this, &NrvNokonoko.CaptureJump)) {
        if (al::isOnGround(this, 0))
            al::setNerve(this, &NrvNokonoko.CaptureJumpEnd);
    }

    else if (al::isNerve(this, &NrvNokonoko.CaptureSwim)) {
        if (rs::isTriggerHackJump(mHackActor)) {
            al::addVelocityJump(this, 10.0f);
            al::setNerve(this, &NrvNokonoko.CaptureSwim);
            return;
        }

        if (al::isOnGround(this, 0) || al::isActionEnd(this))
            al::setNerve(this, &NrvNokonoko.CaptureSwimWait);
    }
}

void Nokonoko::exeCaptureJumpEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "JumpEnd");

    updateNokonokoVelocity(this, nullptr);
    updateAccelStick();

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvNokonoko.CaptureWait);
}

void Nokonoko::exeCaptureSpinStandbyStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "SpinStandbyStart");

    updateNokonokoVelocity(this, nullptr);

    if (al::isActionEnd(this)) {
        sead::Vector3f hackerMoveDir;
        if (rs::calcHackerMoveDir(&hackerMoveDir, mHackActor, sead::Vector3f::ey))
            al::makeQuatFrontUp(al::getQuatPtr(this), hackerMoveDir, sead::Vector3f::ey);

        mCapTargetInfo->setFollowLockOnMtx("Shell", sShellSpinLockOnOffset,
                                           sShellSpinLockOnRotation);
        al::setNerve(this, &NrvNokonoko.CaptureSpin);
    }
}

void Nokonoko::exeCaptureSpin() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "Spin");
        mHackSwingCooldown = 0;
    }

    mWaterSurfaceFinder->update(al::getTrans(this), -al::getGravity(this), 50.0f);
    updateNokonokoVelocity(this, mWaterSurfaceFinder);

    if (mWaterSurfaceSpringCooldown)
        mWaterSurfaceSpringCooldown--;
    else
        al::approachWaterSurfaceSpringDumper(this, mWaterSurfaceFinder, -5.0f, 12.0f, 1.0f, 0.05f,
                                             0.9f);

    sead::Vector3f velocityDir;
    al::calcFrontDir(&velocityDir, this);

    if (mHackSwingCooldown)
        mHackSwingCooldown--;
    else if (rs::isTriggerHackSwing(mHackActor)) {
        mHackSwingCooldown = 30;
        al::startHitReaction(this, "振りで加速");
        al::addVelocity(this, velocityDir * 15.0f);
    }

    if ((al::isOnGround(this, 0) || mWaterSurfaceFinder->isFoundSurface()) &&
        rs::isTriggerHackJump(mHackActor)) {
        al::addVelocityJump(this, mWaterSurfaceFinder->isFoundSurface() ? 20.0f : 17.0f);
        if (mWaterSurfaceFinder->isFoundSurface())
            mWaterSurfaceSpringCooldown = 15;
    }

    if (al::isCollidedWallVelocity(this)) {
        sead::Vector3f collisionNormal = al::isCollidedWallVelocity(this) ?
                                             al::getCollidedWallNormal(this) :
                                             al::getCollidedCeilingNormal(this);
        if (collisionNormal.dot(al::getVelocity(this)) < 0.0f) {
            sead::Vector3f collidedPos = al::isCollidedWall(this) ? al::getCollidedWallPos(this) :
                                                                    al::getCollidedCeilingPos(this);
            al::startHitReactionHitEffect(this, "衝突", collidedPos);
            sead::Vector3f upVel;
            al::parallelizeVec(&upVel, collisionNormal, al::getVelocity(this));
            al::addVelocity(this, upVel.length() * 1.9f * collisionNormal);

            sead::Vector3f frontDir;
            al::calcFrontDir(&frontDir, this);

            f32 dot = collisionNormal.dot(frontDir);
            sead::Vector3f reflection = frontDir - (dot * (2 * collisionNormal));

            if (al::tryNormalizeOrZero(&reflection))
                al::makeQuatFrontUp(al::getQuatPtr(this), reflection, sead::Vector3f::ey);
            if (al::isNerve(this, &NrvNokonoko.CaptureSpinDrift)) {
                al::setNerve(this, &NrvNokonoko.CaptureSpin);
                return;
            }
        }
    }

    bool isOnGround = al::isOnGround(this, 0);
    bool isFoundWater = mWaterSurfaceFinder->isFoundSurface();
    f32 frontVel = (isOnGround || isFoundWater) ? 0.9f : 0.2f;

    al::addVelocity(this, velocityDir * frontVel);

    if (al::isOnGround(this, 0) || al::isInWater(this) || mWaterSurfaceFinder->isFoundSurface()) {
        f32 hackMoveStick = rs::getHackMoveStickRaw(mHackActor);
        f32 rotateMaxDegree = al::isNerve(this, &NrvNokonoko.CaptureSpinDrift) ? 5.0f : 1.7f;
        if (al::isInWater(this))
            rotateMaxDegree = 1.6f;

        al::rotateQuatYDirDegree(al::getQuatPtr(this), al::getQuat(this),
                                 -(hackMoveStick * rotateMaxDegree));

        if (al::isNerve(this, &NrvNokonoko.CaptureSpinDrift)) {
            sead::Vector3f frontDir;
            al::calcFrontDir(&frontDir, this);
            if (al::calcAngleDegree(frontDir, mStartSpinDriftFrontDir) > 135.0f) {
                sead::Quatf quat;
                al::makeQuatFrontUp(&quat, mStartSpinDriftFrontDir, sead::Vector3f::ey);
                al::rotateQuatYDirDegree(al::getQuatPtr(this), quat,
                                         al::sign(mStartSpinDriftSideDir.dot(frontDir)) * 135.0f);
            }
        }
    }

    if (!al::isNerve(this, &NrvNokonoko.CaptureSpin)) {
        if (al::isNerve(this, &NrvNokonoko.CaptureSpinDrift)) {
            if (al::isGreaterStep(this, 90) || !rs::isHoldHackJump(mHackActor))
                al::setNerve(this, &NrvNokonoko.CaptureSpin);
        } else if (al::isNerve(this, &NrvNokonoko.CaptureSpinCapRethrow) &&
                   al::isGreaterStep(this, 15))
            al::setNerve(this, &NrvNokonoko.CaptureSpin);
        return;
    }

    if (!rs::isHoldHackAction(mHackActor)) {
        al::setNerve(this, &NrvNokonoko.CaptureSpinEnd);
        return;
    }

    if (al::isOnGround(this, 0) && rs::isHoldHackJump(mHackActor) &&
        !rs::isTriggerHackJump(mHackActor)) {
        sead::Vector3f moveVec;
        rs::calcHackerMoveVec(&moveVec, mHackActor, sead::Vector3f::ey);
        if (moveVec.length() > 0.5f) {
            al::startHitReaction(this, "ドリフト開始");
            al::calcSideDir(&mStartSpinDriftSideDir, this);
            al::calcFrontDir(&mStartSpinDriftFrontDir, this);

            al::setNerve(this, &NrvNokonoko.CaptureSpinDrift);
        }
    }
}

void Nokonoko::exeCaptureSpinEnd() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "SpinEnd");
        mCapTargetInfo->setFollowLockOnMtx("Head", sShellLockOnOffset, sShellLockOnRotation);
    }

    updateNokonokoVelocity(this, nullptr);

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvNokonoko.CaptureWait);
}

void Nokonoko::exeDie() {
    al::setVelocityZero(this);
    al::startHitReaction(this, "死亡");
    kill();
}
