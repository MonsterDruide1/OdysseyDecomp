#include "Enemy/Nokonoko2D.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Item/ItemUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Util/ActorDimensionUtil.h"
#include "Util/ItemUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Nokonoko2D, Walk)
NERVE_IMPL(Nokonoko2D, KouraMove)
NERVE_IMPL(Nokonoko2D, KouraMoveNoTouch)
NERVE_IMPL(Nokonoko2D, Damage)
NERVE_IMPL(Nokonoko2D, DamagePoison)
NERVE_IMPL(Nokonoko2D, KouraStop)
NERVE_IMPL(Nokonoko2D, KouraEnd)
NERVE_IMPL(Nokonoko2D, HideWait)
NERVE_IMPL(Nokonoko2D, AppearWait)

NERVES_MAKE_STRUCT(Nokonoko2D, Walk, KouraMove, KouraMoveNoTouch, Damage, DamagePoison, KouraStop,
                   KouraEnd, HideWait, AppearWait)
}  // namespace

Nokonoko2D::Nokonoko2D(const char* name)
    : al::LiveActor(name), mCollisionPartsFilter{rs::createCollisionPartsFilter2DOnly()} {}

void Nokonoko2D::init(const al::ActorInitInfo& initInfo) {
    using Nokonoko2DFunctor = al::FunctorV0M<Nokonoko2D*, void (Nokonoko2D::*)()>;

    mIsGreen = al::isObjectName(initInfo, "NokonokoGreen2D");
    if (mIsGreen)
        al::initActorWithArchiveName(this, initInfo, "NokonokoGreen2D", nullptr);
    else
        al::initActorWithArchiveName(this, initInfo, "NokonokoRed2D", nullptr);

    al::initNerve(this, &NrvNokonoko2D.Walk, 0);
    rs::createAndSetFilter2DOnly(this);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);

    if (!rs::isIn2DArea(this)) {
        makeActorDead();
        return;
    }

    rs::snap2DGravity(this, this, 500.0f);
    makeActorAlive();
    mLocalZRotator = 0.0f;
    al::initJointControllerKeeper(this, 1);
    al::initJointLocalZRotator(this, &mLocalZRotator, "Center");
    mInitTrans = al::getTrans(this);
    mInitFront = al::getFront(this);

    if (al::isExistLinkChild(initInfo, "Nokonoko2dResetArea", 0)) {
        mResetArea = al::createLinkAreaGroup(this, initInfo, "Nokonoko2dResetArea",
                                             "子供エリアグループ", "子供エリア");
        al::PlacementInfo placementInfo;
        al::getLinksInfo(&placementInfo, initInfo, "Nokonoko2dResetArea");
        al::tryGetArg(&mIsResetForce, placementInfo, "IsResetForce");
    }
    al::tryGetArg(&mClippingTime, initInfo, "ClippingTime");
    al::tryGetArg(&mIsHeavyGravity, initInfo, "IsHeavyGravity");
    al::listenStageSwitchOn(this, "SwitchReset", Nokonoko2DFunctor(this, &Nokonoko2D::reset));
}

void Nokonoko2D::reset() {
    al::validateClipping(this);
    al::offCollide(this);
    al::hideModelIfShow(this);
    al::invalidateHitSensors(this);
    al::setVelocityZero(this);
    al::setTrans(this, mInitTrans);
    al::setFront(this, mInitFront);
    clearVertFlip();
    al::resetPosition(this);
    al::setNerve(this, &NrvNokonoko2D.Walk);
}

void Nokonoko2D::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorEnemyBody(self)) {
        if ((!al::isNerve(this, &NrvNokonoko2D.KouraMove) &&
             !al::isNerve(this, &NrvNokonoko2D.KouraMoveNoTouch)) ||
            al::isSensorPlayer(other)) {
            if (rs::sendMsgPush2D(other, self)) {
                if (al::isFaceToTargetDegreeH(this, al::getSensorPos(other), al::getFront(this),
                                              5.0f)) {
                    al::turnFront(this, 180.0f);
                    if (al::isNerve(this, &NrvNokonoko2D.KouraMove))
                        al::startHitReaction(this, "２Ｄコウラ壁ヒット");
                    al::setVelocityToFront(this, mVelocityFront);
                    al::addVelocityToGravity(this, 0.65f);
                    updateCollider();
                }
            }
        } else {
            if (rs::sendMsgKouraAttack2D(other, self))
                al::startHitReaction(this, "２Ｄコウラヒット");
            rs::sendMsgKouraItemGet2D(other, self);
        }
    }

    if (al::isSensorPlayer(other) && al::isSensorEnemyAttack(self)) {
        if (al::isNerve(this, &NrvNokonoko2D.KouraMove) || al::isNerve(this, &NrvNokonoko2D.Walk))
            rs::sendMsgKouraAttack2D(other, self);
    }
}

bool Nokonoko2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    if (rs::isMsgPlayerDisregardHomingAttack(message) ||
        rs::isMsgPlayerDisregardTargetMarker(message) || al::isMsgPlayerDisregard(message))
        return true;

    if (al::isNerve(this, &NrvNokonoko2D.Damage))
        return false;

    if (rs::isMsgPush2D(message) && al::isNerve(this, &NrvNokonoko2D.Walk)) {
        if (al::isFaceToTargetDegreeH(this, al::getSensorPos(other), al::getFront(this), 5.0f)) {
            al::turnFront(this, 180.0f);
            al::setVelocityToFront(this, mVelocityFront);
            al::addVelocityToGravity(this, 0.65f);
            updateCollider();
        }
        return true;
    }

    if (al::isNerve(this, &NrvNokonoko2D.DamagePoison))
        return false;

    if (rs::isMsgPlayerTrample2D(message) && al::isSensorName(self, "Body")) {
        if (rs::isPlayerOnGround(this))
            return false;

        updateCollider();
        rs::requestHitReactionToAttacker(message, self, other);
        mIsTrampled = true;

        if (!al::isNerve(this, &NrvNokonoko2D.Walk)) {
            if ((al::isNerve(this, &NrvNokonoko2D.KouraStop) && al::isGreaterStep(this, 4)) ||
                al::isNerve(this, &NrvNokonoko2D.KouraEnd)) {
                al::setNerve(this, &NrvNokonoko2D.KouraMoveNoTouch);
                return false;
            }

            if (!al::isNerve(this, &NrvNokonoko2D.KouraMove))
                return false;
        }

        al::setVelocityToFront(this, 0.0f);
        al::addVelocityToGravity(this, 0.65f);
        al::setNerve(this, &NrvNokonoko2D.KouraStop);
        return true;
    }

    if (rs::isMsgItemGet2D(message) && al::isSensorPlayer(other) && al::isSensorEnemyBody(self)) {
        if (al::isNerve(this, &NrvNokonoko2D.KouraStop)) {
            if (al::isGreaterStep(this, 4))
                al::setNerve(this, &NrvNokonoko2D.KouraMoveNoTouch);
        } else if (al::isNerve(this, &NrvNokonoko2D.KouraEnd)) {
            al::setNerve(this, &NrvNokonoko2D.KouraMoveNoTouch);
        }
        return false;
    }

    if (rs::isMsgBlockUpperPunch2D(message)) {
        if (al::isNerve(this, &NrvNokonoko2D.KouraStop) && al::isLessEqualStep(this, 3))
            return false;

        if (mVelocityFront == 0.0f)
            mVelocityFront = 2.5f;

        const sead::Vector3f& front = al::getFront(this);
        const sead::Vector3f& selfSensor = al::getSensorPos(self);
        const sead::Vector3f& otherSensor = al::getSensorPos(other);
        bool facingPlayer = (selfSensor - otherSensor).dot(front) >= 0.0f;

        if (facingPlayer)
            al::setVelocityToFront(this, mVelocityFront);
        else
            al::setVelocityToFront(this, -mVelocityFront);
        al::addVelocityToGravity(this, -20.0f);

        if (al::isNerve(this, &NrvNokonoko2D.KouraMove) ||
            al::isNerve(this, &NrvNokonoko2D.KouraMoveNoTouch))
            return true;

        setVertFlip();

        al::setNerve(this, &NrvNokonoko2D.KouraStop);
        return true;
    }

    if (rs::isMsgKouraAttack2D(message)) {
        bool isKoura = false;
        if (al::isNerve(this, &NrvNokonoko2D.KouraMove) ||
            al::isNerve(this, &NrvNokonoko2D.KouraMoveNoTouch))
            isKoura = true;

        setVertFlip();

        al::setNerve(this, &NrvNokonoko2D.Damage);
        rs::setAppearItemFactorAndOffsetByMsg(this, message, other);
        al::appearItem(this);
        if (isKoura && rs::sendMsgKouraAttack2D(other, self))
            al::startHitReaction(this, "２Ｄコウラヒット");

        return true;
    }

    return false;
}

void Nokonoko2D::startMove() {
    al::onCollide(this);
    al::setNerve(this, &NrvNokonoko2D.Walk);
}

void Nokonoko2D::control() {
    if (al::isNerve(this, &NrvNokonoko2D.HideWait) || al::isNerve(this, &NrvNokonoko2D.AppearWait))
        return;

    if (al::isInDeathArea(this)) {
        al::setNerve(this, &NrvNokonoko2D.HideWait);
        return;
    }

    rs::updateDimensionKeeper(mDimensionKeeper);
    if (al::isCollidedGroundFloorCode(this, "Poison2D")) {
        al::setNerve(this, &NrvNokonoko2D.DamagePoison);
        return;
    }

    if (mIsHeavyGravity)
        al::addVelocityToGravity(this, 5.0f);
    else
        al::addVelocityToGravity(this, 0.65f);
    al::scaleVelocityDirection(this, al::getGravity(this), 0.98f);

    if (al::isCollidedWall(this)) {
        if (al::isNerve(this, &NrvNokonoko2D.KouraMove) ||
            al::isNerve(this, &NrvNokonoko2D.KouraMoveNoTouch)) {
            rs::sendMsgKouraAttack2D(al::getCollidedWallSensor(this),
                                     al::getHitSensor(this, "Attack"));
        }

        al::turnFront(this, 180.0f);

        if (al::isNerve(this, &NrvNokonoko2D.KouraMove) ||
            al::isNerve(this, &NrvNokonoko2D.KouraMoveNoTouch)) {
            al::startHitReaction(this, "２Ｄコウラ壁ヒット");
        }

        if (al::isOnGround(this, 0)) {
            al::setVelocityToFront(this, mVelocityFront);
            al::addVelocityToGravity(this, 0.65f);
        } else if (mIsTrampled) {
            sead::Vector3f parallelized;
            al::parallelizeVec(&parallelized, al::getFront(this),
                               al::getCollidedWallPos(this) - al::getTrans(this));
            f32 scale = al::getColliderRadius(this) - parallelized.length() + 1.0f;
            sead::Vector3f ez = sead::Vector3f::ez;
            al::addTransOffsetLocal(this, ez * scale);
        } else {
            sead::Vector3f* velocity = al::getVelocityPtr(this);
            al::limitVectorOppositeDir(velocity, al::getFront(this), *velocity, velocity->length());
            al::addVelocityToFront(this, mVelocityFront);
        }

        updateCollider();
    }

    mIsTrampled = false;

    if (!al::isNerve(this, &NrvNokonoko2D.Damage) &&
        !al::isNerve(this, &NrvNokonoko2D.DamagePoison)) {
        if (!rs::isIn2DArea(this)) {
            al::setNerve(this, &NrvNokonoko2D.HideWait);
            return;
        }
        if (al::isOnGround(this, 0)) {
            al::setVelocityToFront(this, mVelocityFront);
            al::addVelocityToGravity(this, 0.65f);
        }
        rs::snap2DGravity(this, this, 500.0f);
    }

    if (mClippingTime != -1) {
        bool isKouraMove = al::isNerve(this, &NrvNokonoko2D.KouraMoveNoTouch) ||
                           al::isNerve(this, &NrvNokonoko2D.KouraMove);
        if (isKouraMove && !isInClippingFrustum()) {
            if (mOffscreenTimer++ >= mClippingTime) {
                mOffscreenTimer = 0;
                al::setNerve(this, &NrvNokonoko2D.HideWait);
                return;
            }
        } else {
            mOffscreenTimer = 0;
        }
    }

    al::LiveActor* player = al::tryFindNearestPlayerActor(this);
    if (player && mResetArea) {
        if (al::isNerve(this, &NrvNokonoko2D.HideWait))
            return;

        if (al::isInAreaObj(mResetArea, al::getTrans(player))) {
            if (mIsResetForce)
                reset();
            else if (!isInClippingFrustum())
                al::setNerve(this, &NrvNokonoko2D.HideWait);
        }
    }
}

void Nokonoko2D::exeWalk() {
    if (al::isFirstStep(this)) {
        al::validateClipping(this);
        al::showModelIfHide(this);
        al::onCollide(this);
        al::validateHitSensors(this);
        clearVertFlip();
        mVelocityFront = 2.5f;
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocityToGravity(this, 0.65f);
        al::startAction(this, "Move");
    }

    if (al::isOnGround(this, 0)) {
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocityToGravity(this, 0.65f);
    }

    if (!mIsGreen && al::isOnGround(this, 0)) {
        bool isPathClear = alCollisionUtil::checkStrikeArrow(
            this, al::getTrans(this) + al::getFront(this) * 30.0f - al::getGravity(this) * 50.0f,
            al::getGravity(this) * 100.0f, mCollisionPartsFilter, nullptr);
        if (!isPathClear) {
            al::turnFront(this, 180.0f);
            al::setVelocityToFront(this, mVelocityFront);
            al::addVelocityToGravity(this, 0.65f);
            updateCollider();
        }
    }
}

void Nokonoko2D::exeKouraMove() {}

void Nokonoko2D::exeKouraMoveNoTouch() {
    if (al::isFirstStep(this)) {
        if (mClippingTime == -1)
            al::validateClipping(this);
        else
            al::invalidateClipping(this);

        const sead::Vector3f& trans = al::getTrans(this);
        sead::Vector3f playerPos = sead::Vector3f::zero;
        if (al::tryFindNearestPlayerPos(&playerPos, this)) {
            const sead::Vector3f& front = al::getFront(this);
            bool playerOnFrontSide = (playerPos - trans).dot(front) >= 0.0f;
            if (playerOnFrontSide)
                al::turnFront(this, 180.0f);
        }

        mVelocityFront = 17.3f;
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocityToGravity(this, 0.65f);
        al::startHitReaction(this, "コウラキック");
        al::startAction(this, "Spin");
    }

    if (al::isGreaterStep(this, 15))
        al::setNerve(this, &NrvNokonoko2D.KouraMove);
}

void Nokonoko2D::exeDamage() {
    if (al::isFirstStep(this)) {
        al::offCollide(this);
        al::invalidateHitSensors(this);
        al::turnFront(this, 180.0f);
        mVelocityFront = 2.5f;
        al::setVelocityToFront(this, mVelocityFront);
        al::addVelocity(this, al::getGravity(this) * -20.0f);
        al::startAction(this, "SpinStandby");
    }

    if (al::isGreaterStep(this, 90))
        al::setNerve(this, &NrvNokonoko2D.HideWait);
}

void Nokonoko2D::exeDamagePoison() {
    if (al::isFirstStep(this)) {
        al::offCollide(this);
        al::invalidateHitSensors(this);
    }

    if (al::isGreaterStep(this, 90))
        al::setNerve(this, &NrvNokonoko2D.HideWait);
}

void Nokonoko2D::exeKouraStop() {
    if (al::isFirstStep(this)) {
        al::startHitReaction(this, "踏まれ");
        al::startAction(this, "SpinStandby");
    }

    if (al::isOnGround(this, 0)) {
        mVelocityFront = 0.0f;
        al::setVelocityToFront(this, 0.0f);
        al::addVelocityToGravity(this, 0.65f);
    }

    if (al::isGreaterStep(this, 300))
        al::setNerve(this, &NrvNokonoko2D.KouraEnd);
}

void Nokonoko2D::exeKouraEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "SpinStandbyEnd");

    if (al::isOnGround(this, 0)) {
        mVelocityFront = 0.0f;
        al::setVelocityToFront(this, 0.0f);
        al::addVelocityToGravity(this, 0.65f);
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &NrvNokonoko2D.Walk);
}

void Nokonoko2D::exeHideWait() {
    if (al::isFirstStep(this)) {
        al::offCollide(this);
        al::hideModelIfShow(this);
        al::invalidateHitSensors(this);
        al::setVelocityZero(this);
        al::setTrans(this, mInitTrans);
        al::setFront(this, mInitFront);
        clearVertFlip();
        al::resetPosition(this);
        return;
    }

    if (!isInClippingFrustum()) {
        al::validateClipping(this);
        al::setNerve(this, &NrvNokonoko2D.AppearWait);
    }
}

void Nokonoko2D::exeAppearWait() {
    if (isInClippingFrustum()) {
        al::showModelIfHide(this);
        al::onCollide(this);
        al::validateHitSensors(this);
        al::setNerve(this, &NrvNokonoko2D.Walk);
    }
}

void Nokonoko2D::setVertFlip() {
    if (!mIsVertFlipped) {
        mLocalZRotator = 180.0f;
        mIsVertFlipped = true;
    }
}

void Nokonoko2D::clearVertFlip() {
    if (mIsVertFlipped) {
        mLocalZRotator = 0.0f;
        mIsVertFlipped = false;
    }
}

bool Nokonoko2D::isInClippingFrustum() const {
    const sead::Vector3f& center = al::getClippingCenterPos(this);
    f32 radius = al::getClippingRadius(this);
    return al::isInClippingFrustumAllView(this, center, radius, 0.0f);
}
