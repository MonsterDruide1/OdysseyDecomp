#include "Item/CoinStackBound.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Movement/FlashingTimer.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nature/WaterSurfaceFinder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"

#include "System/GameDataFunction.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CoinStackBound, Alive)
NERVE_IMPL(CoinStackBound, Appear)
NERVE_IMPL(CoinStackBound, Fall)
NERVE_IMPL(CoinStackBound, Bounce)
NERVE_IMPL(CoinStackBound, Vanish)
NERVE_IMPL(CoinStackBound, Collected)

NERVES_MAKE_NOSTRUCT(CoinStackBound, Alive, Appear, Collected)
NERVES_MAKE_STRUCT(CoinStackBound, Vanish, Fall, Bounce)
}  // namespace

CoinStackBound::CoinStackBound(const char* name) : al::LiveActor(name) {
    mBounceMtxEffect.makeIdentity();
}

void CoinStackBound::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "CoinStackBound", nullptr);
    al::initJointControllerKeeper(this, 1);
    mQuat.set(1.0f, 0.0f, 0.0f, 0.0f);
    al::initJointGlobalQuatController(this, &mQuat, "AllRoot");

    mFlashingTimer = new al::FlashingTimer(500, 180, 20, 10);
    mWaterSurfaceFinder = new al::WaterSurfaceFinder(this);
    al::initNerve(this, &Alive, 0);
    al::setEffectNamedMtxPtr(this, "跳ね返るエフェクトの原点", &mBounceMtxEffect);
    makeActorDead();
}

bool CoinStackBound::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                al::HitSensor* self) {
    if (rs::isMsgItemGetAll(message) && mBounceCount != 0 && mFlashingTimer->getLastTime() < 470) {
        al::invalidateClipping(this);
        al::setNerve(this, &Collected);
        return true;
    }
    return false;
}

void CoinStackBound::appear() {
    mMoveDirH = al::getTrans(this) - al::getPlayerPos(this, 0);
    mMoveDirH.normalize();

    if (mMoveDirH.dot(al::getGravity(this)) > 0.85f) {
        mMoveDirH.set(0.0f, 0.0f, 1.0f);
        al::rotateVectorQuat(&mMoveDirH, al::getQuat(this));
    }

    f32 hOffsetDegree = al::getRandom(20.0f, 50.0f);
    hOffsetDegree *= al::getRandom() > 0.5f ? 1.0f : -1.0f;

    al::rotateVectorDegree(&mMoveDirH, mMoveDirH, -al::getGravity(this), hOffsetDegree);

    mRotationAxis = mMoveDirH.cross(-al::getGravity(this));
    mRotationAxis.normalize();

    mBounceCount = 0;
    mRotationSpeed = 30.0f;
    mIsTimerRush = false;
    al::setVelocity(this, {0.0f, 0.0f, 0.0f});
    mFlashingTimer->start(500, 180, 20, 10);
    al::invalidateClipping(this);
    if (al::isHideModel(this))
        al::showModel(this);

    mBounceNormal.set(0.0f, 0.0f, 0.0f);
    mQuat.set(1.0f, 0.0f, 0.0f, 0.0f);
    al::setNerve(this, &Alive);
    makeActorAlive();
}

void CoinStackBound::processTimer() {
    mFlashingTimer->update();
    if (mFlashingTimer->isHurryStart() && !mIsTimerRush) {
        mIsTimerRush = true;
        al::startHitReaction(this, "タイマー急ぎ期間開始");
    }
    if (mFlashingTimer->getTime() <= 0)
        al::setNerve(this, &NrvCoinStackBound.Vanish);
}

void CoinStackBound::exeAlive() {
    al::startHitReaction(this, "タイマー起動");
    al::setNerve(this, &Appear);
}

void CoinStackBound::exeAppear() {
    al::startHitReaction(this, "出力");
    al::setVelocitySeparateHV(this, mMoveDirH, 10.0f, 30.0f);
    al::setNerve(this, &NrvCoinStackBound.Fall);
}

void CoinStackBound::exeFall() {
    if (al::isInWater(this) || al::isInDeathArea(this) ||
        al::isCollidedGroundFloorCode(this, "DamageFire") ||
        al::isCollidedGroundFloorCode(this, "Needle") ||
        al::isCollidedGroundFloorCode(this, "Poison")) {
        al::setNerve(this, &NrvCoinStackBound.Vanish);
        return;
    }

    processTimer();
    al::rotateQuatRadian(&mQuat, mQuat, mRotationAxis, sead::Mathf::deg2rad(mRotationSpeed));
    mRotationSpeed *= 0.995f;
    al::addVelocityToGravity(this, 1.1f);
    al::scaleVelocityHV(this, 1.0f, 1.0f);
    if (al::isOnGround(this, 0)) {
        mBounceNormal = al::getOnGroundNormal(this, 0);
        mHasWaterCollision = true;
        mHasWallCollision = true;
        al::setNerve(this, &NrvCoinStackBound.Bounce);
        return;
    }

    if (mHasWallCollision && al::isCollidedWall(this)) {
        mBounceNormal = al::getCollidedWallNormal(this);
        mHasWaterCollision = false;
        mHasWallCollision = false;
        al::setNerve(this, &NrvCoinStackBound.Bounce);
    }
}

inline f32 getAngle(const sead::Vector3f& v1, const sead::Vector3f& v2) {
    sead::Vector3f cross;
    f32 dot = v1.dot(v2);
    cross.setCross(v1, v2);
    return atan2f(cross.length(), dot);
}

void CoinStackBound::exeBounce() {
    if (al::isFirstStep(this) && mHasWaterCollision) {
        mWaterSurfaceFinder->update(al::getTrans(this), mBounceNormal,
                                    al::getSensorRadius(this, "Body"));
        if (mWaterSurfaceFinder->isFoundSurface() &&
            al::isInWaterPos(this, mWaterSurfaceFinder->getSurfacePosition())) {
            al::updateMaterialCodePuddle(this, true);
            al::makeMtxUpNoSupportPos(&mBounceMtxEffect, mWaterSurfaceFinder->getSurfaceNormal(),
                                      mWaterSurfaceFinder->getSurfacePosition());
        } else {
            al::updateMaterialCodePuddle(this, false);
            al::makeMtxUpNoSupportPos(&mBounceMtxEffect, mBounceNormal, al::getTrans(this));
        }
        al::startHitReaction(this, "跳ね返る");
        mHasWaterCollision = false;
    }

    mBounceCount++;
    processTimer();

    f32 angle = sead::Mathf::rad2deg(getAngle(mBounceNormal, sead::Vector3f::ey));
    if (angle > 25.0f && angle < 65.0f)
        mBounceNormal = sead::Vector3f::ey;

    al::calcReflectionVector(al::getVelocityPtr(this), mBounceNormal, 0.95f, 0.0f);

    sead::Vector3f velV;
    sead::Vector3f velH;
    al::separateVelocityHV(&velV, &velH, this);

    velV *= 0.9f;
    f32 speedV = velV.length();

    f32 speedH = velH.length();
    if (speedH > 28.0f)
        velH *= 28.0f / speedH;

    if (!mHasReachedMaxVelocity && speedV > mMaxVelocity)
        mHasReachedMaxVelocity = true;

    if (mHasReachedMaxVelocity) {
        velV *= mMaxVelocity / speedV;
        if (mMaxVelocity > 0.0f)
            mMaxVelocity -= 1.0f;
    }

    al::setVelocity(this, velV + velH);

    sead::Vector3f velocity = al::getVelocity(this);
    velocity.normalize();

    mRotationAxis = velocity.cross(-al::getGravity(this));
    mRotationAxis.normalize();

    al::setNerve(this, &NrvCoinStackBound.Fall);
}

void CoinStackBound::exeVanish() {
    al::startHitReaction(this, "消える");
    al::tryAddRippleMiddle(this);
    makeActorDead();
}

void CoinStackBound::exeCollected() {
    al::startHitReaction(this, "取得");
    GameDataFunction::addCoin(this, 10);
    makeActorDead();
}
