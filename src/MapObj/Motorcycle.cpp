#include "MapObj/Motorcycle.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Camera/TransCameraSubTarget.h"
#include "Library/Collision/Collider.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/Item/ItemUtil.h"
#include "Library/Joint/JointControllerKeeper.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nature/NatureUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Project/Camera/CameraSubTargetTurnParam.h"

#include "MapObj/MotorcyclePlayerAnimator.h"
#include "Player/CollidedShapeResult.h"
#include "Player/CollisionShapeInfo.h"
#include "Player/CollisionShapeKeeper.h"
#include "Player/PlayerCollider.h"
#include "System/GameDataFunction.h"
#include "System/GameDataUtil.h"
#include "Util/CameraUtil.h"
#include "Util/InputInterruptTutorialUtil.h"
#include "Util/NpcAnimUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerPuppetFunction.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Motorcycle, Wait)
NERVE_IMPL(Motorcycle, Creep)
NERVE_IMPL(Motorcycle, Fall)
NERVE_IMPL(Motorcycle, Jump)
NERVE_IMPL(Motorcycle, Reaction)
NERVE_IMPL(Motorcycle, Reset)
NERVE_IMPL_(Motorcycle, ResetNoReaction, Reset)
NERVE_IMPL_(Motorcycle, RideStartLeft, RideStart)
NERVE_IMPL_(Motorcycle, RideStartRight, RideStart)
NERVE_IMPL(Motorcycle, RideStartOn)
NERVE_IMPL(Motorcycle, RideWait)
NERVE_IMPL(Motorcycle, RideWaitJump)
NERVE_IMPL(Motorcycle, RideWaitLand)
NERVE_IMPL(Motorcycle, RideRunStart)
NERVE_IMPL(Motorcycle, RideRun)
NERVE_IMPL(Motorcycle, RideRunCollide)
NERVE_IMPL(Motorcycle, RideRunFall)
NERVE_IMPL(Motorcycle, RideRunWheelie)
NERVE_IMPL(Motorcycle, RideRunLand)
NERVE_IMPL(Motorcycle, RideRunJump)
NERVE_IMPL(Motorcycle, RideRunBoundStart)
NERVE_IMPL(Motorcycle, RideRunBound)
NERVE_IMPL(Motorcycle, RideRunClash)
NERVE_IMPL(Motorcycle, RideParkingSnap)
NERVE_IMPL(Motorcycle, RideParkingStart)
NERVE_IMPL(Motorcycle, RideParking)
NERVE_IMPL(Motorcycle, RideParkingAfter)

NERVES_MAKE_STRUCT(Motorcycle, Wait, Jump, RideWaitJump, RideRunFall, RideRunWheelie, RideRunJump,
                   RideRunBoundStart, RideRunCollide, RideRunLand, Fall, ResetNoReaction,
                   RideStartLeft, RideStartRight, RideStartOn, RideWait, Creep, Reaction, Reset,
                   RideWaitLand, RideRunStart, RideRun, RideRunBound)
NERVES_MAKE_NOSTRUCT(Motorcycle, RideRunClash, RideParking, RideParkingAfter, RideParkingStart,
                     RideParkingSnap)
}  // namespace

const sead::Vector3f forceField5 = {0.0f, 140.0f, -85.0f};
const sead::Vector3f sFrontArrowBase = {0.0f, -70.0f, 0.0f};
const sead::Vector3f sBackSphere = {0.0f, 80.0f, -120.0f};
const sead::Vector3f sCenterFaceSphere = {0.0f, 80.0f, -60.0f};
const sead::Vector3f sFrontCenterArrow = {0.0f, 50.0f, 10.0f};
const sead::Vector3f sFrontLeftArrow = {-25.0f, 50.0f, -150.0f};
const sead::Vector3f sFrontRightArrow = {25.0f, 50.0f, -150.0f};
const sead::Vector3f sActorOffset = {0.0f, 50.0f, -80.0f};
const sead::Vector3f sGroundNormal = {0.0f, -1.0f, 0.0f};
const sead::Vector3f sItemSpawnOffset = {0.0f, 150.0f, -90.0f};

static inline void reset(Motorcycle* actor) {
    al::setVelocityZero(actor);
    al::invalidateHitSensors(actor);
    al::hideModelIfShow(actor);
    al::setNerve(actor, &NrvMotorcycle.Reset);
}

bool tryHandleGroundCollision(Motorcycle* actor) {
    if (rs::isOnGround(actor, actor)) {
        if (al::isInWater(actor)) {
            reset(actor);
            return true;
        }

        if (rs::isCollisionCodeJump(actor)) {
            al::HitSensor* sensor = rs::tryGetCollidedGroundSensor(actor);
            if (sensor)
                rs::sendMsgPlayerTouchFloorJumpCode(sensor, al::getHitSensor(actor, "PlayerBody"));
            al::setNerve(actor, &NrvMotorcycle.Jump);
            return true;
        }

        al::setNerve(actor, &NrvMotorcycle.Creep);
        return true;
    }

    return false;
}

void handleCeilingCollision(Motorcycle* actor) {
    if (rs::isCollidedCeiling(actor) && al::getVelocity(actor).dot(sGroundNormal) < 0.0f) {
        sead::Vector3f* velocity = al::getVelocityPtr(actor);
        al::verticalizeVec(velocity, sGroundNormal, *velocity);

        al::HitSensor* sensor = rs::tryGetCollidedCeilingSensor(actor);
        if (sensor)
            al::sendMsgPlayerUpperPunch(sensor, al::getHitSensor(actor, "PlayerAttack"));
    }
}

bool TryJump(Motorcycle* actor, IUsePlayerPuppet* playerPuppet) {
    if (!rs::isOnGround(actor, actor))
        return false;

    if (rs::isCollisionCodeJump(actor)) {
        al::HitSensor* sensor = rs::tryGetCollidedGroundSensor(actor);
        if (sensor)
            rs::sendMsgPlayerTouchFloorJumpCode(sensor, al::getHitSensor(actor, "PlayerBody"));
        al::setNerve(actor, &NrvMotorcycle.RideRunJump);
        return true;
    }

    if (rs::isPuppetTrigJumpButton(playerPuppet)) {
        al::setNerve(actor, &NrvMotorcycle.RideWaitJump);
        return true;
    }

    return false;
}

bool TryJumpWithCameraAbsorb(Motorcycle* actor, IUsePlayerPuppet* playerPuppet) {
    if (!rs::isOnGround(actor, actor))
        return false;

    if (rs::isCollisionCodeJump(actor)) {
        al::HitSensor* sensor = rs::tryGetCollidedGroundSensor(actor);
        if (sensor)
            rs::sendMsgPlayerTouchFloorJumpCode(sensor, al::getHitSensor(actor, "PlayerBody"));

        al::setNerve(actor, &NrvMotorcycle.RideRunJump);
        return true;
    }

    if (rs::isPuppetTrigJumpButton(playerPuppet)) {
        if (sead::Mathf::rad2deg(sead::Mathf::asin(al::getFront(actor).y)) > 17.5f)
            al::requestStopCameraVerticalAbsorb(actor);
        al::setNerve(actor, &NrvMotorcycle.RideRunJump);
        return true;
    }

    return false;
}

void endRide(Motorcycle* actor, MotorcyclePlayerAnimator* playerAnimator,
             al::CameraTargetBase* cameraTarget, al::TransCameraSubTarget* cameraSubTarget) {
    playerAnimator->endBind();
    alPadRumbleFunction::stopPadRumbleDirectValue(actor, -1);
    rs::tryCloseBindTutorial(actor);
    al::invalidateHitSensors(actor);
    al::validateHitSensorBindableAll(actor);
    al::validateHitSensorRideAll(actor);
    al::hideSilhouetteModel(actor);
    al::validateOcclusionQuery(actor);
    al::showModelIfHide(actor);
    al::resetCameraTarget(actor, cameraTarget);
    al::offCameraRideObj(actor);
    al::invalidateDepthShadowMap(actor);
    al::onDepthShadowModel(actor);
    if (al::isActiveCameraSubTarget(cameraSubTarget))
        al::resetCameraSubTarget(actor, cameraSubTarget);

    if (!tryHandleGroundCollision(actor))
        al::setNerve(actor, &NrvMotorcycle.Fall);
}

void syncPuppetPose(IUsePlayerPuppet* playerPuppet, Motorcycle* actor, const sead::Quatf& startQuat,
                    const sead::Vector3f& startTrans) {
    sead::Quatf jointQuat = sead::Quatf::unit;
    sead::Vector3f trans = {0.0f, 0.0f, 0.0f};
    sead::Matrix34f* jointMtx = al::getJointMtxPtr(actor, "JointRoot");

    jointMtx->toQuat(jointQuat);
    jointQuat.normalize();
    jointMtx->getTranslation(trans);

    if (al::isNerve(actor, &NrvMotorcycle.RideStartLeft) ||
        al::isNerve(actor, &NrvMotorcycle.RideStartRight)) {
        f32 rate = al::calcNerveRate(actor, rs::getPuppetAnimFrameMax(playerPuppet));
        al::slerpQuat(&jointQuat, startQuat, jointQuat, rate);
        al::lerpVec(&trans, startTrans, trans, rate);
    }

    rs::setPuppetTrans(playerPuppet, trans);
    rs::setPuppetQuat(playerPuppet, jointQuat);
}

// NOTE: This might be a sead function
inline f32 getAngleBetweenTwoVectors(const sead::Vector3f& a, const sead::Vector3f& b) {
    f32 x = a.dot(b);
    f32 y = a.cross(b).length();
    return sead::Mathf::atan2(y, x);
}

static inline bool isTouchingGround(MotorcycleParams* params) {
    return al::isNormalize(params->groundNormalAvg) && params->isTouchingGround;
}

void updateOrientation(Motorcycle* actor, f32 steerAngle) {
    sead::Vector3f normal;
    if (rs::isCollidedGround(actor))
        normal = -rs::getCollidedGroundNormal(actor);
    else
        normal = -actor->getParams()->groundNormal;

    if (!al::isParallelDirection(al::getGravity(actor), normal)) {
        f32 startAngle = 1.0f;
        if (rs::isCollidedGround(actor)) {
            startAngle = al::lerpValue(
                1.0f, 0.3f,
                al::normalize(sead::Mathf::rad2deg(sead::Mathf::sin(al::getFront(actor).y)), 11.0f,
                              15.0f));
        }
        steerAngle = al::lerpValue(startAngle, 1.0, al::normalize(steerAngle, 10.0f, 40.0f));
        sead::Vector3f normalGravity = al::getGravity(actor).cross(normal);
        al::normalize(&normalGravity);

        sead::Quatf steerQuat;
        steerQuat.setAxisRadian(normalGravity,
                                0.05f * getAngleBetweenTwoVectors(al::getGravity(actor), normal) *
                                    steerAngle);

        al::getGravityPtr(actor)->setRotated(steerQuat, al::getGravity(actor));
        al::normalize(al::getGravityPtr(actor));

        sead::Vector3f* frontPtr = al::getFrontPtr(actor);
        al::verticalizeVec(frontPtr, al::getGravity(actor), *frontPtr);
        al::normalize(al::getFrontPtr(actor));
    }
    MotorcycleParams* params = actor->getParams();

    if (!isTouchingGround(params) && params->isTouchingWall) {
        sead::Vector3f front = al::getFront(actor);
        al::verticalizeVec(&front, sGroundNormal, front);
        if (al::tryNormalizeOrZero(&front)) {
            if (!(al::getFront(actor).y > 0.0f)) {
                al::rotateVectorDegree(&front, front, front.cross({0.0f, -1.0f, 0.0f}), 45.0f);
                al::normalize(&front);
            }

            al::turnVecToVecDegree(&front, al::getFront(actor), front, 2.0f);
            sead::Vector3f* frontPtr = al::getFrontPtr(actor);
            al::turnVecToVecRate(frontPtr, *frontPtr, front, 0.15f);
            al::normalize(al::getFrontPtr(actor));

            sead::Vector3f* gravityPtr = al::getGravityPtr(actor);
            al::verticalizeVec(gravityPtr, al::getFront(actor), *gravityPtr);
            al::normalize(al::getGravityPtr(actor));
        }
    }

    if (isTouchingGround(params) && params->isTouchingWall)
        rs::tryFollowRotateFrontAxisUpIfCollidedGround(actor, actor);
}

void updateVelocity(Motorcycle* actor, const sead::Vector3f& horizontalAxis,
                    const sead::Vector3f& verticalAxis) {
    sead::Vector3f velH = {0.0f, 0.0f, 0.0f};
    sead::Vector3f velV = {0.0f, 0.0f, 0.0f};
    al::separateVectorHV(&velH, &velV, horizontalAxis, al::getVelocity(actor));

    sead::Vector3f front = al::getFront(actor);
    al::verticalizeVec(&front, verticalAxis, front);
    al::tryNormalizeOrZero(&front);

    al::setVelocity(actor, (velH.length() * front) + (velV.length() * verticalAxis));
}

void calcInputStick(sead::Vector2f* outStick, Motorcycle* actor, IUsePlayerPuppet* playerPuppet) {
    if (!actor->isStickWorldPose()) {
        outStick->set(rs::getPuppetMoveStick(playerPuppet));
        return;
    }

    outStick->set(0.0f, 0.0f);
    sead::Vector3f vecView = {0.0f, 0.0f, 0.0f};
    al::calcVecViewInput(&vecView, rs::getPuppetMoveStick(playerPuppet), -al::getGravity(actor),
                         al::getViewMtxPtr(actor, 0));

    sead::Vector3f sideDir = {0.0f, 0.0f, 0.0f};
    al::calcSideDir(&sideDir, actor);
    al::verticalizeVec(&sideDir, sead::Vector3f::ey, sideDir);
    al::normalize(&sideDir);

    sead::Vector3f front = al::getFront(actor);
    al::verticalizeVec(&front, sead::Vector3f::ey, front);
    al::normalize(&front);

    sead::Vector3f parallelVec = {0.0f, 0.0f, 0.0f};
    al::parallelizeVec(&parallelVec, sideDir, vecView);
    sead::Vector3f diff = vecView - parallelVec;

    if (!al::isNearZero(parallelVec)) {
        f32 signX = (parallelVec.dot(sideDir) > 0.0f) ? -1.0f : 1.0f;
        outStick->x = signX * parallelVec.length();
    }

    if (!al::isNearZero(diff)) {
        f32 signY = (diff.dot(front) > 0.0f) ? 1.0f : -1.0f;
        outStick->y = signY * diff.length();
    }
}

void applyAirPhysics(Motorcycle* actor, IUsePlayerPuppet* playerPuppet, f32 gravityAccel) {
    sead::Vector2f stick = {0.0f, 0.0f};
    calcInputStick(&stick, actor, playerPuppet);

    al::rotateVectorDegreeY(al::getFrontPtr(actor), stick.x * -0.5f);
    al::normalize(al::getFrontPtr(actor));

    sead::Vector3f front = al::getFront(actor);
    front.y = 0;
    al::tryNormalizeOrZero(&front);
    const sead::Vector3f& vel = al::getVelocity(actor);
    sead::Vector3f velocity =
        front * sead::Mathf::sqrt(sead::Mathf::pow(vel.x, 2) + sead::Mathf::pow(vel.z, 2));

    f32 friction = 0.95f;
    if (rs::isPuppetHoldActionButton(playerPuppet))
        friction = 1.0f;

    al::setVelocity(actor, velocity * friction + (al::getVelocity(actor).y - gravityAccel) *
                                                     sead::Vector3f::ey * 0.95f);
}

bool tryHandleWallCollision(Motorcycle* actor) {
    if (rs::isCollisionCodePoleClimbWall(actor))
        return false;

    s32 hitNum = rs::getWallHitInfoNum(actor);
    s32 i = 0;
    bool isCollision = false;
    for (; i < hitNum; i++) {
        isCollision |= rs::sendMsgMotorcycleDashCollide(rs::getWallHitInfoSensor(actor, i),
                                                        al::getHitSensor(actor, "PlayerBody"));
    }
    if (isCollision) {
        al::LiveActor* host = al::getSensorHost(rs::tryGetCollidedWallSensor(actor));
        if (al::isDead(host))
            return false;
        al::CollisionParts* parts = host->getCollisionParts();
        if (parts && !parts->isValidCollision())
            return false;
    }

    sead::Vector3f front = {al::getFront(actor).x, 0.0f, al::getFront(actor).z};
    if (!al::tryNormalizeOrZero(&front))
        return false;

    if (sead::Mathf::cos(sead::Mathf::deg2rad(15.0f)) <
        front.dot(-rs::getCollidedWallNormal(actor))) {
        al::setVelocityZeroX(actor);
        al::setVelocityZeroZ(actor);
        al::setNerve(actor, &RideRunClash);
        return true;
    }

    return false;
}

bool tryHandleHighSpeedWallCollision(Motorcycle* actor, const sead::Vector3f& transDelta) {
    if (!rs::isCollidedWallVelocity(actor, actor))
        return false;

    s32 hitNum = rs::getWallHitInfoNum(actor);
    bool isCollision = false;
    for (s32 i = 0; i < hitNum; i++) {
        isCollision |= rs::sendMsgMotorcycleDashCollide(rs::getWallHitInfoSensor(actor, i),
                                                        al::getHitSensor(actor, "PlayerBody"));
    }
    if (isCollision)
        return false;

    if (al::calcSpeedMax(1.8f, 0.95f) * 0.9f < al::calcSpeedH(actor) &&
        tryHandleWallCollision(actor))
        return true;

    sead::Vector3f delta = transDelta;
    al::verticalizeVec(&delta, al::getGravity(actor), delta);
    if (delta.length() >= 10.0f) {
        al::setNerve(actor, &NrvMotorcycle.RideRunCollide);
        return true;
    }
    return false;
}

void updateSteerAngleAndAccel(Motorcycle* actor, IUsePlayerPuppet* playerPuppet,
                              AccelerationState* accelerationState, f32* steerAngle) {
    f32 accelRate =
        al::lerpValue(1.0f, 0.75f, al::normalize(actor->getParams()->framesInAir, 0, 15));
    f32 speed = al::normalize(al::calcSpeedH(actor), 0.0f, al::calcSpeedMax(1.8f, 0.95f));
    f32 lerpSpeed;
    if (actor->isStickWorldPose())
        lerpSpeed = al::lerpValue(3.4f, 2.6f, speed);
    else
        lerpSpeed = al::lerpValue(3.6f, 2.8f, speed);

    if (!al::isNearZero(rs::getPuppetMoveStick(playerPuppet).x)) {
        accelRate *= lerpSpeed * al::normalizeAbs(*steerAngle, 0.0f, 55.0f);
        if (rs::isCollidedWall(actor)) {
            sead::Vector3f front = al::getFront(actor);
            al::rotateVectorDegree(&front, front, al::getGravity(actor), accelRate);
            sead::Vector3f diff = front - al::getFront(actor);
            if (al::tryNormalizeOrZero(&diff) && actor->getParams()->isInFront(diff) &&
                actor->getParams()->isInBack(diff))
                accelRate = 0.0;
        }
    } else {
        al::AreaObj* frontSnapArea = al::tryFindAreaObj(actor, "MotorcycleFrontSnapArea");
        if (frontSnapArea) {
            sead::Vector3f front = {0.0f, 0.0f, 0.0f};
            al::getAreaObjDirFront(&front, frontSnapArea);
            accelRate =
                al::calcAngleOnPlaneDegree(front, al::getFront(actor), -al::getGravity(actor)) *
                sead::Mathf::deg2rad(2.0f);
        } else {
            accelRate = 0.0f;
        }
    }

    sead::Vector3f* frontPtr = al::getFrontPtr(actor);
    al::rotateVectorDegree(frontPtr, *frontPtr, al::getGravity(actor), accelRate);
    al::normalize(al::getFrontPtr(actor));
    accelerationState->isAccelerating = rs::isPuppetHoldActionButton(playerPuppet);
    if (accelerationState->isAccelerating)
        accelRate =
            al::lerpValue(1.0f, 0.3f, al::normalize(sead::Mathf::abs(accelRate), 0.0f, 0.7f));
    else
        accelRate = -0.75f;

    accelerationState->accelRate += accelRate;
    accelerationState->accelRate = sead::Mathf::clamp(accelerationState->accelRate, 0.0f, 5.0f);
}

bool TryWheelieOrFall(Motorcycle* actor) {
    if (rs::isCollidedGround(actor))
        return false;
    if (actor->getParams()->framesInAir > 1) {
        if (sead::Mathf::rad2deg(sead::Mathf::sin(al::getFront(actor).y)) > 17.5f) {
            sead::Vector3f trans = {0.0f, 0.0f, 0.0f};
            al::calcTransLocalOffset(&trans, actor, sFrontCenterArrow);

            if (!alCollisionUtil::getFirstPolyOnArrow(actor, nullptr, nullptr, trans,
                                                      al::getGravity(actor) * 125.0f, nullptr,
                                                      nullptr)) {
                al::limitVelocityDirSign(actor, al::getGravity(actor), 0.0f);
                al::setNerve(actor, &NrvMotorcycle.RideRunWheelie);
                return true;
            }
        }
    }

    MotorcycleParams* params = actor->getParams();
    if (params->framesInAir >= 6) {
        updateVelocity(actor, -params->groundNormal, sGroundNormal);
        al::limitVelocityDirSign(actor, sGroundNormal, 3.0f);
        al::setNerve(actor, &NrvMotorcycle.RideRunFall);
        return true;
    }
    return false;
}

static inline f32 getGroundDistance(Motorcycle* actor, MotorcycleParams* params) {
    sead::Vector3f distance = al::getTrans(actor) - params->lastGroundPos;
    al::parallelizeVec(&distance, al::getGravity(actor), distance);
    return distance.dot(al::getGravity(actor));
}

bool tryBound(Motorcycle* actor) {
    if (!rs::isOnGround(actor, actor))
        return false;

    MotorcycleParams* params = actor->getParams();
    if (!rs::isCollidedGround(actor) || params->isOnGround)
        return false;

    if (getGroundDistance(actor, params) > 700.0f) {
        al::startHitReaction(actor, "バウンド");
        f32 distance = al::normalize(getGroundDistance(actor, actor->getParams()), 700.0f, 5000.0f);
        al::setVelocityY(actor, al::lerpValue(25.0f, 40.0f, distance));
        al::setNerve(actor, &NrvMotorcycle.RideRunBoundStart);
        return true;
    }
    return false;
}

bool tryLand(Motorcycle* actor) {
    if (rs::isOnGround(actor, actor)) {
        updateVelocity(actor, sGroundNormal, -rs::getCollidedGroundNormal(actor));
        al::limitVelocityDirSign(actor, -rs::getCollidedGroundNormal(actor), 5.0f);

        if (al::isGreaterEqualStep(actor, 15)) {
            MotorcycleParams* params = actor->getParams();
            if (rs::isCollidedGround(actor) && !params->isOnGround) {
                if (getGroundDistance(actor, params) > 150.0f) {
                    updateVelocity(actor, sGroundNormal, -rs::getCollidedGroundNormal(actor));
                    al::limitVelocityDirSign(actor, -rs::getCollidedGroundNormal(actor), 5.0f);
                    al::setNerve(actor, &NrvMotorcycle.RideRunLand);
                    return true;
                }
            }
        }
        al::setNerve(actor, &NrvMotorcycle.RideRun);
        return true;
    }

    return false;
}

void startGetOff(Motorcycle* actor, IUsePlayerPuppet** playerPuppet, s32* mGetOffDelay,
                 MotorcyclePlayerAnimator* playerAnimator, al::CameraTargetBase* cameraTarget,
                 al::TransCameraSubTarget* cameraSubTarget) {
    const sead::Vector3f groundNormal = {0.0f, -1.0f, 0.0f};

    al::limitVelocityDirSign(actor, -groundNormal, 0.0f);

    rs::showPuppetShadow(*playerPuppet);
    rs::validatePuppetReceivePush(*playerPuppet);

    bool actorIsNull = actor == nullptr;  // Hack early load for getViewMtxPtr
    MotorcycleParams* params = actor->getParams();
    if (params->frontContactPoints.size() > 0 && params->backContactPoints.size() > 0) {
        sead::Vector3f offset = {0.0f, 0.0f, 0.0f};
        al::calcTransLocalOffset(&offset, actor, sActorOffset);
        rs::resetPuppetPosition(*playerPuppet, offset);
    }

    sead::Vector2f stick = {rs::getPuppetMoveStick(*playerPuppet).x,
                            -rs::getPuppetMoveStick(*playerPuppet).y};

    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    sead::Vector3f jumpDir;
    if (al::isNearZero(stick, 0.2f) ||
        !al::calcDirViewInput(&front, stick, sGroundNormal, al::getViewMtxPtr(actor, 0))) {
        al::verticalizeVec(&front, -groundNormal, al::getFront(actor));
        al::normalize(&front);
        jumpDir.set(-front);
    } else {
        jumpDir.set(front);
    }

    rs::setPuppetFront(*playerPuppet, front);
    rs::setPuppetUp(*playerPuppet, -groundNormal);

    rs::endBindJumpAndPuppetNull(playerPuppet, sead::Vector3f::ey * 20.0f + jumpDir * 9.0f, 3);

    *mGetOffDelay = 5;
    endRide(actor, playerAnimator, cameraTarget, cameraSubTarget);
}

bool tryParking(Motorcycle* actor, IUsePlayerPuppet* playerPuppet, ParkingParams* params,
                MotorcyclePose* pose) {
    if (rs::isPuppetHoldActionButton(playerPuppet) || al::calcSpeedH(actor) > 8.0f ||
        !rs::isCollidedGround(actor))
        return false;

    al::HitSensor* groundSensor = rs::tryGetCollidedGroundSensor(actor);
    if (!groundSensor)
        return false;

    sead::Vector3f backDir = {0.0f, 0.0f, 0.0f};
    al::calcBackDir(&backDir, al::getSensorHost(groundSensor));
    if (!(sead::Mathf::abs(al::getFront(actor).dot(backDir)) <
          sead::Mathf::cos(sead::Mathf::deg2rad(25.0f)))) {
        al::parallelizeVec(&backDir, backDir, al::getFront(actor));
        al::normalize(&backDir);

        sead::Quatf quat = sead::Quatf::unit;
        sead::Vector3f up = {0.0f, 0.0f, 0.0f};
        sead::Vector3f side = {0.0f, 0.0f, 0.0f};
        al::calcUpDir(&up, al::getSensorHost(groundSensor));
        al::makeQuatFrontUp(&quat, backDir, up);
        al::calcQuatSide(&side, quat);

        sead::Vector3f targetPos = {0.0f, 30.0f, 75.0f};
        targetPos.rotate(quat);
        targetPos += al::getActorTrans(groundSensor);

        sead::Vector3f distanceH = {0.0f, 0.0f, 0.0f};
        al::parallelizeVec(&distanceH, side, al::getTrans(actor) - targetPos);

        if (distanceH.length() > 100.0f)
            return false;

        sead::Vector3f distanceV = {0.0f, 0.0f, 0.0f};
        al::parallelizeVec(&distanceV, -backDir, al::getTrans(actor) - targetPos);

        if (distanceV.length() > 100.0f)
            return false;

        if (rs::sendMsgMotorcycleCollideParkingLot(groundSensor,
                                                   al::getHitSensor(actor, "PlayerBody"))) {
            params->actor = al::getSensorHost(groundSensor);
            params->pose = *pose;

            al::calcQuat(&params->quatA, actor);
            params->quatB.set(quat);
            params->mCameraSubTargetPos.set(al::getTrans(actor));
            params->vectorB.set(targetPos);

            al::offCollide(actor);
            al::setVelocityZero(actor);
            al::setNerve(actor, &RideParkingSnap);
            return true;
        }
    }

    return false;
}

void updateSeRumble(SeRumbleState* valueB, Motorcycle* actor, IUsePlayerPuppet* playerPuppet,
                    f32 steerAngle, bool isJumping) {
    f32 throttleScale;
    if (rs::isPuppetHoldActionButton(playerPuppet)) {
        throttleScale = 1.0f;
        valueB->rumble = sead::Mathi::clampMax(valueB->rumble + 1, 240);
    } else {
        throttleScale = 0.6f;
        valueB->rumble = sead::Mathi::clampMin(valueB->rumble - 15, 0);
    }

    valueB->volume = al::calcSpeedH(actor);
    f32 rumbleFade = sead::Mathf::clamp(valueB->rumble / -240.0f + 1.0f, 0.25f, 1.0f);
    f32 baseRumbleAmp = throttleScale * rumbleFade * 0.35f;

    f32 vibrationWave = 0.0f;
    if (valueB->rumble < 40) {
        f32 phase = (valueB->rumble / 40.0f - 1.0f) * sead::Mathf::pi();
        vibrationWave = (phase * sinf(phase)) / 1.8f;
    }

    f32 rumbleScale = baseRumbleAmp * (vibrationWave * 1.45f + 1.0f);
    f32 lowRumble = rumbleScale;
    f32 speedOffset = sead::Mathf::clampMin(valueB->volume - 5.0f, 0.0f);
    f32 baseFrequency = (vibrationWave * 0.08f + 1.0f) * ((speedOffset / 70.0f + 1.0f) * 168.0f);
    f32 highRumble = baseFrequency;
    f32 frequency = 160.0f;
    if (isJumping) {
        frequency = 272.0f;
        highRumble = baseFrequency * 1.7f;
        lowRumble = rumbleScale * 0.6f;
    }

    al::holdSeWithParam(actor, "CurveLv", valueB->volume * steerAngle, "回転角(Degree)");

    f32 leftWeight = steerAngle > 0.0f ? 0.0f : 0.01f;
    leftWeight *= steerAngle;
    leftWeight = 0.7f - leftWeight;
    f32 rightWeight = (steerAngle > 0.0f ? 0.01f : 0.0f) * steerAngle;

    alPadRumbleFunction::startPadRumbleDirectValue(actor, frequency, highRumble, lowRumble,
                                                   lowRumble, leftWeight * 0.7f,
                                                   (rightWeight + 0.7f) * 0.7f, -1);

    f32 throttleVolume = rs::isPuppetHoldActionButton(playerPuppet) ? 9.0f : 0.0f;
    if (rs::isPuppetHoldActionButton(playerPuppet))
        al::holdSeWithParam(actor, "MoveStartLv", throttleVolume + valueB->volume, "");
    else
        al::holdSeWithParam(actor, "MoveEndLv", throttleVolume + valueB->volume, "");
    al::holdSeWithParam(actor, "MoveLv", throttleVolume + valueB->volume, "");
    al::holdSeWithParam(actor, "IdleLv", throttleVolume + valueB->volume, "");
}

static inline f32 getJointDistance(Motorcycle* actor, const char* jointNameA,
                                   const char* jointNameB) {
    sead::Vector3f frontTirePos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f backTirePos = {0.0f, 0.0f, 0.0f};
    al::calcJointPos(&frontTirePos, actor, jointNameA);
    al::calcJointPos(&backTirePos, actor, jointNameB);
    return (frontTirePos - backTirePos).length();
}

void updateAirOrientation(Motorcycle* actor) {
    if (al::getVelocity(actor).y > 0.0f)
        return;

    sead::Vector3f hitPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f checkPos =
        (al::getTrans(actor) -
         al::getFront(actor) * getJointDistance(actor, "FrontTire", "BackTire")) -
        al::getGravity(actor) * 150.0f;
    if (!alCollisionUtil::getFirstPolyOnArrow(actor, &hitPos, nullptr, checkPos,
                                              al::getGravity(actor) * 150.0f, nullptr, nullptr)) {
        return;
    }

    sead::Vector3f front = al::getTrans(actor) - hitPos;
    al::normalize(&front);
    al::setFront(actor, front);

    sead::Vector3f* gravity = al::getGravityPtr(actor);
    al::verticalizeVec(gravity, front, *gravity);
    al::normalize(al::getGravityPtr(actor));
}

static inline void updateSteerAngle(Motorcycle* actor, f32* steerAngle,
                                    IUsePlayerPuppet* playerPuppet, f32 stickScale, f32 tiltScale,
                                    f32 turnRate) {
    sead::Vector2f stick = {0.0f, 0.0f};
    calcInputStick(&stick, actor, playerPuppet);
    f32 stickH = stick.x * stickScale;
    f32 leftAngle = al::diffNearAngleDegree(0.0f, rs::getPuppetPoseRotZDegreeLeft(playerPuppet));
    f32 rigthAngle = al::diffNearAngleDegree(0.0f, rs::getPuppetPoseRotZDegreeRight(playerPuppet));

    f32 puppetAngle = al::normalizeAbs(
        sead::Mathf::abs(leftAngle) < sead::Mathf::abs(rigthAngle) ? rigthAngle : leftAngle, 45.0f,
        135.0f);

    f32 target = stickH + puppetAngle * tiltScale;
    *steerAngle = al::lerpValue(*steerAngle, target, turnRate);
}

void updateBoundPhysics(Motorcycle* actor, IUsePlayerPuppet* playerPuppet, f32* steerAngle,
                        SeRumbleState* seRumbleState, bool wasInWater) {
    updateSteerAngle(actor, steerAngle, playerPuppet, 12.5f, -7.5f, 0.1f);
    sead::Vector2f stick = {0.0f, 0.0f};
    calcInputStick(&stick, actor, playerPuppet);
    al::rotateVectorDegreeY(al::getFrontPtr(actor), stick.x * -0.5f);
    al::normalize(al::getFrontPtr(actor));

    sead::Vector3f velocity = {al::getVelocity(actor).x, 0.0f, al::getVelocity(actor).z};
    f32 velocityY = al::getVelocity(actor).y;
    if (!rs::isPuppetHoldActionButton(playerPuppet))
        velocity *= 0.95f;

    sead::Vector3f front = {al::getFront(actor).x, 0.0f, al::getFront(actor).z};
    al::tryNormalizeOrZero(&front);
    al::setVelocity(actor, velocity.length() * front + sead::Vector3f{0.0f, velocityY, 0.0f});
    al::addVelocityY(actor, -2.0f);
    updateSeRumble(seRumbleState, actor, playerPuppet, *steerAngle, true);

    if (tryBound(actor))
        return;

    if (rs::isCollidedWallVelocity(actor, actor) && tryHandleWallCollision(actor))
        return;

    updateAirOrientation(actor);
    if (tryLand(actor))
        return;

    if (al::isInWater(actor) && !wasInWater)
        al::startHitReaction(actor, "着水");
}

// TODO: might be moved into `sead`
inline f32 normalize2(sead::Vector3f* v, f32 scalar) {
    const f32 len = v->length();
    if (len > 0) {
        const f32 inv_len = scalar / len;
        v->x *= inv_len;
        v->y *= inv_len;
        v->z *= inv_len;
    }

    return len;
}

void updateRunningVelocity(Motorcycle* actor, f32 value) {
    const sead::Vector3f groundNormal = {0.0f, -1.0f, 0.0f};
    rs::reboundVelocityPart(actor, actor, 0.0f, -0.5f, 0.0f, 0.0f);
    sead::Vector3f velH = {0.0f, 0.0f, 0.0f};
    sead::Vector3f velV = {0.0f, 0.0f, 0.0f};
    al::separateVectorHV(&velH, &velV, actor->getParams()->groundNormal, al::getVelocity(actor));

    sead::Vector3f normal = {0.0f, 0.0f, 0.0f};
    if (rs::isCollidedGround(actor)) {
        if (!isTouchingGround(actor->getParams()))
            normal.set(rs::getCollidedGroundNormal(actor));
        else
            normal.set(actor->getParams()->groundNormalAvg);
    } else {
        normal.set(-groundNormal);
        al::verticalizeVec(&normal, al::getFront(actor), normal);
        if (!al::tryNormalizeOrZero(&normal))
            normal.set(-groundNormal);
    }

    velV = -normal * (velV.length() + 3.0f);
    al::limitLength(&velV, velV, rs::isCollidedGround(actor) ? 5.0f : 35.0f);

    if (!al::isNearZero(velH)) {
        f32 length = velH.length();
        al::parallelizeVec(&velH, al::getFront(actor), velH);
        if (!al::isNearZero(velH))
            normalize2(&velH, length);
        sead::Vector3f front = al::getFront(actor);
        al::verticalizeVec(&front, normal, front);
        al::normalize(&front);
        if (!al::isParallelDirection(al::getFront(actor), velH)) {
            sead::Vector3f faceDir = velH;
            al::normalize(&faceDir);
            al::turnVecToVecRate(&front, faceDir, al::getFront(actor), 0.15f);
            al::normalize(&front);
        }
        velH = (front * value + velH) * 0.95f;
    } else {
        velH = (al::getFront(actor) * value) * 0.95f;
    }

    if (rs::isCollidedGround(actor)) {
        const al::CollisionParts* parts = rs::getCollidedGroundCollisionParts(actor);

        sead::Vector3f transformedVelH = velH;
        transformedVelH.rotate(parts->getPrevBaseInvMtx());
        transformedVelH.rotate(parts->getBaseMtx());

        al::verticalizeVec(&transformedVelH, al::getGravity(actor), transformedVelH);
        normalize2(&transformedVelH, velH.length());
        velH.set(transformedVelH);
    }
    al::setVelocity(actor, velH + velV);
}

Motorcycle::Motorcycle(const char* name) : al::LiveActor(name) {}

// NON_MATCHING: Uses ldr instead of add https://decomp.me/scratch/TZRB7
void Motorcycle::init(const al::ActorInitInfo& info) {
    al::tryGetArg(&mIsOnLight, info, "IsOnLight");
    al::initActorSuffix(this, info, mIsOnLight ? "Night" : nullptr);
    al::initNerve(this, &NrvMotorcycle.Wait, 0);
    al::hideSilhouetteModel(this);
    al::invalidateDepthShadowMap(this);
    al::invalidateHitSensors(this);
    al::validateHitSensorBindableAll(this);
    al::validateHitSensorRideAll(this);
    al::startAction(this, "Wait");
    if (mIsOnLight)
        al::startMclAnim(this, "FrontOnTailOff");
    else
        al::startMclAnim(this, "FrontOffTailOff");
    al::offCollide(this);
    al::calcQuat(&mBaseQuat, this);

    mBaseTrans.set(al::getTrans(this));
    mPlayerAnimator = new MotorcyclePlayerAnimator();
    mAccelerationState = new AccelerationState;
    mParams = new MotorcycleParams;
    mSeRumbleState = new SeRumbleState;
    mParkingParams = new ParkingParams;

    al::initJointControllerKeeper(this, 6);
    al::initJointLocalZRotator(this, &mPose.steerAngle, "AllRoot");
    al::initJointLocalXRotator(this, &mPose.handleAngle, "Handle");
    al::initJointLocalYRotator(this, &mPose.handleAngle, "FrontWheel");
    al::initJointLocalYRotator(this, &mPose.handleAngle, "Cowl");
    al::initJointLocalXRotator(this, &mPose.leanAngle, "AllRoot");
    al::initJointLocalXRotator(this, &mPose.jumpAngle, "JointRoot");

    CollisionShapeKeeper* collision = new CollisionShapeKeeper(7, 0x40, 0x10);
    collision->createShapeArrow("FrontCenter", sFrontCenterArrow, sFrontArrowBase, 20.0f, 0);
    collision->createShapeArrow("FrontLeft", sFrontLeftArrow, sFrontArrowBase, 20.0f, 1);
    collision->createShapeArrow("FrontRight", sFrontRightArrow, sFrontArrowBase, 20.0f, 2);

    collision->createShapeSphereSupportGround("FrontFace", 60.0f, {0.0f, 80.0f, -35.0f},
                                              sead::Vector3f::ey, 20.0f);
    collision->createShapeSphereSupportGround("BackFace", 60.0f, sBackSphere, sead::Vector3f::ey,
                                              20.0f);
    collision->createShapeSphereSupportGround("CenterFace", 60.0f, sCenterFaceSphere,
                                              sead::Vector3f::ey, 20.0f);

    collision->createShapeSphereIgnoreGround("Head", 40.0f, {0.0f, 155.0f, -80.0f});
    collision->updateShape();

    PlayerCollider* collider = new PlayerCollider(getCollisionDirector(), getBaseMtx(),
                                                  al::getTransPtr(this), &sGroundNormal, false);
    collider->setCollisionShapeKeeper(collision);
    collider->onInvalidate();
    mPlayerCollider = collider;
    rs::createAndSetColliderFilterSpecialPurpose(this, "MoveLimit");

    mColliderCameraTarget = new MotorcycleColliderCameraTarget(this);
    mTransCameraSubTarget = al::createTransCameraSubTarget("バイク進行方向", &mCameraSubTargetPos);

    mCameraSubTargetTurnParam = new al::CameraSubTargetTurnParam();
    al::initCameraSubTargetTurnParam(mTransCameraSubTarget, mCameraSubTargetTurnParam);

    mCollisionPartsConnector = al::tryCreateCollisionPartsConnector(this, info, mBaseQuat);
    mBindKeepDemoInfo = rs::initBindKeepDemoInfo();
    al::tryGetArg(&mIsStickWorldPose, info, "IsStickWorldPose");

    mCoursePointSize = al::calcLinkChildNum(info, "CoursePoint");
    if (mCoursePointSize > 0) {
        mCoursePoints = new sead::Vector3f[mCoursePointSize];
        for (s32 i = 0; i < mCoursePointSize; i++)
            al::getChildLinkT(&mCoursePoints[i], info, "CoursePoint", i);
    }

    al::setEffectNamedMtxPtr(this, "WaterSurface", &mWaterSurfaceMtx);
    al::setSeUserSyncParamPtr(this, &mSeRumbleState->volume, "スピード");
    makeActorAlive();
}

void Motorcycle::initAfterPlacement() {
    if (mCollisionPartsConnector)
        al::attachCollisionPartsConnectorToGround(mCollisionPartsConnector, this);
}

bool isAnyRideRun(Motorcycle* actor) {
    return al::isNerve(actor, &NrvMotorcycle.RideRunStart) ||
           al::isNerve(actor, &NrvMotorcycle.RideRun) ||
           al::isNerve(actor, &NrvMotorcycle.RideRunCollide) ||
           al::isNerve(actor, &NrvMotorcycle.RideRunFall) ||
           al::isNerve(actor, &NrvMotorcycle.RideRunWheelie) ||
           al::isNerve(actor, &NrvMotorcycle.RideRunLand) ||
           al::isNerve(actor, &NrvMotorcycle.RideRunJump) ||
           al::isNerve(actor, &NrvMotorcycle.RideRunBoundStart) ||
           al::isNerve(actor, &NrvMotorcycle.RideRunBound);
}

void Motorcycle::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (mPlayerPuppet && al::isSensorPlayerAttack(self)) {
        if (al::isNerve(this, &NrvMotorcycle.Jump) ||
            al::isNerve(this, &NrvMotorcycle.RideWaitJump) ||
            al::isNerve(this, &NrvMotorcycle.RideRunFall) ||
            al::isNerve(this, &NrvMotorcycle.RideRunWheelie) ||
            al::isNerve(this, &NrvMotorcycle.RideRunJump) ||
            al::isNerve(this, &NrvMotorcycle.RideRunBoundStart) ||
            al::isNerve(this, &NrvMotorcycle.RideRunBound)) {
            if (al::sendMsgPlayerAttackTrample(other, self, nullptr))
                return;
        }

        if (isAnyRideRun(this)) {
            if (rs::sendMsgMotorcycleDashAttack(other, self) ||
                rs::sendMsgMotorcycleAttack(other, self) || rs::sendMsgBirdFlyAway(other, self) ||
                mPlayerPuppet)
                return;
        }
    }

    if (al::isSensorBindableAll(self) || al::isSensorPlayerAll(self) ||
        rs::sendMsgIgnorePushMotorcycle(other, self) || al::sendMsgPush(other, self)) {
        return;
    }

    if (mGetOffDelay <= 0) {
        if (rs::sendMsgPushToPlayer(other, self))
            return;
    }

    rs::sendMsgPushToMotorcycle(other, self);
}

bool Motorcycle::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    if (rs::isMsgTargetMarkerPosition(message)) {
        sead::Vector3f position = {0.0f, 0.0f, 0.0f};
        al::calcTransLocalOffset(&position, this, forceField5);
        rs::setMsgTargetMarkerPosition(message, position);
        return true;
    }

    if (rs::isMsgIgnoredByRunawayNpc(message))
        return true;

    if (al::isSensorPlayer(self)) {
        if (rs::isMsgPushToMotorcycle(message)) {
            rs::calcNpcPushVecBetweenSensors(&mExternalPushVelocity, other, self);
            return true;
        }

        if (al::isSensorNpc(other) &&
            (al::isMsgPushAll(message) || rs::isMsgPushToPlayer(message))) {
            sead::Vector3f sideDir = {0.0f, 0.0f, 0.0f};
            al::calcSideDir(&sideDir, this);
            sead::Vector3f sensorDistance = al::getSensorPos(self) - al::getSensorPos(other);
            al::parallelizeVec(&sensorDistance, sideDir, sensorDistance);
            f32 radius = sead::Mathf::max(al::getSensorRadius(other), al::getSensorRadius(self)) -
                         sensorDistance.length();
            if (radius > 0.0f && al::tryNormalizeOrZero(&sensorDistance)) {
                f32 pushRate = sead::Mathf::min(radius, 10.0f);
                mExternalPushVelocity += pushRate * sensorDistance;
                al::limitLength(&mExternalPushVelocity, mExternalPushVelocity, radius);

                return true;
            }
        }
    }

    if (rs::isMsgKillByShineGet(message) || rs::isMsgKillByHomeDemo(message)) {
        if (mPlayerPuppet) {
            rs::endBindAndPuppetNull(&mPlayerPuppet);
            endRide(this, mPlayerAnimator, mColliderCameraTarget, mTransCameraSubTarget);
        }
        reset(this);
        al::setNerve(this, &NrvMotorcycle.ResetNoReaction);
        return true;
    }

    if (mPlayerPuppet) {
        if (rs::isActiveBindKeepDemo(mBindKeepDemoInfo)) {
            if (rs::isMsgBindKeepDemoExecute(message)) {
                mPlayerAnimator->tryStartBindRideRunIfNotPlaying();
                mPlayerAnimator->update(0.0f, 1.0f, 0.0f);
                if (mPlayerPuppet)
                    syncPuppetPose(mPlayerPuppet, this, mPuppetQuat, mPuppetTrans);
                mIsExecutingBindDemo = false;
                return true;
            }

            if (rs::tryEndBindKeepDemo(mBindKeepDemoInfo, message, mPlayerPuppet)) {
                mPlayerAnimator->tryStartBindRideRunIfNotPlaying();
                mIsExecutingBindDemo = false;
                if (al::isNerve(this, &NrvMotorcycle.RideStartLeft) ||
                    al::isNerve(this, &NrvMotorcycle.RideStartRight) ||
                    al::isNerve(this, &NrvMotorcycle.RideStartOn)) {
                    al::setNerve(this, &NrvMotorcycle.RideWait);
                    return true;
                }
                return true;
            }
        } else if (rs::tryStartBindKeepDemo(mBindKeepDemoInfo, message, mPlayerPuppet)) {
            mPlayerAnimator->startBindKeepDemo();
            mIsExecutingBindDemo = true;
            return true;
        }

        if (al::isMsgPlayerDisregard(message))
            return true;

        if (rs::isMsgEnableMapCheckPointWarp(message) || rs::isMsgBindCollidedGround(message))
            return rs::isCollidedGround(this);

        if (!al::isSensorName(self, "Pushed") && al::isSensorPlayer(self) &&
            (al::isMsgExplosion(message) || rs::isMsgEnemyAttackTRex(message) ||
             (!isAnyRideRun(this) && al::isMsgEnemyAttack(message)))) {
            return rs::requestDamage(mPlayerPuppet);
        }

        if (rs::tryReceiveBindCancelMsgAndPuppetNull(&mPlayerPuppet, message)) {
            endRide(this, mPlayerAnimator, mColliderCameraTarget, mTransCameraSubTarget);
            return true;
        }
        if (al::isMsgBindEnd(message)) {
            startGetOff(this, &mPlayerPuppet, &mGetOffDelay, mPlayerAnimator, mColliderCameraTarget,
                        mTransCameraSubTarget);
            return true;
        }
        return rs::isMsgBindRecoveryLife(message) && al::isNerve(this, &NrvMotorcycle.RideWait);
    }

    if (rs::isMsgCheckCarObstacle(message))
        return true;

    if (rs::isMsgFireSwitchFire(message)) {
        reset(this);
        return true;
    }

    if (al::isSensorRide(self)) {
        if (rs::isMsgPlayerObjectWallHit(message))
            return false;

        if (rs::isMsgNpcCapReactionAll(message) || al::isMsgExplosion(message) ||
            (rs::isPlayerHack(this) &&
             rs::checkMsgNpcTrampleReactionAll(message, other, self, false))) {
            rs::requestHitReactionToAttacker(message, self, other);

            if (al::isNerve(this, &NrvMotorcycle.Wait) || al::isNerve(this, &NrvMotorcycle.Creep) ||
                al::isNerve(this, &NrvMotorcycle.Reaction)) {
                if (al::isNerve(this, &NrvMotorcycle.Reaction) && al::isLessStep(this, 7))
                    return false;

                if (mItemSpawnCount > 0) {
                    sead::Vector3f itemPos = sItemSpawnOffset;
                    al::multVecPose(&itemPos, this, itemPos);
                    sead::Quatf itemQuat = sead::Quatf::unit;
                    al::calcQuat(&itemQuat, this);
                    al::appearItem(this, itemPos, itemQuat, nullptr);
                    mItemSpawnCount--;
                }
                al::setNerve(this, &NrvMotorcycle.Reaction);
            }
            return true;
        }

        return false;
    }

    if (!al::isSensorBindableAll(self) || rs::isPlayer2D(this))
        return false;

    if (al::isMsgBindStart(message) &&
        (al::isNerve(this, &NrvMotorcycle.Wait) || al::isNerve(this, &NrvMotorcycle.Creep) ||
         al::isNerve(this, &NrvMotorcycle.Reaction))) {
        if (rs::isPlayerOnGround(this)) {
            if (!rs::isPlayerInputTriggerRide(this))
                return false;
        } else {
            if (al::getActorVelocity(other).y > -6.0f)
                return false;

            sead::Vector3f velocity = al::getActorVelocity(other);
            if (!al::tryNormalizeOrZero(&velocity) || velocity.y > 0.0f) {
                if (mGetOffDelay > 0)
                    mGetOffDelay = 5;
                return false;
            }

            if (mGetOffDelay > 0)
                return false;

            sead::Vector3f dits = al::getActorTrans(other) - al::getTrans(this);
            al::parallelizeVec(&dits, al::getFront(this), dits);
            f32 frontDot = al::getFront(this).dot(dits);
            f32 distance = dits.length();
            if (frontDot > 0.0f) {
                if (distance > 50.0f)
                    return false;
            } else if (distance > 200.0f) {
                return false;
            }
            if (al::calcDistanceV(al::getGravity(this), other, self) > 100.0f)
                return false;
        }
        return true;
    }

    if (al::isMsgBindInit(message)) {
        mPlayerPuppet = rs::startPuppet(self, other);
        rs::validateRecoveryArea(mPlayerPuppet);
        mPlayerAnimator->startBind(mPlayerPuppet);
        rs::setPuppetVelocity(mPlayerPuppet, sead::Vector3f::zero);
        bool isCollidedPuppetGround = rs::isCollidedPuppetGround(mPlayerPuppet);
        al::validateDepthShadowMap(this);
        al::offDepthShadowModel(this);
        rs::hidePuppetShadow(mPlayerPuppet);
        rs::invalidatePuppetCollider(mPlayerPuppet);
        rs::invalidatePuppetReceivePush(mPlayerPuppet);
        rs::invalidateWaterSurfaceShadow(mPlayerPuppet);
        al::setCameraTarget(this, mColliderCameraTarget);
        al::onCameraRideObj(this);
        if (mIsStickWorldPose == false) {
            al::setCameraSubTarget(this, mTransCameraSubTarget);
            mCameraSubTargetAngle = 0.0f;
            mCoursePointFollowTimer = 0;
        }
        al::invalidateHitSensors(this);
        al::validateHitSensorPlayerAll(this);
        al::validateHitSensorEnemyBodyAll(this);
        al::invalidateClipping(this);
        al::onCollide(this);
        al::showSilhouetteModel(this);
        al::invalidateOcclusionQuery(this);
        mAccelerationState->reset();
        mAutoResetTimer = 1200;
        rs::calcPuppetQuat(&mPuppetQuat, mPlayerPuppet);
        mPuppetTrans.set(rs::getPuppetTrans(mPlayerPuppet));
        BindInfo bindInfo;
        bindInfo.name = "Motorcycle";
        rs::tryAppearBindTutorial(this, bindInfo);
        rs::rideMotorcycle(this);
        mPuppetRotZLeft = rs::getPuppetPoseRotZDegreeLeft(mPlayerPuppet);
        mPuppetRotZRight = rs::getPuppetPoseRotZDegreeRight(mPlayerPuppet);
        if (!isCollidedPuppetGround) {
            al::setNerve(this, &NrvMotorcycle.RideStartOn);
            return true;
        }

        sead::Vector3f dirH = {0.0f, 0.0f, 0.0f};
        if (al::calcDirBetweenSensorsH(&dirH, self, other)) {
            sead::Vector3f sideDir = {0.0f, 0.0f, 0.0f};
            al::calcSideDir(&sideDir, this);
            if (sideDir.dot(dirH) > 0.0f) {
                al::setNerve(this, &NrvMotorcycle.RideStartLeft);
                return true;
            }
        }
        al::setNerve(this, &NrvMotorcycle.RideStartRight);
        return true;
    }

    return false;
}

static inline bool isCollidedGround(Motorcycle* actor) {
    return rs::isCollidedGround(actor);
}

// NON_MATCHING: Major optimization differences https://decomp.me/scratch/B2j7L
void Motorcycle::movement() {
    if (rs::isActiveBindKeepDemo(mBindKeepDemoInfo)) {
        mPlayerAnimator->update(0.0f, 1.0f, 0.0f);
        return;
    }

    mAccelerationState->isAccelerating = false;
    al::LiveActor::movement();

    if (mPlayerPuppet) {
        rs::syncPuppetVisibility(this, mPlayerPuppet);
        rs::copyPuppetDitherAlpha(mPlayerPuppet, this);
        al::AreaObj* areaObj =
            al::tryFindAreaObj(this, "MotorcycleInCheckArea", al::getTrans(this));
        if (areaObj) {
            al::tryOnStageSwitch(areaObj, "SwitchAreaOn");
            areaObj->invalidate();
        }
        if (mParams->framesInAir >= 60)
            rs::requestUpToTargetCameraAngleBySpeed(this, 50.0f, 3.0f, 0);
        rs::requestUpdateSafetyPoint(mPlayerPuppet, this, this);
        if (rs::isCollidedGround(this))
            rs::sendPlayerCollisionTouchMsg(this, al::getHitSensor(this, "PlayerBody"), this);
        if (rs::isCollidedGround(this))
            al::holdSe(this, "GroundNoiseLv");
    }

    if (al::isActiveCameraSubTarget(mTransCameraSubTarget)) {
        f32 distRate = 0.0f;
        f32 angleRate = 0.0f;

        if (mCoursePointSize < 1 || !al::isOnStageSwitch(this, "SwitchValidCoursePoint")) {
            mCameraSubTargetAngle = al::lerpValue(mCameraSubTargetAngle, 0.0f, 0.045f);
            mCoursePointFollowTimer = sead::Mathi::max(0, mCoursePointFollowTimer - 1);
        } else {
            f32 bestAngle = 180.0f;
            s32 bestIdx = -1;
            for (s32 i = 0; i < mCoursePointSize; i++) {
                f32 distance = (al::getTrans(this) - mCoursePoints[i]).length();
                if (distance < 750.0f || distance > 3000.0f)
                    continue;
                sead::Vector3f lookDir = {0.0f, 0.0f, 0.0f};
                al::calcCameraLookDir(&lookDir, this, 0);
                f32 angle = al::calcAngleDegree(lookDir, mCoursePoints[i] - al::getTrans(this));
                if (angle > 120.0f || !(bestIdx < 0 || angle < bestAngle))
                    continue;

                distRate = al::normalize(distance, 750.0f, 3000.0f);
                angleRate = al::normalize(angle, 0.0f, 120.0f);
                bestIdx = i;
                bestAngle = angle;
            }

            if (bestIdx < 0) {
                mCameraSubTargetAngle = al::lerpValue(mCameraSubTargetAngle, 0.0f, 0.045f);
                mCoursePointFollowTimer = sead::Mathi::clampMin(mCoursePointFollowTimer - 1, 0);
            } else {
                f32 angleOnPlane = al::calcAngleOnPlaneDegree(
                    al::getFront(this), mCoursePoints[bestIdx] - al::getTrans(this),
                    al::getGravity(this));
                f32 targetAngle = sead::Mathf::clamp(angleOnPlane, -50.0f, 50.0f);
                targetAngle = al::lerpValue(mCameraSubTargetAngle, targetAngle, 0.3f);
                mCameraSubTargetAngle = al::lerpValue(mCameraSubTargetAngle, targetAngle, 0.15f);
                mCoursePointFollowTimer = sead::Mathi::clampMax(mCoursePointFollowTimer + 1, 30);
            }
        }

        if (al::isNerve(this, &NrvMotorcycle.RideWait)) {
            mCameraSubTargetTurnParam->setTurnRate1(mCameraSubTargetTurnParam->getTurnRate1() *
                                                    0.98f);
            mCameraSubTargetTurnParam->setTurnRate2(mCameraSubTargetTurnParam->getTurnRate2() *
                                                    0.98f);
        } else {
            f32 normSpeed =
                al::normalize(al::calcSpeedH(this), 0.0f, al::calcSpeedMax(1.8f, 0.95f));
            f32 combinedRate = distRate * al::normalize(mCoursePointFollowTimer, 0, 30) * angleRate;
            f32 turnRate1 =
                al::lerpValue(al::lerpValue(0.3f, 0.1f, normSpeed), 0.275f, combinedRate);
            mCameraSubTargetTurnParam->setTurnRate1(turnRate1);
            f32 turnRate2 =
                al::lerpValue(al::lerpValue(0.15f, 0.075f, normSpeed), 0.1f, combinedRate);
            mCameraSubTargetTurnParam->setTurnRate2(turnRate2);
        }

        sead::Vector3f subTargetFront = al::getFront(this);
        al::rotateVectorDegree(&subTargetFront, subTargetFront, al::getGravity(this),
                               mCameraSubTargetAngle);
        mCameraSubTargetPos = al::getTrans(this) + subTargetFront * 250.0f;
    }

    MotorcycleParams* params = mParams;
    params->frontContactPoints.clear();
    params->backContactPoints.clear();
    params->groundNormalAvg = {0.0f, 0.0f, 0.0f};

    if (!al::isNoCollide(this)) {
        params->isTouchingGround = false;
        params->isTouchingWall = false;
        params->isTouchingFront = false;
        params->isTouchingBack = false;

        if (isCollidedGround(this)) {
            CollisionShapeKeeper* shapeKeeper = getPlayerCollider()->getCollisionShapeKeeper();
            const s32 numResults = shapeKeeper->getNumCollideResult();
            for (s32 i = 0; i < numResults; i++) {
                const CollidedShapeResult* result = shapeKeeper->getCollidedShapeResult(i);
                if (result->isArrow()) {
                    const al::ArrowHitInfo& arrowHitInfo = result->getArrowHitInfo();
                    const sead::Vector3f& normal = arrowHitInfo.hitInfo->triangle.getNormal(0);
                    if (al::isFloorPolygon(normal, sGroundNormal)) {
                        if (result->getShapeInfoArrow()->get_18() == 0) {
                            params->isTouchingGround = true;
                            params->groundNormalAvg +=
                                result->getArrowHitInfo().hitInfo->triangle.getNormal(0);
                        } else {
                            params->isTouchingWall = true;
                        }
                    }
                } else if (result->isSphere()) {
                    const al::SphereHitInfo& sphereHitInfo = result->getSphereHitInfo();
                    sead::Vector3f normal = sphereHitInfo.hitInfo->triangle.getNormal(0);
                    if (al::isEqualString("FrontFace", result->getShapeInfoSphere()->getName())) {
                        params->frontContactPoints.emplaceBack(normal);
                        if (al::isFloorPolygon(normal, sGroundNormal))
                            params->isTouchingFront = true;
                    } else if (al::isEqualString("BackFace",
                                                 result->getShapeInfoSphere()->getName())) {
                        params->backContactPoints.emplaceBack(normal);
                        if (al::isFloorPolygon(normal, sGroundNormal))
                            params->isTouchingBack = true;
                    }
                }
            }
        }
        al::tryNormalizeOrZero(&params->groundNormalAvg);
    }

    if (rs::isCollidedGround(this)) {
        if (isTouchingGround(mParams) && !mParams->isTouchingWall) {
            sead::Vector3f pos = {0.0f, 0.0f, 0.0f};
            const sead::Vector3f& front = al::getFront(this);
            sead::Vector3f frontOffset = front * -300.0f * 0.5f;
            sead::Vector3f arrowPos;
            sead::Vector3f arrowDir;
            al::Triangle triangle;
            const al::IUseCollision* collision = this;
            arrowPos = frontOffset + al::getTrans(this) - al::getGravity(this) * 100.0f;
            const sead::Vector3f& gravity = al::getGravity(this);
            arrowDir = gravity * 100.0f + gravity * 100.0f;
            bool foundFloor =
                alCollisionUtil::getFirstPolyOnArrow(collision, &pos, &triangle, arrowPos, arrowDir,
                                                     nullptr, nullptr) &&
                al::isFloorPolygon(triangle.getNormal(0), sGroundNormal);

            if (foundFloor) {
                if (!al::isNear(pos, al::getTrans(this), 0.001f)) {
                    arrowPos = pos - al::getTrans(this);
                    al::normalize(&arrowPos);
                    sead::Vector3f* frontPtr = al::getFrontPtr(this);
                    arrowDir = -arrowPos;
                    al::turnVecToVecRate(frontPtr, *frontPtr, arrowDir, 0.3f);
                    al::normalize(al::getFrontPtr(this));
                    sead::Vector3f* gravityPtr = al::getGravityPtr(this);
                    al::verticalizeVec(gravityPtr, al::getFront(this), *gravityPtr);
                    al::normalize(al::getGravityPtr(this));
                }
            }
        }
    }

    f32 weightC = 0.0f;
    mPose.handleAngle = al::normalizeAbs(mPose.steerAngle, 0.0f, 55.0f) * -32.5f;
    f32 weightL = 1.0f;
    f32 weightR = 0.0f;
    if (mPlayerPuppet) {
        weightR = 0.0f;
        if (mPose.handleAngle > 0.0f)
            weightL = al::normalize(mPose.handleAngle, 0.0f, 32.5f);
        else
            weightL = 0.0f;
        if (mPose.handleAngle < 0.0f)
            weightR = 1.0f - al::normalize(mPose.handleAngle, -32.5f, 0.0f);
        weightC = 1.0f - (weightL + weightR);
    }
    mPlayerAnimator->update(weightL, weightC, weightR);

    al::setMaterialCode(this,
                        rs::isCollidedGround(this) ? rs::getMaterialCodeGround(this) : "NoCollide");
    al::makeMtxSRT(&mWaterSurfaceMtx, this);

    if (mPlayerPuppet && al::isInWater(this)) {
        sead::Vector3f flatSurface = {0.0f, 0.0f, 0.0f};
        sead::Vector3f waterSurface = {0.0f, 0.0f, 0.0f};
        sead::Vector3f waterSurfaceNormal = {0.0f, 0.0f, 0.0f};
        sead::Vector3f actorUp = {0.0f, 0.0f, 0.0f};
        al::calcUpDir(&actorUp, this);
        bool foundFlat = al::calcFindWaterSurfaceFlat(
            &flatSurface, nullptr, this, al::getTrans(this), sead::Vector3f::ey, 121.0f);
        bool foundWater = al::calcFindWaterSurface(&waterSurface, &waterSurfaceNormal, this,
                                                   al::getTrans(this), sead::Vector3f::ey, 121.0f);

        sead::Vector3f surface = {0.0f, 0.0f, 0.0};
        bool foundSurface = false;
        if (foundFlat || foundWater) {
            if (foundFlat && foundWater)
                if ((flatSurface - al::getTrans(this)).length() >
                    (waterSurface - al::getTrans(this)).length())
                    surface.set(flatSurface);
                else
                    surface.set(waterSurface);
            else if (foundFlat)
                surface.set(flatSurface);
            else
                surface.set(waterSurface);
            foundSurface = true;
        }

        if (foundSurface) {
            al::updateMaterialCodePuddle(this, true);
            al::updateMaterialCodeWater(this, false);
            mWaterSurfaceMtx.setTranslation(surface);
        } else {
            startGetOff(this, &mPlayerPuppet, &mGetOffDelay, mPlayerAnimator, mColliderCameraTarget,
                        mTransCameraSubTarget);
            reset(this);
        }
    } else {
        al::updateMaterialCodePuddle(this, false);
        al::updateMaterialCodeWater(this, false);
    }

    mGetOffDelay--;

    bool wasAccelerating = mIsAccelerating;
    bool isAccelerating = mPlayerPuppet && !mAccelerationState->isAccelerating;

    mIsAccelerating = isAccelerating;
    if (wasAccelerating != isAccelerating) {
        const char* anim = mIsOnLight ? (isAccelerating ? "FrontOnTailOn" : "FrontOnTailOff") :
                                        (isAccelerating ? "FrontOffTailOn" : "FrontOffTailOff");
        al::startMclAnim(this, anim);
    }
}

void Motorcycle::kill() {
    al::LiveActor::kill();
    al::onDepthShadowModel(this);
    al::invalidateDepthShadowMap(this);
}

void Motorcycle::updateCollider() {
    if (rs::isActiveBindKeepDemo(mBindKeepDemoInfo))
        return;
    MotorcycleParams* params = mParams;
    if (al::isNoCollide(this)) {
        params->isOnGround = false;
        params->framesInAir = -1;
        params->isOnJump = false;
    } else {
        params->isOnGround = rs::isCollidedGround(this);
        params->isOnJump = rs::isCollisionCodeJump(this);
        if (params->isOnGround) {
            params->framesInAir = 0;
            params->lastGroundPos.set(al::getTrans(this));
            params->groundNormal.set(rs::getCollidedGroundNormal(this));
        } else
            params->framesInAir++;
    }

    mIsInWater = al::isInWater(this);
    mPrevTransDelta = -al::getTrans(this);

    rs::updateCollider(this, this,
                       al::getVelocity(this) + mExternalPushVelocity + mSteerShiftVelocity);
    mExternalPushVelocity = {0.0f, 0.0f, 0.0f};
    mSteerShiftVelocity = {0.0f, 0.0f, 0.0f};
    mPrevTransDelta += al::getTrans(this);
}

void Motorcycle::calcAnim() {
    al::LiveActor::calcAnim();
    if (mPlayerPuppet && !mIsExecutingBindDemo)
        syncPuppetPose(mPlayerPuppet, this, mPuppetQuat, mPuppetTrans);
}

void Motorcycle::exeWait() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "Wait");
        al::setVelocityZero(this);
    }
    mPose.steerAngle = al::lerpValue(mPose.steerAngle, 0.0f, 0.2f);
    mPose.handleAngle = al::lerpValue(mPose.handleAngle, 0.0f, 0.2f);
    mPose.leanAngle = al::lerpValue(mPose.leanAngle, 0.0f, 0.2f);
    mPose.jumpAngle = al::lerpValue(mPose.jumpAngle, 0.0f, 0.2f);

    if (al::isNoCollide(this)) {
        if (mCollisionPartsConnector) {
            if (!al::isMtxConnectorConnecting(mCollisionPartsConnector)) {
                kill();
                return;
            }
            sead::Quatf quat = sead::Quatf::unit;
            sead::Vector3f trans = {0.0f, 0.0f, 0.0f};
            al::calcConnectQT(&quat, &trans, mCollisionPartsConnector);
            al::resetQuatPosition(this, quat, trans);
        }
    } else {
        updateOrientation(this, mPose.steerAngle);
        al::setVelocityToGravity(this, 2.0f);
        if (!rs::isCollidedGround(this) && al::isGreaterEqualStep(this, 2)) {
            al::setNerve(this, &NrvMotorcycle.Fall);
            return;
        }
    }
    if (mAutoResetTimer > 0 && mAutoResetTimer-- == 1)
        reset(this);
}

void Motorcycle::exeCreep() {
    if (al::isFirstStep(this))
        al::tryStartActionIfNotPlaying(this, "Wait");

    mPose.steerAngle = al::lerpValue(mPose.steerAngle, 0.0f, 0.2f);
    mPose.handleAngle = al::lerpValue(mPose.handleAngle, 0.0f, 0.2f);
    mPose.leanAngle = al::lerpValue(mPose.leanAngle, 0.0f, 0.2f);
    mPose.jumpAngle = al::lerpValue(mPose.jumpAngle, 0.0f, 0.2f);
    updateOrientation(this, mPose.steerAngle);

    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.9f);
    al::limitVelocityDir(this, al::getGravity(this), 360.0f);

    if (!isCollidedGround(this) && al::isGreaterEqualStep(this, 2)) {
        al::setNerve(this, &NrvMotorcycle.Fall);
        return;
    }

    if (al::isInDeathArea(this) || al::isGreaterEqualStep(this, 360)) {
        reset(this);
        return;
    }

    if (al::isVelocitySlowH(this, 3.0f))
        al::setNerve(this, &NrvMotorcycle.Wait);
}

void Motorcycle::exeFall() {
    bool isInWater = al::isInWater(this);
    al::addVelocityY(this, isInWater ? -2.0f : -1.0f);
    if (isInWater)
        al::scaleVelocityParallelVertical(this, sGroundNormal, 0.8f, 0.925f);
    else
        al::scaleVelocityExceptDirection(this, sGroundNormal, 0.95f);

    if (al::getVelocity(this).dot({0.0f, -1.0f, 0.0f}) > 0.0f)
        al::limitVelocityDir(this, sGroundNormal, 35.0f);

    if (al::isInDeathArea(this)) {
        reset(this);
        return;
    }

    if (tryHandleGroundCollision(this))
        return;

    if (al::isGreaterEqualStep(this, 360)) {
        reset(this);
        return;
    }

    if (al::isInWater(this) && !mIsInWater)
        al::startHitReaction(this, "着水");
}

void Motorcycle::exeJump() {
    if (al::isFirstStep(this))
        al::setVelocity(this, 0.0f, 50.0f, 0.0f);

    bool isInWater = al::isInWater(this);
    al::addVelocityY(this, isInWater ? -2.0f : -1.0f);
    if (!isInWater)
        al::scaleVelocityExceptDirection(this, sGroundNormal, 0.95f);
    else
        al::scaleVelocityParallelVertical(this, sGroundNormal, 0.8f, 0.925f);

    if (al::getVelocity(this).dot({0.0f, -1.0f, 0.0f}) > 0.0f)
        al::limitVelocityDir(this, sGroundNormal, 35.0f);

    if (al::getVelocity(this).dot({0.0f, -1.0f, 0.0f}) > 0.0f || al::isInDeathArea(this)) {
        reset(this);
        return;
    }

    if (!tryHandleGroundCollision(this) && al::isInWater(this) && !mIsInWater)
        al::startHitReaction(this, "着水");
}

void Motorcycle::endJump() {
    mPlayerAnimator->tryStartBindRideLandIfJump();
    mPose.jumpAngle = 0.0f;
}

void Motorcycle::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");

    mPose.steerAngle = al::lerpValue(mPose.steerAngle, 0.0f, 0.2f);
    mPose.handleAngle = al::lerpValue(mPose.handleAngle, 0.0f, 0.2f);
    mPose.leanAngle = al::lerpValue(mPose.leanAngle, 0.0f, 0.2f);
    mPose.jumpAngle = al::lerpValue(mPose.jumpAngle, 0.0f, 0.2f);
    al::setNerveAtActionEnd(this, &NrvMotorcycle.Wait);
}

void Motorcycle::exeReset() {
    if (al::isFirstStep(this)) {
        al::tryKillEmitterAndParticleAll(this);
        al::stopAction(this);
        if (al::isNerve(this, &NrvMotorcycle.Reset))
            al::startHitReaction(this, "リセット");
        al::offCollide(this);
        mPose.steerAngle = 0.0f;
        mPose.handleAngle = 0.0f;
        mPose.leanAngle = 0.0f;
        mPose.jumpAngle = 0.0f;
        mAutoResetTimer = -1;
    }

    if (al::isStep(this, 1)) {
        al::resetQuatPosition(this, mBaseQuat, mBaseTrans);
        rs::resetCollision(this);
    }

    if (al::isGreaterEqualStep(this, 60)) {
        al::validateClipping(this);
        al::restartAction(this);
        al::showModelIfHide(this);
        al::invalidateHitSensors(this);
        al::validateHitSensorBindableAll(this);
        al::validateHitSensorRideAll(this);
        al::startHitReaction(this, "出現");
        al::setNerve(this, &NrvMotorcycle.Wait);
    }
}

void Motorcycle::exeRideStart() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "Wait");
        if (al::isNerve(this, &NrvMotorcycle.RideStartLeft))
            mPlayerAnimator->startBindRideStartL();
        else
            mPlayerAnimator->startBindRideStartR();
    }

    if (rs::isPuppetActionEnd(mPlayerPuppet))
        al::setNerve(this, &NrvMotorcycle.RideStartOn);
}

void Motorcycle::exeRideStartOn() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "RideOn");
        mPlayerAnimator->startBindRideOn();
        rs::requestResetCameraAngleV(this, 0);
    }

    if (rs::isPuppetActionEnd(mPlayerPuppet)) {
        rs::validatePuppetLookAt(mPlayerPuppet);
        al::setNerve(this, &NrvMotorcycle.RideWait);
    }
}

static inline bool tryTriggerGetOff(Motorcycle* actor, IUsePlayerPuppet** puppy,
                                    MotorcyclePlayerAnimator* animator,
                                    al::CameraTargetBase* camera,
                                    al::TransCameraSubTarget* subCamera) {
    if (!rs::isActiveBindKeepDemo(actor->getBindKeepDemoInfo()) && rs::isTriggerGetOff(*puppy)) {
        if (!al::isInAreaObj(actor, "MotorcycleInvalidGetOffArea")) {
            startGetOff(actor, puppy, actor->getOffDelay(), animator, camera, subCamera);
            return true;
        }
        al::startHitReaction(actor, "降車キャンセル");
    }
    return false;
}

void Motorcycle::exeRideWait() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "Wait");
        mPlayerAnimator->startBindWait();
    }
    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;

    mPose.steerAngle = al::lerpValue(mPose.steerAngle, 0.0f, 0.025f);
    alPadRumbleFunction::startPadRumbleDirectValue(this, 160.0f, 168.0f, sead::Mathf::deg2rad(2.0f),
                                                   sead::Mathf::deg2rad(2.0f), 0.7f, 0.7f, -1);
    updateOrientation(this, mPose.steerAngle);
    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.95f);

    if (rs::isCollidedGround(this))
        al::limitVelocityDirSign(this, al::getGravity(this), 3.0f);

    if (rs::isPuppetHoldActionButton(mPlayerPuppet)) {
        al::setNerve(this, &NrvMotorcycle.RideRunStart);
        return;
    }

    if (!TryJump(this, mPlayerPuppet))
        tryParking(this, mPlayerPuppet, mParkingParams, &mPose);
}

void Motorcycle::exeRideWaitJump() {
    if (al::isFirstStep(this)) {
        mParams->groundNormal = {0.0f, 1.0f, 0.0f};

        // TODO: Check mParams->isOnJump
        bool isOnJump = mParams->isOnJump == 0;
        f32 velocity = isOnJump ? 18.5f : 50.0f;
        mIsOnJump = isOnJump;

        al::setVelocity(this, 0.0f, velocity, 0.0f);
        al::startHitReaction(this, "ジャンプ開始");
        mPlayerAnimator->startBindRideJump();
    }

    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;

    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, true);
    handleCeilingCollision(this);
    updateSteerAngleAndAccel(this, mPlayerPuppet, mAccelerationState, &mPose.steerAngle);
    sead::Vector2f stick = {0.0f, 0.0f};
    calcInputStick(&stick, this, mPlayerPuppet);
    f32 angle = stick.x * 2.5f;
    if (angle * angle > 0.1f) {
        sead::Vector3f front = al::getFront(this);
        sead::Vector3f front2 = al::getFront(this);
        al::rotateVectorDegree(&front2, front2, al::getGravity(this), angle);
        al::normalize(&front2);

        sead::Vector3f diff = front2 - front;
        al::normalize(&diff);
        al::setFront(this, front2);

        f32 fVar10 = getJointDistance(this, "AllRoot", "JointRoot") *
                     sead::Mathf::sin(sead::Mathf::deg2rad(sead::Mathf::abs(angle * 0.5f))) * 2;
        f32 x = diff.x * fVar10;
        f32 z = diff.z * fVar10;
        mSteerShiftVelocity.x = x;
        mSteerShiftVelocity.z = z;
    }

    al::addVelocityToDirection(
        this, al::getFront(this),
        al::lerpValue(0.0f, 1.8f,
                      al::easeIn(al::normalize(mAccelerationState->accelRate, 0.0f, 5.0f))));
    if (!mIsOnJump || !rs::isPuppetHoldJumpButton(mPlayerPuppet) || al::isGreaterStep(this, 6)) {
        mIsOnJump = false;
        al::addVelocityY(this, -2.0f);
    }

    al::scaleVelocityExceptDirection(this, sGroundNormal, 0.95f);
    if (al::getVelocity(this).dot({0.0f, -1.0f, 0.0f}) > 0.0f)
        al::limitVelocityDir(this, sGroundNormal, 35.0f);

    if (rs::isOnGround(this, this)) {
        al::startHitReaction(this, "着地");
        if (!TryJump(this, mPlayerPuppet)) {
            mPose.leanAngle = 0.0f;
            al::setNerve(this, &NrvMotorcycle.RideWaitLand);
        } else {
            al::startAction(this, "WaitLand");
        }
    }
}

void Motorcycle::endRideWaitJump() {
    mPlayerAnimator->tryStartBindRideLandIfJump();
    mPose.jumpAngle = 0.0f;
}

void Motorcycle::exeRideWaitLand() {
    if (al::isFirstStep(this))
        al::startAction(this, "WaitLand");

    if (TryJump(this, mPlayerPuppet))
        return;

    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;

    mPose.steerAngle = al::lerpValue(mPose.steerAngle, 0.0f, 0.025f);
    alPadRumbleFunction::startPadRumbleDirectValue(this, 160.0f, 168.0f, sead::Mathf::deg2rad(2.0f),
                                                   sead::Mathf::deg2rad(2.0f), 0.7f, 0.7f, -1);
    updateOrientation(this, mPose.steerAngle);
    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.95f);

    if (rs::isCollidedGround(this))
        al::limitVelocityDirSign(this, al::getGravity(this), 3.0f);

    if (rs::isPuppetHoldActionButton(mPlayerPuppet)) {
        al::setNerve(this, &NrvMotorcycle.RideRunStart);
        return;
    }

    al::setNerveAtActionEnd(this, &NrvMotorcycle.RideWait);
}

void Motorcycle::exeRideRunStart() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "RunStart");
        mPlayerAnimator->startBindRideRunStart();
        mSeRumbleState->reset();
    }
    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;
    if (tryHandleHighSpeedWallCollision(this, mPrevTransDelta))
        return;

    updateSteerAngle(this, &mPose.steerAngle, mPlayerPuppet, 40.0f, -15.0f, 0.3f);
    updateOrientation(this, mPose.steerAngle);
    updateSteerAngleAndAccel(this, mPlayerPuppet, mAccelerationState, &mPose.steerAngle);

    updateRunningVelocity(
        this, al::lerpValue(0.0f, 1.8f,
                            al::easeIn(al::normalize(mAccelerationState->accelRate, 0.0f, 5.0f))));
    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, false);

    if (TryJumpWithCameraAbsorb(this, mPlayerPuppet))
        return;

    if (TryWheelieOrFall(this))
        return;

    MotorcycleParams* params = mParams;
    if (rs::isCollidedGround(this) && !params->isOnGround) {
        if (150.0f < getGroundDistance(this, params)) {
            updateVelocity(this, sGroundNormal, -rs::getCollidedGroundNormal(this));
            al::limitVelocityDirSign(this, -rs::getCollidedGroundNormal(this), 5.0f);
            al::setNerve(this, &NrvMotorcycle.RideRunLand);
            return;
        }
    }
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvMotorcycle.RideRun);
}

void Motorcycle::exeRideRun() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "Run");
        mPlayerAnimator->tryStartBindRideRunIfNotPlaying();
        mHighSpeedCameraTimer = 0;
    }
    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;
    if (tryHandleHighSpeedWallCollision(this, mPrevTransDelta))
        return;
    updateSteerAngle(this, &mPose.steerAngle, mPlayerPuppet, 40.0f, -15.0f, 0.3f);
    updateOrientation(this, mPose.steerAngle);
    updateSteerAngleAndAccel(this, mPlayerPuppet, mAccelerationState, &mPose.steerAngle);

    updateRunningVelocity(
        this, al::lerpValue(0.0f, 1.8f,
                            al::easeIn(al::normalize(mAccelerationState->accelRate, 0.0f, 5.0f))));
    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, false);
    if (!mAccelerationState->isAccelerating)
        mHighSpeedCameraTimer = 0;
    else if (mHighSpeedCameraTimer++ > 13)
        rs::requestDownToDefaultCameraAngleBySpeed(this, 6.0f, 0);

    if (TryJumpWithCameraAbsorb(this, mPlayerPuppet) || TryWheelieOrFall(this))
        return;

    MotorcycleParams* params = mParams;
    if (rs::isCollidedGround(this) && !params->isOnGround) {
        if (getGroundDistance(this, params) > 150.0f) {
            updateVelocity(this, sGroundNormal, -rs::getCollidedGroundNormal(this));
            al::limitVelocityDirSign(this, -rs::getCollidedGroundNormal(this), 5.0f);
            al::setNerve(this, &NrvMotorcycle.RideRunLand);
            return;
        }
    }

    if (!tryParking(this, mPlayerPuppet, mParkingParams, &mPose) &&
        al::isVelocitySlowH(this, 3.0f) && mAccelerationState->accelRate < 0.01f &&
        rs::isOnGround(this, this)) {
        al::setNerve(this, &NrvMotorcycle.RideWait);
    }
}

void Motorcycle::exeRideRunCollide() {
    if (al::isFirstStep(this))
        al::startAction(this, "RunCollide");
    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;
    updateSteerAngle(this, &mPose.steerAngle, mPlayerPuppet, 40.0f, -15.0f, 0.3f);
    updateOrientation(this, mPose.steerAngle);
    updateSteerAngleAndAccel(this, mPlayerPuppet, mAccelerationState, &mPose.steerAngle);

    updateRunningVelocity(
        this, al::lerpValue(0.0f, 1.8f,
                            al::easeIn(al::normalize(mAccelerationState->accelRate, 0.0f, 5.0f))));
    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, false);

    if (TryJumpWithCameraAbsorb(this, mPlayerPuppet))
        return;

    if (TryWheelieOrFall(this))
        return;

    MotorcycleParams* params = mParams;
    if (rs::isCollidedGround(this) && !params->isOnGround) {
        if (150.0f < getGroundDistance(this, params)) {
            updateVelocity(this, sGroundNormal, -rs::getCollidedGroundNormal(this));
            al::limitVelocityDirSign(this, -rs::getCollidedGroundNormal(this), 5.0f);
            al::setNerve(this, &NrvMotorcycle.RideRunLand);
            return;
        }
    }

    if (rs::isCollidedWallVelocity(this, this))
        return;
    if (mParams->frontContactPoints.size() > 0 && mParams->backContactPoints.size() > 0)
        return;
    al::setNerve(this, &NrvMotorcycle.RideRun);
}

void Motorcycle::exeRideRunFall() {
    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;

    mParams->groundNormal = {0.0f, 1.0f, 0.0f};
    updateSteerAngle(this, &mPose.steerAngle, mPlayerPuppet, 12.5f, -7.5f, 0.1f);

    applyAirPhysics(this, mPlayerPuppet, al::isInWater(this) ? 1.0f : 2.0f);
    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, true);
    if (!tryBound(this) &&
        (!rs::isCollidedWallVelocity(this, this) || !tryHandleWallCollision(this))) {
        updateAirOrientation(this);
        if (!tryLand(this) && al::isInWater(this) && !mIsInWater) {
            al::startHitReaction(this, "着水");
            return;
        }
    }
    return;
}

void Motorcycle::exeRideRunWheelie() {
    if (al::isFirstStep(this))
        mPlayerAnimator->startBindRideJump();

    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;
    mParams->groundNormal = {0.0f, 1.0f, 0.0f};

    updateSteerAngle(this, &mPose.steerAngle, mPlayerPuppet, 12.5f, -7.5f, 0.1f);

    applyAirPhysics(this, mPlayerPuppet,
                    al::calcNerveEaseInValue(this, 30, 0.0f, al::isInWater(this) ? 1.0f : 2.0f));
    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, true);
    if (!tryBound(this) &&
        (!rs::isCollidedWallVelocity(this, this) || !tryHandleWallCollision(this))) {
        updateAirOrientation(this);
        if (!tryLand(this) && al::isInWater(this) && !mIsInWater) {
            al::startHitReaction(this, "着水");
            return;
        }
    }
}

void Motorcycle::endRideRunWheelie() {
    mPlayerAnimator->tryStartBindRideLandIfJump();
    mPose.jumpAngle = 0.0f;
}

void Motorcycle::exeRideRunLand() {
    if (al::isFirstStep(this))
        al::startAction(this, "RunLand");

    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget) ||
        tryHandleHighSpeedWallCollision(this, mPrevTransDelta))
        return;

    updateSteerAngle(this, &mPose.steerAngle, mPlayerPuppet, 40.0f, -15.0f, 0.3f);
    updateOrientation(this, mPose.steerAngle);
    updateSteerAngleAndAccel(this, mPlayerPuppet, mAccelerationState, &mPose.steerAngle);

    updateRunningVelocity(
        this, al::lerpValue(0.0f, 1.8f,
                            al::easeIn(al::normalize(mAccelerationState->accelRate, 0.0f, 5.0f))));
    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, al::isLessStep(this, 3));
    if (TryJumpWithCameraAbsorb(this, mPlayerPuppet))
        return;

    if (!rs::isCollidedGround(this) && mParams->framesInAir >= 6) {
        updateVelocity(this, -mParams->groundNormal, sGroundNormal);
        al::limitVelocityDirSign(this, sGroundNormal, 3.0f);
        al::setNerve(this, &NrvMotorcycle.RideRunFall);
        return;
    }

    al::setNerveAtActionEnd(this, &NrvMotorcycle.RideRun);
}

void Motorcycle::exeRideRunJump() {
    if (al::isFirstStep(this)) {
        al::tryStartActionIfNotPlaying(this, "Run");
        al::startHitReaction(this, "ジャンプ開始");

        f32 velocity = 55.0f;
        if (!mParams->isOnJump) {
            velocity = 21.5f;
            mIsOnJump = true;
        } else {
            al::requestStopCameraVerticalAbsorb(this);
            mIsOnJump = false;
        }

        al::setVelocityY(this, velocity);
        mPlayerAnimator->startBindRideJump();
    }
    mParams->groundNormal = {0.0f, 1.0f, 0.0f};
    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;

    updateSteerAngle(this, &mPose.steerAngle, mPlayerPuppet, 40.0f, -15.0f, 0.3f);
    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, true);
    updateOrientation(this, mPose.steerAngle);
    handleCeilingCollision(this);
    updateSteerAngleAndAccel(this, mPlayerPuppet, mAccelerationState, &mPose.steerAngle);
    mPose.jumpAngle = al::lerpValue(0.0f, -20.0f, al::calcNerveRate(this, 4));

    if (!mIsOnJump || !rs::isPuppetHoldJumpButton(mPlayerPuppet) ||
        al::isGreaterEqualStep(this, 6)) {
        mIsOnJump = 0;
        sead::Vector3f front = al::getFront(this);
        al::verticalizeVec(&front, sGroundNormal, front);
        al::tryNormalizeOrZero(&front);
        al::addVelocity(
            this, front * al::lerpValue(0.0f, 1.8f,
                                        al::easeIn(al::normalize(mAccelerationState->accelRate,
                                                                 0.0f, 5.0f))) +
                      (al::isInWater(this) ? 1.0f : 2.0f) * sead::Vector3f(0.0f, -1.0f, 0.0f));
    }

    al::scaleVelocityExceptDirection(this, sGroundNormal, 0.95f);
    if (0.0f < al::getVelocity(this).dot({0.0f, -1.0f, 0.0f}))
        al::limitVelocityDir(this, sGroundNormal, 35.0f);

    if (tryBound(this))
        return;

    if (!rs::isOnGround(this, this)) {
        if (al::isInWater(this) && !mIsInWater)
            al::startHitReaction(this, "着水");
        return;
    }

    if (al::isVelocitySlowH(this, 3.0f) && 0.01f > mAccelerationState->accelRate) {
        if (!TryJump(this, mPlayerPuppet))
            al::setNerve(this, &NrvMotorcycle.RideWaitLand);
        return;
    }

    if (TryJumpWithCameraAbsorb(this, mPlayerPuppet)) {
        al::startAction(this, "RunLand");
        return;
    }

    updateVelocity(this, sGroundNormal, -rs::getCollidedGroundNormal(this));
    al::limitVelocityDirSign(this, -rs::getCollidedGroundNormal(this), 5.0f);
    al::setNerve(this, &NrvMotorcycle.RideRunLand);
    return;
}

void Motorcycle::endRideRunJump() {
    mPlayerAnimator->tryStartBindRideLandIfJump();
    mPose.jumpAngle = 0.0f;
}

void Motorcycle::exeRideRunBoundStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "RunLand");

    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;

    mParams->groundNormal.set(0.0f, 1.0f, 0.0f);
    updateBoundPhysics(this, mPlayerPuppet, &mPose.steerAngle, mSeRumbleState, mIsInWater);
    al::setNerveAtActionEnd(this, &NrvMotorcycle.RideRunBound);
}

void Motorcycle::exeRideRunBound() {
    if (al::isFirstStep(this))
        al::startAction(this, "Run");

    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;
    mParams->groundNormal.set(0.0f, 1.0f, 0.0f);
    updateBoundPhysics(this, mPlayerPuppet, &mPose.steerAngle, mSeRumbleState, mIsInWater);
}

void Motorcycle::exeRideRunClash() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Clash");
        mPlayerAnimator->startBindRideClash();
    }

    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;

    mPose.steerAngle = al::lerpValue(mPose.steerAngle, 0.0f, 0.025f);
    updateSeRumble(mSeRumbleState, this, mPlayerPuppet, mPose.steerAngle, true);
    al::addVelocityY(this, -2.0f);
    al::scaleVelocityY(this, 0.95f);
    if (al::isActionEnd(this) && rs::isOnGround(this, this))
        al::setNerve(this, &NrvMotorcycle.RideWait);
}

void Motorcycle::exeRideParkingSnap() {
    if (al::isFirstStep(this)) {
        mSeRumbleState->reset();
        alPadRumbleFunction::stopPadRumbleDirectValue(this, -1);
    }

    f32 rate = al::calcNerveRate(this, 6);
    ParkingParams* params = mParkingParams;
    f32 invRate = 1.0f - rate;
    mPose.steerAngle = invRate * params->pose.steerAngle;
    mPose.handleAngle = invRate * params->pose.handleAngle;
    mPose.leanAngle = invRate * params->pose.leanAngle;
    mPose.jumpAngle = invRate * params->pose.jumpAngle;

    sead::Quatf quat = sead::Quatf::unit;
    al::slerpQuat(&quat, params->quatA, params->quatB, rate);

    sead::Vector3f pos = {0.0f, 0.0f, 0.0f};
    al::lerpVec(&pos, mParkingParams->mCameraSubTargetPos, mParkingParams->vectorB, rate);
    al::resetQuatPosition(this, quat, pos);
    al::setNerveAtGreaterEqualStep(this, &RideParkingStart, 6);
}

void Motorcycle::exeRideParkingStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "ParkingStart");
    al::setNerveAtActionEnd(this, &RideParking);
}

void Motorcycle::exeRideParking() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Parking");
        al::tryOnStageSwitch(mParkingParams->actor, "SwitchMotorcycleFloorTouchBothWheelOn");
        al::onCollide(this);
    }

    updateOrientation(this, mPose.steerAngle);
    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.95f);

    if (rs::isCollidedGround(this))
        al::limitVelocityDirSign(this, al::getGravity(this), 3.0f);

    al::setNerveAtGreaterEqualStep(this, &RideParkingAfter, 8);
}

void Motorcycle::exeRideParkingAfter() {
    if (tryTriggerGetOff(this, &mPlayerPuppet, mPlayerAnimator, mColliderCameraTarget,
                         mTransCameraSubTarget))
        return;

    updateOrientation(this, mPose.steerAngle);
    al::addVelocityToGravity(this, 2.0f);
    al::scaleVelocity(this, 0.95f);

    if (rs::isCollidedGround(this))
        al::limitVelocityDirSign(this, al::getGravity(this), 3.0f);

    if (rs::isPuppetHoldActionButton(mPlayerPuppet)) {
        al::setNerve(this, &NrvMotorcycle.RideRunStart);
        return;
    }

    TryJump(this, mPlayerPuppet);
}
