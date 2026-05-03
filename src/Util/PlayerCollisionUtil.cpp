#include "Util/PlayerCollisionUtil.h"

#include <algorithm>

#include "Library/Collision/Collider.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Collision/PartsInterpolator.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/CollisionShapeInfo.h"
#include "Player/CollisionShapeKeeper.h"
#include "Player/IUsePlayerCollision.h"
#include "Player/IUsePlayerHeightCheck.h"
#include "Player/PlayerCollider.h"
#include "Player/PlayerConst.h"
#include "Util/AreaUtil.h"
#include "Util/CollisionShapeFunction.h"

namespace rs {

struct PoleTriangleFilter {
    const void* _0;
    const sead::Vector3f* direction;
};

bool isInvalidPoleTriangle(const PoleTriangleFilter* filter, const al::Triangle& triangle) {
    if (!isCollisionCodePole(triangle))
        return true;

    const sead::Vector3f& normal = triangle.getNormal(0);
    return filter->direction->dot(normal) > 0.99619f;
}

}  // namespace rs

namespace {

PlayerCollider* getCollider(const IUsePlayerCollision* collision) {
    return collision->getPlayerCollider();
}

al::HitInfo* getGroundHitInfo(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getGroundHitInfo();
}

al::HitInfo* getWallHitInfo(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getWallHitInfo();
}

al::HitInfo* getCeilingHitInfo(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getCeilingHitInfo();
}

bool isGroundCode(const IUsePlayerCollision* collision, const char* code) {
    if (!rs::isCollidedGround(collision))
        return false;
    return al::isFloorCode(getGroundHitInfo(collision), code);
}

bool isWallCode(const IUsePlayerCollision* collision, const char* code) {
    if (!rs::isCollidedWall(collision))
        return false;
    return al::isWallCode(getWallHitInfo(collision), code);
}

bool isWallFloorCode(const IUsePlayerCollision* collision, const char* code) {
    if (!rs::isCollidedWall(collision))
        return false;
    return al::isFloorCode(getWallHitInfo(collision), code);
}

bool isCeilingCode(const IUsePlayerCollision* collision, const char* code) {
    if (!rs::isCollidedCeiling(collision))
        return false;
    return al::isFloorCode(getCeilingHitInfo(collision), code);
}

bool isHitInfoFloorCode(const al::HitInfo& hitInfo, const char* code) {
    return al::isFloorCode(&hitInfo, code);
}

bool isHitInfoWallCode(const al::HitInfo& hitInfo, const char* code) {
    return al::isWallCode(&hitInfo, code);
}

bool isHitInfoTriangleFloorCode(const al::HitInfo& hitInfo, const char* code) {
    return al::isFloorCode(hitInfo.triangle, code);
}

const sead::Vector3f& getHitInfoNormal(const al::HitInfo* hitInfo) {
    return hitInfo->triangle.getFaceNormal();
}

al::HitSensor* getHitInfoSensor(const al::HitInfo* hitInfo) {
    return hitInfo->triangle.getCollisionParts()->getConnectedSensor();
}

const al::CollisionParts* getHitInfoCollisionParts(const al::HitInfo* hitInfo) {
    return hitInfo->triangle.getCollisionParts();
}

}  // namespace

namespace al {

bool TriangleFilterFunc::isInvalidTriangle(const Triangle& triangle) const {
    return mFunc(triangle);
}

}  // namespace al

namespace rs {

bool isCollisionCodePoleThunk(const al::Triangle& triangle) {
    return isCollisionCodePole(triangle);
}

void resetCollision(IUsePlayerCollision* collision) {
    collision->getPlayerCollider()->onInvalidate();
}

void resetCollisionPose(const IUsePlayerCollision* collision, const sead::Quatf& quat) {
    collision->getPlayerCollider()->resetPose(quat);
}

void resetCollisionExpandCheck(IUsePlayerCollision* collision) {
    PlayerCollider* collider = collision->getPlayerCollider();
    f32 radius = 0.0f;
    collider->calcBoundingRadius(&radius);
    f32 scale;

    if (al::isNearZero(radius, 0.001f) ||
        (scale = collider->getCollisionShapeScale(), al::isNearZero(scale, 0.001f))) {
        collision->getPlayerCollider()->onInvalidate();
        return;
    }

    f32 radiusScale = radius / scale;
    f32 maxRadiusScale = 20.0f;
    f32 minRadiusScale = std::min(radiusScale, maxRadiusScale);
    f32 rate = minRadiusScale / radiusScale;
    if (rate < 0.0f)
        rate = 0.0f;
    else if (rate > 1.0f)
        rate = 1.0f;

    collider->setCollisionShapeScale(rate);
    collision->getPlayerCollider()->onInvalidate();
    collider->setCollisionShapeScale(scale);
}

bool isCollided(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getGroundDistance() >= 0.0f ||
           getCollider(collision)->getWallDistance() >= 0.0f ||
           getCollider(collision)->getCeilingDistance() >= 0.0f;
}

bool isCollidedGround(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getGroundDistance() >= 0.0f;
}

bool isCollidedWall(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getWallDistance() >= 0.0f;
}

bool isCollidedCeiling(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getCeilingDistance() >= 0.0f;
}

bool isCollidedGroundFace(const IUsePlayerCollision* collision) {
    if (!isCollidedGround(collision))
        return false;
    return getGroundHitInfo(collision)->isCollisionAtFace();
}

bool isCollidedWallFace(const IUsePlayerCollision* collision) {
    if (!isCollidedWall(collision))
        return false;
    return getWallHitInfo(collision)->isCollisionAtFace();
}

const sead::Vector3f& getCollidedGroundNormal(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getCollidedGroundNormal();
}

const sead::Vector3f& getCollidedWallNormal(const IUsePlayerCollision* collision) {
    return getWallHitInfo(collision)->triangle.getFaceNormal();
}

const sead::Vector3f& getCollidedCeilingNormal(const IUsePlayerCollision* collision) {
    return getCeilingHitInfo(collision)->triangle.getFaceNormal();
}

const sead::Vector3f& getCollidedGroundPos(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getCollidedGroundPos();
}

const sead::Vector3f& getCollidedWallPos(const IUsePlayerCollision* collision) {
    return getWallHitInfo(collision)->collisionHitPos;
}

const sead::Vector3f& getCollidedCeilingPos(const IUsePlayerCollision* collision) {
    return getCeilingHitInfo(collision)->collisionHitPos;
}

const sead::Vector3f& getCollidedFixReaction(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getCollidedFixReaction();
}

al::HitSensor* tryGetCollidedGroundSensor(const IUsePlayerCollision* collision) {
    al::HitSensor* sensor = nullptr;
    if (isCollidedGround(collision))
        sensor = getHitInfoSensor(getGroundHitInfo(collision));
    return sensor;
}

al::HitSensor* tryGetCollidedWallSensor(const IUsePlayerCollision* collision) {
    al::HitSensor* sensor = nullptr;
    if (isCollidedWall(collision))
        sensor = getHitInfoSensor(getWallHitInfo(collision));
    return sensor;
}

al::HitSensor* tryGetCollidedCeilingSensor(const IUsePlayerCollision* collision) {
    al::HitSensor* sensor = nullptr;
    if (isCollidedCeiling(collision))
        sensor = getHitInfoSensor(getCeilingHitInfo(collision));
    return sensor;
}

const al::CollisionParts* getCollidedGroundCollisionParts(const IUsePlayerCollision* collision) {
    return getHitInfoCollisionParts(getGroundHitInfo(collision));
}

const al::CollisionParts* getCollidedWallCollisionParts(const IUsePlayerCollision* collision) {
    return getHitInfoCollisionParts(getWallHitInfo(collision));
}

const al::CollisionParts* getCollidedCeilingCollisionParts(const IUsePlayerCollision* collision) {
    return getHitInfoCollisionParts(getCeilingHitInfo(collision));
}

s32 getGroundHitInfoNum(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getGroundHitInfoArray().size();
}

const sead::Vector3f& getGroundHitInfoNormal(const IUsePlayerCollision* collision, s32 index) {
    return getHitInfoNormal(getCollider(collision)->getGroundHitInfoArray()[index]);
}

al::HitSensor* getGroundHitInfoSensor(const IUsePlayerCollision* collision, s32 index) {
    return getHitInfoSensor(getCollider(collision)->getGroundHitInfoArray()(index));
}

const al::CollisionParts* getGroundHitInfoCollisionParts(const IUsePlayerCollision* collision,
                                                         s32 index) {
    return getHitInfoCollisionParts(getCollider(collision)->getGroundHitInfoArray()(index));
}

s32 getWallHitInfoNum(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getWallHitInfoArray().size();
}

const sead::Vector3f& getWallHitInfoNormal(const IUsePlayerCollision* collision, s32 index) {
    return getHitInfoNormal(getCollider(collision)->getWallHitInfoArray()[index]);
}

al::HitSensor* getWallHitInfoSensor(const IUsePlayerCollision* collision, s32 index) {
    return getHitInfoSensor(getCollider(collision)->getWallHitInfoArray()(index));
}

const al::CollisionParts* getWallHitInfoCollisionParts(const IUsePlayerCollision* collision,
                                                       s32 index) {
    return getHitInfoCollisionParts(getCollider(collision)->getWallHitInfoArray()(index));
}

bool isExistCollidedGroundParts(const IUsePlayerCollision* collision,
                                const al::CollisionParts* parts) {
    const s32 count = getGroundHitInfoNum(collision);
    if (count < 1)
        return false;

    for (s32 i = 0; i < count; ++i)
        if (getGroundHitInfoCollisionParts(collision, i) == parts)
            return true;
    return false;
}

bool isCollisionCodePress(const al::HitInfo& hitInfo) {
    return isHitInfoFloorCode(hitInfo, "Press");
}

bool isCollisionCodeGrabCeil(const al::HitInfo& hitInfo) {
    return isHitInfoFloorCode(hitInfo, "GrabCeil");
}

bool isCollisionCodeDeadGround(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "Poison");
}

bool isCollisionCodeDeadWall(const IUsePlayerCollision* collision) {
    return isWallCode(collision, "Poison");
}

bool isCollisionCodePoisonTouch(const IUsePlayerCollision* collision) {
    PlayerCollider* collider = collision->getPlayerCollider();
    if (isCollidedGround(collision) && al::isFloorCode(collider->getGroundHitInfo(), "Poison"))
        return true;
    if (isCollidedWall(collision) && al::isFloorCode(collider->getWallHitInfo(), "Poison"))
        return true;
    return false;
}

bool isCollisionCodeDead2dGround(const IUsePlayerCollision* collision) {
    if (!isCollidedGround(collision))
        return false;
    al::HitInfo* hitInfo = getGroundHitInfo(collision);
    if (al::isFloorCode(hitInfo, "Poison2D") || al::isFloorCode(hitInfo, "DamageFire2D"))
        return true;
    return al::isFloorCode(hitInfo, "DamageFire");
}

bool isCollisionCodeDead2dCeiling(const IUsePlayerCollision* collision) {
    if (!isCollidedCeiling(collision))
        return false;
    al::HitInfo* hitInfo = getCeilingHitInfo(collision);
    if (al::isFloorCode(hitInfo, "Poison2D") || al::isFloorCode(hitInfo, "DamageFire2D"))
        return true;
    return al::isFloorCode(hitInfo, "DamageFire");
}

bool isCollisionCodeDamageGround(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "Needle");
}

bool isCollisionCodeDamageWall(const IUsePlayerCollision* collision) {
    return isWallCode(collision, "Needle");
}

bool isCollisionCodeDamageCeiling(const IUsePlayerCollision* collision) {
    return isCeilingCode(collision, "Needle");
}

bool isCollisionCodeDamageFireGround(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "DamageFire");
}

bool isCollisionCodeDamageFireWall(const IUsePlayerCollision* collision) {
    return isWallCode(collision, "DamageFire");
}

bool isCollisionCodeDamageFireCeiling(const IUsePlayerCollision* collision) {
    return isCeilingCode(collision, "DamageFire");
}

bool isCollisionCodeSandSink(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "SandSink");
}

bool isCollisionCodeSlide(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "Slide");
}

bool isCollisionCodeJump(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "Jump");
}

bool isCollisionCodeJumpSmall(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "JumpSmall");
}

bool isCollisionCodeBed(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "Bed");
}

bool isCollisionCodeChair(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "Chair");
}

bool isCollisionCodeGrabCeilAny(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "GrabCeil") || isWallCode(collision, "GrabCeil") ||
           isCeilingCode(collision, "GrabCeil");
}

bool isCollisionCodeGrabCeilGrround(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "GrabCeil");
}

bool isCollisionCodeGrabCeilWall(const IUsePlayerCollision* collision) {
    return isWallCode(collision, "GrabCeil");
}

bool isCollisionCodeGrabCeilCeiling(const IUsePlayerCollision* collision) {
    return isCeilingCode(collision, "GrabCeil");
}

bool isCollisionCodePoleClimbAny(const IUsePlayerCollision* collision) {
    return isCollisionCodePoleClimbGround(collision) || isCollisionCodePoleClimbWall(collision) ||
           isCollisionCodePoleClimbCeiling(collision);
}

bool isCollisionCodePoleClimbGround(const IUsePlayerCollision* collision) {
    if (!isCollidedGround(collision))
        return false;
    al::HitInfo* hitInfo = getGroundHitInfo(collision);
    if (al::isWallCode(hitInfo, "NoClimbPole"))
        return false;
    if (al::isFloorCode(hitInfo, "Pole") || al::isFloorCode(hitInfo, "Pole10") ||
        al::isFloorCode(hitInfo, "Pole20"))
        return true;
    return al::isFloorCode(hitInfo, "Pole30Plus");
}

bool isCollisionCodePoleClimbWall(const IUsePlayerCollision* collision) {
    if (!isCollidedWall(collision))
        return false;
    al::HitInfo* hitInfo = getWallHitInfo(collision);
    if (al::isWallCode(hitInfo, "NoClimbPole"))
        return false;
    if (al::isFloorCode(hitInfo, "Pole") || al::isFloorCode(hitInfo, "Pole10") ||
        al::isFloorCode(hitInfo, "Pole20"))
        return true;
    return al::isFloorCode(hitInfo, "Pole30Plus");
}

bool isCollisionCodePoleClimbCeiling(const IUsePlayerCollision* collision) {
    if (!isCollidedCeiling(collision))
        return false;
    al::HitInfo* hitInfo = getCeilingHitInfo(collision);
    if (al::isWallCode(hitInfo, "NoClimbPole"))
        return false;
    if (al::isFloorCode(hitInfo, "Pole") || al::isFloorCode(hitInfo, "Pole10") ||
        al::isFloorCode(hitInfo, "Pole20"))
        return true;
    return al::isFloorCode(hitInfo, "Pole30Plus");
}

bool isCollisionCodePoleClimb(const al::HitInfo& hitInfo) {
    if (isHitInfoWallCode(hitInfo, "NoClimbPole"))
        return false;
    if (isHitInfoFloorCode(hitInfo, "Pole") || isHitInfoFloorCode(hitInfo, "Pole10") ||
        isHitInfoFloorCode(hitInfo, "Pole20"))
        return true;
    return isHitInfoFloorCode(hitInfo, "Pole30Plus");
}

bool isCollisionCodePole(const al::Triangle& triangle) {
    if (!al::isWallCode(triangle, "NoClimbPole") &&
        (al::isFloorCode(triangle, "Pole") || al::isFloorCode(triangle, "Pole10") ||
         al::isFloorCode(triangle, "Pole20") || al::isFloorCode(triangle, "Pole30Plus")))
        return true;
    return al::isWallCode(triangle, "NoClimbPole");
}

bool isActionCodeNoWallKeepWall(const IUsePlayerCollision* collision) {
    return isWallCode(collision, "NoAction") || isWallCode(collision, "OnlyWallHitDown") ||
           isWallCode(collision, "NoWallGrab") || isWallCode(collision, "ReflectStickNoWallGrab");
}

bool isActionCodeNoActionGround(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "NoAction");
}

bool isActionCodeNoActionWall(const IUsePlayerCollision* collision) {
    return isWallCode(collision, "NoAction");
}

bool isActionCodeNoActionCeiling(const IUsePlayerCollision* collision) {
    return isCeilingCode(collision, "NoAction");
}

bool isActionCodeNoWallGrab(const IUsePlayerCollision* collision) {
    if (isWallCode(collision, "NoAction"))
        return true;
    if (isWallCode(collision, "NoWallGrab"))
        return true;
    if (isWallCode(collision, "ReflectStickNoWallGrab"))
        return true;
    if (isWallCode(collision, "OnlyWallHitDown"))
        return true;
    if (isWallFloorCode(collision, "GrabCeil"))
        return true;
    if (isWallFloorCode(collision, "Pole"))
        return true;
    if (isWallFloorCode(collision, "Needle"))
        return true;
    return isWallFloorCode(collision, "DamageFire");
}

bool isActionCodeNoWallGrab(const al::HitInfo& hitInfo) {
    if (isHitInfoWallCode(hitInfo, "NoAction") || isHitInfoWallCode(hitInfo, "NoWallGrab") ||
        isHitInfoWallCode(hitInfo, "ReflectStickNoWallGrab") ||
        isHitInfoWallCode(hitInfo, "OnlyWallHitDown") || isHitInfoFloorCode(hitInfo, "GrabCeil") ||
        isHitInfoFloorCode(hitInfo, "Pole") || isHitInfoTriangleFloorCode(hitInfo, "Needle") ||
        isHitInfoTriangleFloorCode(hitInfo, "DamageFire"))
        return true;
    return isHitInfoTriangleFloorCode(hitInfo, "Poison");
}

bool isActionCodeNoWallPopUp(const al::HitInfo& hitInfo) {
    if (isHitInfoWallCode(hitInfo, "NoAction") || isHitInfoWallCode(hitInfo, "NoWallGrab") ||
        isHitInfoWallCode(hitInfo, "ReflectStickNoWallGrab") ||
        isHitInfoWallCode(hitInfo, "OnlyWallHitDown") || isHitInfoFloorCode(hitInfo, "GrabCeil") ||
        isHitInfoTriangleFloorCode(hitInfo, "Needle") ||
        isHitInfoTriangleFloorCode(hitInfo, "DamageFire"))
        return true;
    return isHitInfoTriangleFloorCode(hitInfo, "Poison");
}

bool isActionCodeNoTongueClingGround(const IUsePlayerCollision* collision) {
    return isGroundCode(collision, "NoClingTongue") || isGroundCode(collision, "NoAction") ||
           isGroundCode(collision, "Needle");
}

bool isActionCodeNoTongueClingWall(const IUsePlayerCollision* collision) {
    if (isWallCode(collision, "NoClingTongue"))
        return true;
    if (isWallCode(collision, "NoAction"))
        return true;
    if (isWallCode(collision, "OnlyWallHitDown"))
        return true;
    if (isWallFloorCode(collision, "Needle"))
        return true;
    if (isWallFloorCode(collision, "DamageFire"))
        return true;
    return isWallFloorCode(collision, "Poison");
}

f32 getPoleCodeAngleOffset(const al::HitInfo& hitInfo) {
    if (isHitInfoTriangleFloorCode(hitInfo, "Pole10"))
        return -10.0f;
    if (isHitInfoTriangleFloorCode(hitInfo, "Pole20"))
        return -20.0f;
    return isHitInfoTriangleFloorCode(hitInfo, "Pole30Plus") ? 30.0f : 0.0f;
}

f32 getPoleCodeAngleOffsetGround(const IUsePlayerCollision* collision) {
    al::HitInfo* hitInfo = getGroundHitInfo(collision);
    if (isHitInfoTriangleFloorCode(*hitInfo, "Pole10"))
        return -10.0f;
    if (isHitInfoTriangleFloorCode(*hitInfo, "Pole20"))
        return -20.0f;
    return isHitInfoTriangleFloorCode(*hitInfo, "Pole30Plus") ? 30.0f : 0.0f;
}

f32 getPoleCodeAngleOffsetWall(const IUsePlayerCollision* collision) {
    al::HitInfo* hitInfo = getWallHitInfo(collision);
    if (isHitInfoTriangleFloorCode(*hitInfo, "Pole10"))
        return -10.0f;
    if (isHitInfoTriangleFloorCode(*hitInfo, "Pole20"))
        return -20.0f;
    return isHitInfoTriangleFloorCode(*hitInfo, "Pole30Plus") ? 30.0f : 0.0f;
}

f32 getPoleCodeAngleOffsetCeiling(const IUsePlayerCollision* collision) {
    al::HitInfo* hitInfo = getCeilingHitInfo(collision);
    if (isHitInfoTriangleFloorCode(*hitInfo, "Pole10"))
        return -10.0f;
    if (isHitInfoTriangleFloorCode(*hitInfo, "Pole20"))
        return -20.0f;
    return isHitInfoTriangleFloorCode(*hitInfo, "Pole30Plus") ? 30.0f : 0.0f;
}

const char* getMaterialCodeGround(const IUsePlayerCollision* collision) {
    return al::getMaterialCodeName(getGroundHitInfo(collision));
}

const char* getMaterialCodeWall(const IUsePlayerCollision* collision) {
    return al::getMaterialCodeName(getWallHitInfo(collision));
}

const char* getMaterialCodeCeiling(const IUsePlayerCollision* collision) {
    return al::getMaterialCodeName(getCeilingHitInfo(collision));
}

bool isRippleGenearateMaterialLawn(const al::Triangle& triangle) {
    return al::isMaterialCode(triangle, "LawnDeep") ||
           al::isMaterialCode(triangle, "LawnDeepForest") ||
           al::isMaterialCode(triangle, "LawnDeepWaterfall");
}

const char* getRippleGenerateMaterialFlower() {
    return "FlowerForest";
}

bool isCollidedDamageCodeAnyWallHit(const IUsePlayerCollision* collision) {
    const PlayerCollider* collider = getCollider(collision);
    const sead::PtrArray<al::HitInfo>& hitInfoArray = collider->getWallHitInfoArray();
    const s32 hitInfoNum = hitInfoArray.size();
    if (hitInfoNum < 1)
        return false;

    for (s32 i = 0; i < hitInfoNum; ++i) {
        const al::HitInfo* hitInfo = hitInfoArray[i];
        if (isHitInfoTriangleFloorCode(*hitInfo, "Needle"))
            return true;
    }
    return false;
}

bool isEnableRecordSafetyPoint(sead::Vector3f** safetyPoint, const al::HitInfo& hitInfo,
                               al::HitSensor* sensor, const sead::Vector3f& gravity) {
    if (!al::sendMsgAskSafetyPoint(alCollisionUtil::getCollisionHitSensor(&hitInfo), sensor,
                                   safetyPoint))
        return false;
    if (al::calcAngleDegree(alCollisionUtil::getCollisionHitNormal(&hitInfo), gravity) >= 60.0f)
        return false;
    return isCollisionCodeSafetyPoint(hitInfo);
}

bool isCollisionCodeSafetyPointImpl(const al::HitInfo& hitInfo) {
    if (isHitInfoTriangleFloorCode(hitInfo, "Ground"))
        return true;
    if (isHitInfoTriangleFloorCode(hitInfo, "Poison"))
        return false;
    if (isHitInfoTriangleFloorCode(hitInfo, "Poison2D"))
        return false;
    if (isHitInfoTriangleFloorCode(hitInfo, "DamageFire2D"))
        return false;
    if (isHitInfoTriangleFloorCode(hitInfo, "DamageFire"))
        return false;
    if (isHitInfoTriangleFloorCode(hitInfo, "Needle"))
        return false;
    if (isHitInfoTriangleFloorCode(hitInfo, "DamageFire"))
        return false;
    if (!al::isWallCode(hitInfo.triangle, "NoClimbPole")) {
        if (isHitInfoTriangleFloorCode(hitInfo, "Pole"))
            return false;
        if (isHitInfoTriangleFloorCode(hitInfo, "Pole10"))
            return false;
        if (isHitInfoTriangleFloorCode(hitInfo, "Pole20"))
            return false;
        if (isHitInfoTriangleFloorCode(hitInfo, "Pole30Plus"))
            return false;
    }
    if (al::isWallCode(hitInfo.triangle, "NoClimbPole"))
        return false;
    if (isHitInfoTriangleFloorCode(hitInfo, "GrabCeil"))
        return false;
    if (isHitInfoTriangleFloorCode(hitInfo, "Slide"))
        return false;
    return !isHitInfoTriangleFloorCode(hitInfo, "SandSink");
}

bool isCollisionCodeSafetyPoint(const al::HitInfo& hitInfo) {
    return isCollisionCodeSafetyPointImpl(hitInfo);
}

void calcCollisionCodeNoSafetyPointPos(sead::Vector3f* pos, bool* isNoCollideGround,
                                       const al::LiveActor* actor,
                                       const IUsePlayerCollision* collision) {
    if (isCollidedWall(collision)) {
        const al::HitInfo* wallHitInfo = getWallHitInfo(collision);
        if (!isCollisionCodeSafetyPoint(*wallHitInfo)) {
            *isNoCollideGround = false;
            pos->set(wallHitInfo->collisionHitPos);
            return;
        }
    }

    if (isCollidedGround(collision)) {
        *isNoCollideGround = false;
        const al::HitInfo* groundHitInfo = getGroundHitInfo(collision);
        if (!isCollisionCodeSafetyPoint(*groundHitInfo)) {
            pos->set(groundHitInfo->collisionHitPos);
            return;
        }
    } else {
        *isNoCollideGround = true;
    }

    pos->set(al::getTrans(actor));
}

void calcActorCollisionCodeNoSafetyPointPos(sead::Vector3f* pos, bool* isNoCollideGround,
                                            const al::LiveActor* actor,
                                            const IUsePlayerCollision* collision) {
    if (collision) {
        calcCollisionCodeNoSafetyPointPos(pos, isNoCollideGround, actor, collision);
        return;
    }

    if (!al::isExistActorCollider(actor)) {
        *isNoCollideGround = true;
        pos->set(al::getTrans(actor));
        return;
    }

    if (al::isCollidedWall(actor)) {
        const al::Collider* actorCollider = actor->getCollider();
        if (!isCollisionCodeSafetyPoint(actorCollider->getWallHit())) {
            *isNoCollideGround = false;
            pos->set(actorCollider->getWallHit().collisionHitPos);
            return;
        }
    }

    if (!al::isCollidedGround(actor)) {
        *isNoCollideGround = true;
        pos->set(al::getTrans(actor));
        return;
    }

    *isNoCollideGround = false;
    const al::Collider* actorCollider = actor->getCollider();
    if (isCollisionCodeSafetyPoint(actorCollider->getFloorHit())) {
        pos->set(al::getTrans(actor));
        return;
    }
    pos->set(actorCollider->getFloorHit().collisionHitPos);
}

bool calcCollidedGroundSafetyPoint(sead::Vector3f* targetPos, sead::Vector3f* targetRotate,
                                   const al::AreaObj** area, sead::Vector3f** safetyPoint,
                                   const al::LiveActor* actor, const IUsePlayerCollision* collision,
                                   al::HitSensor* sensor, const sead::Vector3f& pos,
                                   const sead::Vector3f& gravity) {
    bool isNoCollideGround = false;
    const sead::Vector3f& gravityDir = pos;
    const sead::Vector3f& recordPos = gravity;
    const al::IUseAreaObj* areaUser = nullptr;
    if (actor)
        areaUser = actor;
    const sead::Vector3f& trans = al::getTrans(actor);
    const bool foundArea = tryFindRecoveryTargetPosArea(&isNoCollideGround, targetPos, targetRotate,
                                                        area, areaUser, trans);
    if (isNoCollideGround)
        return true;

    if (!isCollidedGround(collision))
        return false;

    const bool isGroundFace = getGroundHitInfo(collision)->isCollisionAtFace();
    if (foundArea || !isGroundFace)
        return isGroundFace;

    const al::HitInfo* groundHitInfo = getGroundHitInfo(collision);
    al::HitSensor* groundSensor = alCollisionUtil::getCollisionHitSensor(groundHitInfo);
    if (!al::sendMsgAskSafetyPoint(groundSensor, sensor, safetyPoint))
        return false;
    const sead::Vector3f& hitNormal = alCollisionUtil::getCollisionHitNormal(groundHitInfo);
    if (al::calcAngleDegree(hitNormal, gravityDir) >= 60.0f)
        return false;
    if (!isCollisionCodeSafetyPoint(*groundHitInfo))
        return false;

    if (isCollidedWall(collision)) {
        const al::HitInfo* wallHitInfo = getWallHitInfo(collision);
        if (wallHitInfo->isCollisionAtFace()) {
            const al::HitInfo* wallHitInfoForPos = getWallHitInfo(collision);
            const al::HitInfo* wallHitInfoForNormal = getWallHitInfo(collision);
            const sead::Vector3f& wallNormal = wallHitInfoForNormal->triangle.getFaceNormal();
            const f32 dot = (recordPos - wallHitInfoForPos->collisionHitPos).dot(wallNormal);
            if (!al::isNearZeroOrGreater(dot, 0.001f))
                return false;
        }
    }

    targetPos->set(recordPos);
    targetRotate->set(getCollidedGroundNormal(collision));
    return true;
}

bool calcActorCollidedGroundSafetyPoint(sead::Vector3f* targetPos, sead::Vector3f* targetRotate,
                                        const al::AreaObj** area, sead::Vector3f** safetyPoint,
                                        const al::LiveActor* actor, al::HitSensor* sensor,
                                        const IUsePlayerCollision* collision) {
    if (collision) {
        const sead::Vector3f& gravity = al::getGravity(actor);
        sead::Vector3f negGravity(-gravity.x, -gravity.y, -gravity.z);
        const sead::Vector3f& trans = al::getTrans(actor);
        return calcCollidedGroundSafetyPoint(targetPos, targetRotate, area, safetyPoint, actor,
                                             collision, sensor, negGravity, trans);
    }

    if (!al::isExistActorCollider(actor))
        return false;

    bool isNoCollideGround = false;
    const al::IUseAreaObj* areaUser = nullptr;
    if (actor)
        areaUser = actor;
    const sead::Vector3f& trans = al::getTrans(actor);
    const bool foundArea = tryFindRecoveryTargetPosArea(&isNoCollideGround, targetPos, targetRotate,
                                                        area, areaUser, trans);
    if (isNoCollideGround)
        return true;

    if (!al::isCollidedGround(actor))
        return false;

    const bool isEdgeOrCorner = al::isCollidedGroundEdgeOrCorner(actor);
    if (foundArea || isEdgeOrCorner)
        return !isEdgeOrCorner;

    al::Collider* actorCollider = actor->getCollider();
    const al::HitInfo& groundHitInfo = actorCollider->getFloorHit();
    const sead::Vector3f& gravity = al::getGravity(actor);
    sead::Vector3f negGravity(-gravity.x, -gravity.y, -gravity.z);

    al::HitSensor* groundSensor = alCollisionUtil::getCollisionHitSensor(&groundHitInfo);
    if (!al::sendMsgAskSafetyPoint(groundSensor, sensor, safetyPoint))
        return false;
    const sead::Vector3f& hitNormal = alCollisionUtil::getCollisionHitNormal(&groundHitInfo);
    if (al::calcAngleDegree(hitNormal, negGravity) >= 60.0f)
        return false;
    if (!isCollisionCodeSafetyPoint(groundHitInfo))
        return false;

    if (al::isCollidedWallFace(actor)) {
        const sead::Vector3f& wallPos = al::getCollidedWallPos(actor);
        const sead::Vector3f& wallNormal = al::getCollidedWallNormal(actor);
        const f32 dot = (groundHitInfo.collisionHitPos - wallPos).dot(wallNormal);
        if (!al::isNearZeroOrGreater(dot, 0.001f))
            return false;
    }

    targetPos->set(groundHitInfo.collisionHitPos);
    targetRotate->set(al::getCollidedGroundNormal(actor));
    return true;
}

bool isJustLand(const IUsePlayerCollision* collision) {
    return isCollidedGround(collision) && getCollider(collision)->getTimeInAir() == 1;
}

void calcMovePowerGround(sead::Vector3f* movePower, const IUsePlayerCollision* collision,
                         const sead::Vector3f& pos) {
    al::HitInfo* hitInfo = getGroundHitInfo(collision);
    if (hitInfo->triangle.getCollisionParts() && alCollisionUtil::isCollisionMoving(hitInfo))
        hitInfo->triangle.calcForceMovePower(movePower, pos);
}

void calcMovePowerWall(sead::Vector3f* movePower, const IUsePlayerCollision* collision,
                       const sead::Vector3f& pos) {
    al::HitInfo* hitInfo = getWallHitInfo(collision);
    if (hitInfo->triangle.getCollisionParts() && alCollisionUtil::isCollisionMoving(hitInfo))
        hitInfo->triangle.calcForceMovePower(movePower, pos);
}

void calcMovePowerCeiling(sead::Vector3f* movePower, const IUsePlayerCollision* collision,
                          const sead::Vector3f& pos) {
    al::HitInfo* hitInfo = getCeilingHitInfo(collision);
    if (hitInfo->triangle.getCollisionParts() && alCollisionUtil::isCollisionMoving(hitInfo))
        hitInfo->triangle.calcForceMovePower(movePower, pos);
}

bool isOnGround(const al::LiveActor* actor, const IUsePlayerCollision* collision) {
    if (!isCollidedGround(collision))
        return false;
    sead::Vector3f velocity = al::getVelocity(actor);
    al::tryNormalizeOrZero(&velocity);
    const f32 dot = velocity.dot(getCollidedGroundNormal(collision));
    if (dot <= 0.0f)
        return true;
    return al::isNearZero(dot, 0.001f);
}

bool isOnGroundAndGravity(const al::LiveActor* actor, const IUsePlayerCollision* collision) {
    if (!isCollidedGround(collision))
        return false;
    if (!isOnGround(actor, collision))
        return false;
    return al::getGravity(actor).dot(al::getVelocity(actor)) > 0.0f;
}

bool isCollidedWallVelocity(const al::LiveActor* actor, const IUsePlayerCollision* collision) {
    if (!isCollidedWall(collision))
        return false;
    sead::Vector3f velocity = al::getVelocity(actor);
    al::tryNormalizeOrZero(&velocity);
    return al::isNearZeroOrLess(velocity.dot(getWallHitInfo(collision)->triangle.getFaceNormal()),
                                0.001f);
}

bool isCollidedCeilingVelocity(const al::LiveActor* actor, const IUsePlayerCollision* collision) {
    if (!isCollidedCeiling(collision))
        return false;
    sead::Vector3f velocity = al::getVelocity(actor);
    al::tryNormalizeOrZero(&velocity);
    return al::isNearZeroOrLess(
        velocity.dot(getCeilingHitInfo(collision)->triangle.getFaceNormal()), 0.001f);
}

void calcCollidedNormalSum(sead::Vector3f* normal, const IUsePlayerCollision* collision) {
    normal->x = 0.0f;
    normal->y = 0.0f;
    normal->z = 0.0f;
    if (isCollidedGround(collision))
        *normal += getCollidedGroundNormal(collision);
    if (isCollidedWall(collision))
        *normal += getWallHitInfo(collision)->triangle.getFaceNormal();
    if (isCollidedCeiling(collision))
        *normal += getCeilingHitInfo(collision)->triangle.getFaceNormal();
    normal->multScalar(0.33333f);
}

bool reboundVelocityFromCollision(al::LiveActor* actor, const IUsePlayerCollision* collision,
                                  f32 limit, f32 parallelScale, f32 reboundPower) {
    if (!isCollidedGround(collision) && !isCollidedWall(collision) && !isCollidedCeiling(collision))
        return false;

    sead::Vector3f normal;
    normal.x = 0.0f;
    normal.y = 0.0f;
    normal.z = 0.0f;
    calcCollidedNormalSum(&normal, collision);
    if (!al::isNearZero(normal, 0.001f)) {
        al::normalize(&normal);
        return reboundVelocityByNormal(actor, normal, limit, parallelScale, reboundPower);
    }

    bool result = false;
    if (isCollidedGround(collision))
        result |= reboundVelocityByNormal(actor, getCollidedGroundNormal(collision), limit,
                                          parallelScale, reboundPower);
    if (isCollidedWall(collision))
        result |=
            reboundVelocityByNormal(actor, getWallHitInfo(collision)->triangle.getFaceNormal(),
                                    limit, parallelScale, reboundPower);
    if (isCollidedCeiling(collision))
        result |=
            reboundVelocityByNormal(actor, getCeilingHitInfo(collision)->triangle.getFaceNormal(),
                                    limit, parallelScale, reboundPower);
    return result;
}

bool reboundVelocityByNormal(al::LiveActor* actor, const sead::Vector3f& normal, f32 limit,
                             f32 parallelScale, f32 reboundPower) {
    const sead::Vector3f& velocity = al::getVelocity(actor);
    const f32 dot = normal.dot(velocity);
    if (dot < -parallelScale) {
        *al::getVelocityPtr(actor) -= normal * dot;

        al::getVelocityPtr(actor)->multScalar(reboundPower);

        *al::getVelocityPtr(actor) -= (normal * dot) * limit;
        return true;
    }

    if (dot < 0.0f)
        *al::getVelocityPtr(actor) -= normal * dot;
    return false;
}

// https://decomp.me/scratch/XfQb9 Could only reach 100% match with tricks
u32 reboundVelocityPart(al::LiveActor* actor, const IUsePlayerCollision* collision, f32 limit,
                        f32 groundPower, f32 wallPower, f32 ceilingPower) {
    u32 result = 0;

    if (isCollidedGround(collision)) {
        const sead::Vector3f& normal = getCollidedGroundNormal(collision);
        const f32 normalX = normal.x;
        const f32 normalY = normal.y;
        const f32 normalZ = normal.z;
        const sead::Vector3f& velocity = al::getVelocity(actor);
        const f32 dot = normalX * velocity.x + normalY * velocity.y + normalZ * velocity.z;

        if (limit < 0.0f || dot < -ceilingPower) {
            const f32 rebound = -(limit + 1.0f) * dot;
            sead::Vector3f reboundVelocity;
            reboundVelocity.x = normalX * rebound;
            reboundVelocity.y = normalY * rebound;
            reboundVelocity.z = normalZ * rebound;
            al::addVelocity(actor, reboundVelocity);
            if (limit >= 0.0f) {
                result = 1;
                __asm("");
            }
        } else {
            if (dot < 0.0f) {
                al::addVelocity(actor,
                                sead::Vector3f(normalX * -dot, normalY * -dot, normalZ * -dot));
            }
            result = 0;
        }
    }

    if (isCollidedWall(collision)) {
        const sead::Vector3f& normal = getWallHitInfo(collision)->triangle.getFaceNormal();
        const f32 normalX = normal.x;
        const f32 normalY = normal.y;
        const f32 normalZ = normal.z;
        const sead::Vector3f& velocity = al::getVelocity(actor);
        const f32 dot = normalX * velocity.x + normalY * velocity.y + normalZ * velocity.z;

        if (groundPower < 0.0f || dot < -ceilingPower) {
            const f32 rebound = -(groundPower + 1.0f) * dot;
            sead::Vector3f reboundVelocity;
            reboundVelocity.x = normalX * rebound;
            reboundVelocity.y = normalY * rebound;
            reboundVelocity.z = normalZ * rebound;
            al::addVelocity(actor, reboundVelocity);
            if (__builtin_expect(groundPower >= 0.0f, 0))
                result |= 2;
        } else if (dot < 0.0f) {
            al::addVelocity(actor, sead::Vector3f(normalX * -dot, normalY * -dot, normalZ * -dot));
        }
    }

    if (isCollidedCeiling(collision)) {
        const sead::Vector3f& normal = getCeilingHitInfo(collision)->triangle.getFaceNormal();
        const f32 normalX = normal.x;
        const f32 normalY = normal.y;
        const f32 normalZ = normal.z;
        const sead::Vector3f& velocity = al::getVelocity(actor);
        const f32 dot = normalX * velocity.x + normalY * velocity.y + normalZ * velocity.z;

        if (wallPower < 0.0f || dot < -ceilingPower) {
            const f32 rebound = -(wallPower + 1.0f) * dot;
            sead::Vector3f reboundVelocity;
            reboundVelocity.x = normalX * rebound;
            reboundVelocity.y = normalY * rebound;
            reboundVelocity.z = normalZ * rebound;
            al::addVelocity(actor, reboundVelocity);
            if (__builtin_expect(wallPower >= 0.0f, 0))
                result |= 4;
        } else if (dot < 0.0f) {
            al::addVelocity(actor, sead::Vector3f(normalX * -dot, normalY * -dot, normalZ * -dot));
        }
    }

    return result;
}

void cutVectorCollision(sead::Vector3f* vector, const IUsePlayerCollision* collision, f32 limit) {
    if (isCollidedGround(collision)) {
        const sead::Vector3f& normal = getCollidedGroundNormal(collision);
        al::limitVectorOppositeDir(vector, normal, *vector, vector->length());
        al::addVectorLimit(vector, -getCollidedGroundNormal(collision), limit);
    }
    if (isCollidedWall(collision)) {
        const sead::Vector3f& normal = getWallHitInfo(collision)->triangle.getFaceNormal();
        al::limitVectorOppositeDir(vector, normal, *vector, vector->length());
        const sead::Vector3f& addNormal = getWallHitInfo(collision)->triangle.getFaceNormal();
        al::addVectorLimit(vector, -addNormal, limit);
    }
    if (isCollidedCeiling(collision)) {
        const sead::Vector3f& normal = getCeilingHitInfo(collision)->triangle.getFaceNormal();
        al::limitVectorOppositeDir(vector, normal, *vector, vector->length());
        const sead::Vector3f& addNormal = getCeilingHitInfo(collision)->triangle.getFaceNormal();
        al::addVectorLimit(vector, -addNormal, limit);
    }
}

bool isCollidedNoScaleVelocityWall(const IUsePlayerCollision* collision) {
    return isWallCode(collision, "GrabCeil");
}

bool isCollidedGroundOverAngle(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                               f32 angle) {
    if (!isCollidedGround(collision))
        return false;
    const sead::Vector3f& normal = getCollidedGroundNormal(collision);
    const sead::Vector3f& gravity = al::getGravity(actor);
    const f32 angleCos = cosf(angle * 0.017453292f);
    return !al::isFloorPolygonCos(normal, gravity, angleCos);
}

bool isCollidedGroundLessAngle(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                               f32 angle) {
    if (!isCollidedGround(collision))
        return false;
    const sead::Vector3f& normal = getCollidedGroundNormal(collision);
    const sead::Vector3f& gravity = al::getGravity(actor);
    const f32 angleCos = cosf(angle * 0.017453292f);
    return al::isFloorPolygonCos(normal, gravity, angleCos);
}

bool isCollidedGroundRunAngle(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                              const PlayerConst* playerConst) {
    if (!isCollidedGround(collision))
        return false;
    return isGroundRunAngle(actor, collision, playerConst, isJustLand(collision));
}

bool isGroundRunAngle(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                      const PlayerConst* playerConst, bool useGravity) {
    sead::Vector3f horizontalVelocity;
    horizontalVelocity.x = 0.0f;
    horizontalVelocity.y = 0.0f;
    horizontalVelocity.z = 0.0f;
    const sead::Vector3f& up =
        useGravity ? al::getGravity(actor) : getCollidedGroundNormal(collision);
    al::verticalizeVec(&horizontalVelocity, up, al::getVelocity(actor));
    const f32 minSpeed = playerConst->getHillAngleSpeedMin();
    const f32 maxSpeed = playerConst->getHillAngleSpeedMax();
    const f32 speed = horizontalVelocity.length();
    const f32 rate = al::easeIn(al::calcRate01(speed, minSpeed, maxSpeed));
    const f32 angle =
        al::lerpValue(playerConst->getStandAngleMin(), playerConst->getStandAngleMax(), rate);
    const sead::Vector3f& groundNormal = getCollidedGroundNormal(collision);
    const sead::Vector3f& gravity = al::getGravity(actor);
    const f32 angleCos = cosf(angle * 0.017453292f);
    return al::isFloorPolygonCos(groundNormal, gravity, angleCos);
}

bool isOnGroundRunAngle(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                        const PlayerConst* playerConst) {
    if (!isOnGround(actor, collision))
        return false;
    return isGroundRunAngle(actor, collision, playerConst, isJustLand(collision));
}

bool isOnGroundLessAngle(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                         f32 angle) {
    if (!isOnGround(actor, collision))
        return false;
    const sead::Vector3f& normal = getCollidedGroundNormal(collision);
    const sead::Vector3f& gravity = al::getGravity(actor);
    const f32 angleCos = cosf(angle * 0.017453292f);
    return al::isFloorPolygonCos(normal, gravity, angleCos);
}

bool isLandGroundRunAngle(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                          const PlayerConst* playerConst) {
    if (!isOnGround(actor, collision))
        return false;
    return isGroundRunAngle(actor, collision, playerConst, true);
}

bool isOnGroundSlopeSlideStart(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                               const PlayerConst* playerConst) {
    if (!isOnGround(actor, collision))
        return false;
    const sead::Vector3f& normal = getCollidedGroundNormal(collision);
    const sead::Vector3f& gravity = al::getGravity(actor);
    const f32 angleCos = cosf(playerConst->getSlopeSlideAngleStart() * 0.017453292f);
    return !al::isFloorPolygonCos(normal, gravity, angleCos);
}

bool isOnGroundSlopeSlideEnd(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                             const PlayerConst* playerConst) {
    if (!isOnGround(actor, collision))
        return false;
    const sead::Vector3f& normal = getCollidedGroundNormal(collision);
    const sead::Vector3f& gravity = al::getGravity(actor);
    const f32 angleCos = cosf(playerConst->getSlopeSlideAngleEnd() * 0.017453292f);
    return al::isFloorPolygonCos(normal, gravity, angleCos);
}

bool isOnGroundForceSlideCode(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                              const PlayerConst* playerConst) {
    if (!isOnGround(actor, collision))
        return false;
    if (!isGroundCode(collision, "Slide"))
        return false;
    const sead::Vector3f& normal = getCollidedGroundNormal(collision);
    const sead::Vector3f& gravity = al::getGravity(actor);
    const f32 angleCos = cosf(playerConst->getSlopeSlideAngleEnd() * 0.017453292f);
    return !al::isFloorPolygonCos(normal, gravity, angleCos);
}

bool isOnGroundForceRollingCode(const al::LiveActor* actor, const IUsePlayerCollision* collision) {
    return isOnGround(actor, collision) && isGroundCode(collision, "Rolling");
}

bool isOnGroundSkateCode(const al::LiveActor* actor, const IUsePlayerCollision* collision) {
    return isOnGround(actor, collision) && isGroundCode(collision, "Skate");
}

bool isAutoRunOnGroundSkateCode(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                                f32 speed) {
    if (!isOnGroundSkateCode(actor, collision))
        return false;

    sead::Vector3f velocity = al::getVelocity(actor);
    if (isJustLand(collision)) {
        sead::Vector3f gravity = -al::getGravity(actor);
        al::alongVectorNormalH(&velocity, velocity, gravity, getCollidedGroundNormal(collision));
    } else {
        al::verticalizeVec(&velocity, getCollidedGroundNormal(collision), velocity);
    }
    return velocity.length() >= speed;
}

bool isPressedCollision(const IUsePlayerCollision* collision) {
    return getCollider(collision)->isPressedGround() || getCollider(collision)->isPressedWall();
}

bool isPressedGround(const IUsePlayerCollision* collision) {
    return getCollider(collision)->isPressedGround();
}

bool isPressedWall(const IUsePlayerCollision* collision) {
    return getCollider(collision)->isPressedWall();
}

const sead::Vector3f& getPressCollisionNormal(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getPressCollisionNormal();
}

const sead::Vector3f& getPressCollisionHitPos(const IUsePlayerCollision* collision) {
    return getCollider(collision)->getPressCollisionHitPos();
}

void setColliderFilterCollisionParts(IUsePlayerCollision* collision,
                                     const al::CollisionPartsFilterBase* filter) {
    collision->getPlayerCollider()->setCollisionPartsFilter(filter);
}

void createAndSetColliderFilterSpecialPurpose(IUsePlayerCollision* collision, const char* name) {
    setColliderFilterCollisionParts(collision, new al::CollisionPartsFilterSpecialPurpose(name));
}

void startHitReactionLandIfLanding(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                                   bool isSkip) {
    if (!isSkip && isJustLand(collision))
        al::startHitReaction(actor, "Land");
}

void startHitReactionLandJumpIfLanding(const al::LiveActor* actor,
                                       const IUsePlayerCollision* collision, bool isSkip) {
    if (!isSkip && isJustLand(collision))
        al::startHitReaction(actor, "着地[走り]");
}

void startHitReactionLandRunIfLanding(const al::LiveActor* actor,
                                      const IUsePlayerCollision* collision, bool isSkip) {
    if (!isSkip && isJustLand(collision))
        al::startHitReaction(actor, "着地[走り]");
}

bool isAboveGround(const IUsePlayerHeightCheck* heightCheck) {
    return heightCheck->isAboveGround();
}

f32 getGroundHeight(const IUsePlayerHeightCheck* heightCheck) {
    return heightCheck->getGroundHeight();
}

void updateCollider(al::LiveActor* actor, IUsePlayerCollision* collision,
                    const sead::Vector3f& move) {
    const bool isNoCollide = al::isNoCollide(actor);
    sead::Vector3f* trans = al::getTransPtr(actor);
    if (isNoCollide) {
        *trans += move;
        collision->getPlayerCollider()->onInvalidate();
        return;
    }

    sead::Vector3f fix = collision->getPlayerCollider()->collide(move);
    *trans += fix;
}

bool calcGroundHeight(f32* height, sead::Vector3f* normal, const al::IUseCollision* collision,
                      const sead::Vector3f& start, const sead::Vector3f& up, f32 minHeight,
                      f32 maxHeight) {
    normal->set(up);

    const f32 checkHeight = minHeight + maxHeight + 5.0f;
    sead::Vector3f arrowStart = start + up * minHeight;
    sead::Vector3f arrow = -(up * checkHeight);
    const s32 hitNum =
        alCollisionUtil::checkStrikeArrow(collision, arrowStart, arrow, nullptr, nullptr);
    if (hitNum < 1)
        return false;

    bool isHit = false;
    f32 bestHeight = maxHeight;

    for (s32 i = 0; i < hitNum; ++i) {
        const al::ArrowHitInfo& arrowHit = alCollisionUtil::getStrikeArrowInfo(collision, i);
        const al::HitInfo* hitInfo = *arrowHit;
        const sead::Vector3f& hitNormal = hitInfo->triangle.getFaceNormal();
        if (hitNormal.dot(up) < 0.5f)
            continue;

        const f32 currentHeight = (hitInfo->collisionHitPos - start).length();
        if (currentHeight < bestHeight) {
            bestHeight = currentHeight;
            isHit = true;
            normal->set(hitNormal);
        }
    }

    if (!isHit)
        return false;

    *height = sead::Mathf::clampMin(bestHeight, 0.0f);
    return true;
}

}  // namespace rs

namespace CollisionShapeUtil {

void setShapeOffsetAllArrow(IUsePlayerCollision* collision, const sead::Vector3f& offset) {
    CollisionShapeKeeper* keeper = collision->getPlayerCollider()->getCollisionShapeKeeper();
    const s32 shapeNum = keeper->getShapeNum();
    for (s32 i = 0; i < shapeNum; ++i) {
        const CollisionShapeInfoBase* shape = keeper->getShapeInfoBase(i);
        if (CollisionShapeFunction::isShapeArrow(shape))
            CollisionShapeFunction::updateShapeOffset(const_cast<CollisionShapeInfoBase*>(shape),
                                                      offset);
    }
    keeper->updateShape();
}

void setShapeDiskHalfHeight(IUsePlayerCollision* collision, const char* name, f32 halfHeight) {
    CollisionShapeKeeper* keeper = collision->getPlayerCollider()->getCollisionShapeKeeper();
    CollisionShapeInfoDisk* disk = keeper->getShapeInfoDisk(keeper->findShapeInfoIndex(name));
    disk->updateDiskShape(disk->getDiskRadius(), disk->getDiskOffset(), halfHeight);
    keeper->updateShape();
}

}  // namespace CollisionShapeUtil
