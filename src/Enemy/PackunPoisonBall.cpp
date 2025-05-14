#include "Enemy/PackunPoisonBall.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/ParabolicPath.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(PackunPoisonBall, Move)
NERVE_IMPL(PackunPoisonBall, Paint)
NERVE_IMPL(PackunPoisonBall, Fall)

NERVES_MAKE_NOSTRUCT(PackunPoisonBall, Move, Paint, Fall)
}  // namespace

PackunPoisonBall::PackunPoisonBall(al::LiveActor* parent, bool isBig)
    : LiveActor("ポイズンパックンの毒だま"), mParent(parent), mIsBig(isBig),
      mParabolicPath(new al::ParabolicPath()) {
    getName();
}

void PackunPoisonBall::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "PackunPoisonBall", mIsBig ? "Big" : nullptr);
    al::initNerve(this, &Move, 1);
    al::setEffectNamedMtxPtr(this, "CollidedWallMtx", &mEffectCollidedWallMtx);

    makeActorDead();
}

void PackunPoisonBall::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (!al::isSensorEnemyAttack(self))
        return;

    if (al::isNerve(this, &Paint) &&
        rs::sendMsgPaintTexture(other, self,
                                (s32)(al::calcNerveRate(this, 10) * (mIsBig ? 400.0f : 200.0f)),
                                mAngle, 1)) {
        return;
    }

    if (!al::isSensorEnemy(other) && !al::isSensorPlayer(other) && !al::isSensorMapObj(other))
        return;

    if (al::getSensorHost(other) == mParent)
        return;

    if (!mIsHack) {
        if (rs::sendMsgEnemyAttackPoison(other, self)) {
            al::startHitReaction(this, "命中");
            kill();
        }
    } else if (rs::sendMsgHackAttackPoison(other, self)) {
        al::startHitReaction(this, "命中");
        kill();
    }
}

bool PackunPoisonBall::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                  al::HitSensor* self) {
    if (al::isMsgPlayerDisregard(message) || rs::isMsgPlayerDisregardHomingAttack(message))
        return false;

    if (rs::isMsgPlayerDisregardTargetMarker(message))
        return true;

    if (!al::isDead(this) && !al::isNerve(this, &Paint) && rs::isMsgCapAttack(message)) {
        al::startHitReaction(this, "帽子で死亡");
        rs::requestHitReactionToAttacker(message, self, other);
        kill();
    }

    return false;
}

void PackunPoisonBall::killBySwitch() {
    if (al::isDead(this) || al::isNerve(this, &Paint))
        return;

    al::startHitReaction(this, "消滅");
    kill();
}

void PackunPoisonBall::setParam(const sead::Vector3f& trans, const sead::Quatf& quat, bool isHack,
                                f32 maxShootDist, f32 horizontalSpeed, f32 maxHeight) {
    mIsHack = isHack;
    mMaxShootDist = maxShootDist;
    mHorizontalSpeed = horizontalSpeed;
    mMaxHeight = maxHeight;

    al::setTrans(this, trans);
    al::setQuat(this, quat);
}

void PackunPoisonBall::appear() {
    if (al::isAlive(this))
        return;

    makeActorAlive();
    al::showModelIfHide(this);
    al::startHitReaction(this, "出現");
    al::invalidateClipping(this);
    al::onCollide(this);
    al::setNerve(this, &Move);
}

void updateQuat(al::LiveActor* actor, const sead::Vector3f& dir);

void PackunPoisonBall::exeMove() {
    if (al::isFirstStep(this)) {
        sead::Vector3f front = sead::Vector3f::ez;
        al::calcQuatFront(&front, mParent);

        sead::Vector3f side = sead::Vector3f::ex;
        al::calcQuatSide(&side, mParent);

        mTrans.set(al::getTrans(this));

        f32 shootDistOffset = mIsHack ? al::getRandom(-50.0f, 50.0f) : 0.0f;
        f32 sideOffset = mIsHack ? al::getRandom(-100.0f, 100.0f) : 0.0f;
        f32 heightOffset = mIsHack ? al::getRandom(-20.0f, 20.0f) : 0.0f;

        mParabolicPath->initFromUpVector(
            al::getTrans(this),
            al::getTrans(this) + (shootDistOffset + mMaxShootDist) * front + sideOffset * side,
            -al::getGravity(this), heightOffset + mMaxHeight);

        mParabolicPathTime = mParabolicPath->calcPathTimeFromHorizontalSpeed(mHorizontalSpeed);
    }

    f32 rate = al::calcNerveRate(this, -1, mParabolicPathTime);
    mParabolicPath->calcPosition(al::getTransPtr(this), rate);

    if (al::isCollidedWall(this)) {
        al::makeMtxFrontUpPos(&mEffectCollidedWallMtx, al::getCollidedWallNormal(this),
                              sead::Vector3f::ey, al::getCollidedWallPos(this));
        al::startHitReaction(this, "壁で死亡");
        kill();

        return;
    }

    if (al::isCollidedGround(this)) {
        bool isFloorPoison = al::isCollidedFloorCode(this, "Poison");
        al::startHitReaction(this, "着地");

        if (isFloorPoison) {
            kill();
        } else {
            al::deleteEffect(this, "PackunPoisonBallAttack");
            al::setNerve(this, &Paint);
        }

        return;
    }

    if (al::isGreaterEqualStep(this, mParabolicPathTime)) {
        al::setNerve(this, &Fall);

        return;
    }

    sead::Vector3f dir = al::getTrans(this);
    dir -= mTrans;
    updateQuat(this, dir);
    mTrans.set(al::getTrans(this));
}

void updateQuat(al::LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f front = dir;
    if (!al::tryNormalizeOrZero(&front))
        return;

    sead::Quatf newQuat = al::getQuat(actor);
    if (!al::isParallelDirection(front, sead::Vector3f::ey)) {
        al::makeQuatFrontUp(&newQuat, front, sead::Vector3f::ey);
    } else {
        sead::Vector3f side;
        al::calcSideDir(&side, actor);
        if (!al::isParallelDirection(front, side)) {
            al::makeQuatFrontSide(&newQuat, front, -side);
        } else {
            sead::Vector3f up;
            al::calcUpDir(&up, actor);
            if (!al::isParallelDirection(front, up))
                al::makeQuatFrontUp(&newQuat, front, up);
        }
    }

    al::slerpQuat(al::getQuatPtr(actor), al::getQuat(actor), newQuat, 0.9f);
}

void PackunPoisonBall::exeFall() {
    if (al::isFirstStep(this)) {
        sead::Vector3f velocity = al::getTrans(this);
        velocity -= mTrans;
        al::setVelocity(this, velocity);
    }

    al::addVelocityToGravity(this, 0.98f);
    al::scaleVelocity(this, 0.998f);

    if (al::isGreaterStep(this, 250)) {
        al::startHitReaction(this, "消滅");
        kill();

        return;
    }

    if (al::isCollidedFloorCode(this, "Poison")) {
        al::startHitReaction(this, "着地");
        kill();

        return;
    }

    if (!al::isCollided(this)) {
        updateQuat(this, al::getVelocity(this));

        return;
    }

    al::startHitReaction(this, "着地");
    al::deleteEffect(this, "PackunPoisonBallAttack");
    al::setNerve(this, &Paint);
}

void PackunPoisonBall::exePaint() {
    if (al::isFirstStep(this)) {
        al::setVelocityZero(this);
        al::hideModelIfShow(this);
        mAngle = al::getRandomRadian();
    }

    if (al::isGreaterEqualStep(this, 10))
        kill();
}
