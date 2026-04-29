#include "Util/PlayerHackFunction.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Audio/System/AudioKeeper.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/ParabolicPath.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Player/CapTargetInfo.h"
#include "Player/HackCap.h"
#include "Player/IPlayerModelChanger.h"
#include "Player/IUsePlayerHack.h"
#include "Player/IUsePlayerHeightCheck.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerDamageKeeper.h"
#include "Player/PlayerHackKeeper.h"
#include "Player/PlayerInfo.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerRecoverySafetyPoint.h"
#include "System/GameDataUtil.h"
#include "Util/ObjUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace rs {
CapTargetInfo* createCapTargetInfo(al::LiveActor* actor, const char* name) {
    CapTargetInfo* capTargetInfo = new CapTargetInfo();
    CapTargetInfoFunction::initIterCapTargetInfo(capTargetInfo, nullptr, actor, name);
    return capTargetInfo;
}

CapTargetInfo* createCapTargetInfoWithPlayerCollider(al::LiveActor* actor,
                                                     IUsePlayerCollision* playerCollision,
                                                     const char* name) {
    CapTargetInfo* capTargetInfo = new CapTargetInfo();
    CapTargetInfoFunction::initIterCapTargetInfo(capTargetInfo, playerCollision, actor, name);
    return capTargetInfo;
}

CapTargetInfo* createCapTargetInfoHackMapParts(al::LiveActor* actor) {
    CapTargetInfo* capTargetInfo = createCapTargetInfo(actor, nullptr);
    if (!capTargetInfo->getHackName())
        capTargetInfo->setHackName("HackMapParts");
    return capTargetInfo;
}

CapTargetInfo* createCapTargetInfoLockOnOnlyNoArchive(al::LiveActor* actor,
                                                      const sead::Matrix34f* poseMtx) {
    CapTargetInfo* capTargetInfo = new CapTargetInfo();
    capTargetInfo->init(actor, nullptr);
    capTargetInfo->setPoseMatrix(poseMtx);
    capTargetInfo->setIsLockOnOnly();
    return capTargetInfo;
}

bool requestLockOnCapHitReaction(const al::LiveActor* actor, const CapTargetInfo* capTargetInfo,
                                 const char* name) {
    PlayerActorBase* player = (PlayerActorBase*)al::getPlayerActor(actor, 0);
    PlayerInfo* info = player->getPlayerInfo();
    if (info && info->getHackCap())
        return info->getHackCap()->requestLockOnHitReaction(capTargetInfo, name);
    return false;
}

IUsePlayerHack* startHack(al::HitSensor* hackSensor, al::HitSensor* parentSensor,
                          al::LiveActor* hackActor) {
    IUsePlayerHack* playerHack = (IUsePlayerHack*)(PlayerActorBase*)al::getSensorHost(parentSensor);
    playerHack->getPlayerHackKeeper()->startHack(hackSensor, parentSensor, hackActor);
    return playerHack;
}

void endHack(IUsePlayerHack** playerHack) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void initHackEndParam(HackEndParam* hackEndParam, PlayerHackKeeper* hackKeeper) {
    sead::Vector3f followTrans;
    sead::Vector3f hackFront;
    sead::Vector3f hackUp;
    sead::Vector3f up;

    al::LiveActor* hack = hackKeeper->getHack();

    al::calcFrontDir(&hackFront, hack);
    al::normalize(&hackFront);

    al::calcUpDir(&hackUp, hack);
    al::normalize(&hackUp);

    if (hackKeeper->isCancellingHack()) {
        up = -al::getGravity(hack);
        PlayerInput* playerInput = hackKeeper->getInput();
        sead::Vector3f* front = &hackEndParam->dir;

        if (playerInput->isMove())
            playerInput->calcMoveDirection(front, -al::getGravity(hack));
        else
            hackEndParam->dir.set(hackFront);

        if (al::isParallelDirection(*front, up, 0.01f))
            al::makeQuatUpNoSupport(&hackEndParam->pose, up);
        else
            al::makeQuatFrontUp(&hackEndParam->pose, *front, up);
    } else {
        hackEndParam->dir.set(hackFront);
        al::makeQuatFrontUp(&hackEndParam->pose, hackFront, hackUp);
    }

    followTrans = {0.0f, 0.0f, 0.0f};
    hackKeeper->getHackCap()->calcHackFollowTrans(&followTrans, true);
    hackEndParam->targetPos.set(followTrans);

    hackEndParam->escapeScale = 1.0f;
    hackEndParam->frameDelay = 0;
    hackEndParam->isDamage = false;
    hackEndParam->isDestination = false;
}

void endHackDir(IUsePlayerHack** playerHack, const sead::Vector3f& dir) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.dir.set(dir);

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackTargetQuat(IUsePlayerHack** playerHack, const sead::Quatf& pose,
                       const sead::Vector3f& dir) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.pose = pose;
    hackEndParam.dir.set(dir);

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackFromTargetPos(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.targetPos.set(targetPos);

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackFromTargetPos(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos,
                          const sead::Vector3f& dir) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.targetPos.set(targetPos);
    hackEndParam.dir.set(dir);

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackFromTargetPos(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos,
                          const sead::Quatf& pose, const sead::Vector3f& dir) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.pose = pose;
    hackEndParam.targetPos.set(targetPos);
    hackEndParam.dir.set(dir);

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackWithDamage(IUsePlayerHack** playerHack) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.isDamage = true;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackWithDamageAndVelocity(IUsePlayerHack** playerHack, const sead::Vector3f& vel) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.isDamage = true;
    hackEndParam.isDestination = true;
    hackEndParam.vel = vel;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackEscapeScale(IUsePlayerHack** playerHack, f32 escapeScale) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.escapeScale = escapeScale;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackDirEscapeScale(IUsePlayerHack** playerHack, const sead::Vector3f& dir,
                           f32 escapeScale) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.dir.set(dir);
    hackEndParam.escapeScale = escapeScale;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackDirEscapeScalePose(IUsePlayerHack** playerHack, const sead::Quatf& pose,
                               const sead::Vector3f& dir, f32 escapeScale) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.dir.set(dir);
    hackEndParam.escapeScale = escapeScale;
    hackEndParam.pose = pose;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackAirVelocity(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos,
                        const sead::Quatf& pose, const sead::Vector3f& vel, s32 frameDelay) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.isDestination = true;
    hackEndParam.vel = vel;
    hackEndParam.targetPos = targetPos;
    hackEndParam.pose = pose;
    hackEndParam.frameDelay = frameDelay;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

inline sead::Vector3f calcVel(sead::Vector3f posAfterHack, const sead::Vector3f& targetPos,
                              f32 scale) {
    sead::Vector3f vel;
    vel.setScale(posAfterHack - targetPos, scale);
    return vel;
}

void endHackElectricWireToDestination(IUsePlayerHack** playerHack, const sead::Vector3f& targetPos,
                                      const sead::Vector3f& posAfterHack) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.isDestination = true;

    sead::Vector3f vel = calcVel(posAfterHack, targetPos, 0.04f);
    vel.y = 8.0f;
    hackEndParam.vel.set(vel);

    hackEndParam.targetPos = targetPos;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackFastenerToDestination(IUsePlayerHack** playerHack, const sead::Vector3f& start,
                                  const sead::Vector3f& end) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.isDestination = true;
    al::ParabolicPath path;
    path.initFromUpVector(start, end, sead::Vector3f::ey, 500.0f);

    sead::Vector3f posAfterHack;
    path.calcPosition(&posAfterHack, 0.5f);

    hackEndParam.vel = calcVel(posAfterHack, start, 0.05f);
    hackEndParam.targetPos = start;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

// NON-MATCHING: https://decomp.me/scratch/SNZGx
void endHackBazookaElectricToDestination(IUsePlayerHack** playerHack, const sead::Vector3f& start,
                                         const sead::Vector3f& end, const sead::Quatf& pose,
                                         s32 frameDelay, f32 height) {
    sead::Vector3f start2 = start;
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.isDestination = true;
    hackEndParam.frameDelay = frameDelay;

    al::ParabolicPath path;
    f32 maxHeight = height;
    if (start2.y < end.y)
        maxHeight = (end.y - start2.y) + height;
    path.initFromUpVector(start2, end, sead::Vector3f::ey, maxHeight);

    f32 prev = 0.0f;
    f32 prevStep = 0.0f;
    s32 step = 600;
    for (s32 i = 1; i < 600; i++) {
        f32 next = prevStep + prev;
        if (al::isInRange(maxHeight, prev, next)) {
            step = i;
            break;
        }

        prevStep = sead::Mathf::clamp(prevStep + 1.0f, -35.0f, 35.0f);
        prev = next;
    }

    prevStep = 0.0f;
    f32 target = end.y;
    prev = maxHeight + start2.y + 0.0f;
    s32 step2 = 600;
    for (s32 i = 1; i < 600; i++) {
        f32 next = prevStep + prev;
        if (al::isInRange(target, prev, next)) {
            step2 = i;
            break;
        }

        prevStep = sead::Mathf::clamp(prevStep - 1.0f, -35.0f, 35.0f);
        maxHeight = next;
    }

    maxHeight = 1.0f / (step2 + step);
    f32 vely = step;
    hackEndParam.vel = maxHeight * (end - start2);
    hackEndParam.vel.y = vely;

    hackEndParam.targetPos = start2;
    hackEndParam.pose = pose;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

void endHackThrowed(IUsePlayerHack** playerHack, const sead::Vector3f& vel) {
    PlayerHackKeeper* hackKeeper = (*playerHack)->getPlayerHackKeeper();
    HackEndParam hackEndParam;

    initHackEndParam(&hackEndParam, hackKeeper);
    hackEndParam.isDestination = true;
    hackEndParam.vel = vel;

    hackKeeper->endHack(&hackEndParam);
    *playerHack = nullptr;
}

bool requestDamage(IUsePlayerHack* playerHack) {
    if (playerHack->getPlayerHackKeeper()->getDamageKeeper()->isUpdateModel())
        return false;
    return playerHack->getPlayerHackKeeper()->requestDamage();
}

void syncDamageVisibility(al::LiveActor* actor, const IUsePlayerHack* playerHack) {
    if (isDamageVisibilityHide(playerHack)) {
        al::hideModelIfShow(actor);
        return;
    }
    al::showModelIfHide(actor);
}

bool isDamageVisibilityHide(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getDamageKeeper()->isUpdateModel() &&
           playerHack->getPlayerHackKeeper()->getModelChanger()->isHiddenModel();
}

bool isTouchHackCancelCollisionCode(const al::LiveActor* actor,
                                    const IUsePlayerCollision* playerCollider) {
    if (playerCollider) {
        return isTouchDamageFireCode3D(actor, playerCollider) ||
               isTouchDeadCode3D(actor, playerCollider);
    }

    if (al::isExistActorCollider(actor)) {
        return al::isCollidedFloorCode(actor, "DamageFire") ||
               al::isCollidedFloorCode(actor, "Poison");
    }

    return false;
}

bool isTouchHackDamageCollisionCode(const al::LiveActor* actor,
                                    const IUsePlayerCollision* playerCollider) {
    if (playerCollider)
        return isTouchDamageCode(actor, playerCollider);

    return al::isExistActorCollider(actor) && al::isCollidedFloorCode(actor, "Needle");
}

void updateMaterialCode(al::LiveActor* actor, const IUsePlayerCollision* playerCollider) {
    if (isCollidedGround(playerCollider)) {
        al::setMaterialCode(actor, getMaterialCodeGround(playerCollider));
        return;
    }
    al::setMaterialCode(actor, "NoCode");
}

void startHackStartDemo(IUsePlayerHack* playerHack, al::LiveActor* actor) {
    playerHack->getPlayerHackKeeper()->startHackStartDemo(actor);
}

void startHackStartDemoPuppetable(IUsePlayerHack* playerHack, al::LiveActor* actor) {
    playerHack->getPlayerHackKeeper()->startHackStartDemoPuppetable(actor);
}

void addHackStartDemoActor(IUsePlayerHack* playerHack, al::LiveActor* actor) {
    playerHack->getPlayerHackKeeper()->addHackStartDemo(actor);
}

void endHackStartDemo(IUsePlayerHack* playerHack, al::LiveActor* actor) {
    playerHack->getPlayerHackKeeper()->endHackStartDemo(actor);
}

bool tryEndHackStartDemo(IUsePlayerHack* playerHack, al::LiveActor* actor) {
    if (isActiveHackStartDemo(playerHack)) {
        endHackStartDemo(playerHack, actor);
        return true;
    }
    return false;
}

bool isActiveHackStartDemo(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->isActiveHackStartDemo();
}

bool isHackStartDemoEnterMario(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->isPuppetable();
}

// NON-MATCHING: https://decomp.me/scratch/XXfMn
bool isHackerStopMove(const al::LiveActor* actor, const IUsePlayerHack* playerHack,
                      f32 speedLimit) {
    if (playerHack->getPlayerHackKeeper()->getInput()->isMove())
        return false;

    return al::calcSpeedH(actor) < speedLimit;
}

// NON-MATCHING: https://decomp.me/scratch/GMrwl
bool isHackerStopMoveGround(const al::LiveActor* actor, const IUsePlayerHack* playerHack,
                            f32 speedLimit, const IUsePlayerCollision* playerCollider) {
    if (playerHack->getPlayerHackKeeper()->getInput()->isMove())
        return false;

    if (playerCollider) {
        if (isCollidedGround(playerCollider)) {
            if (al::calcSpeedExceptDir(actor, getCollidedGroundNormal(playerCollider)) >=
                speedLimit)
                return false;
            return true;
        }

        if (isHackerStopMove(actor, playerHack, speedLimit))
            return false;
        return true;
    }

    if (isCollidedGround(actor)) {
        if (al::calcSpeedExceptDir(actor, al::getCollidedGroundNormal(actor)) >= speedLimit)
            return false;
        return true;
    }

    if (isHackerStopMove(actor, playerHack, speedLimit))
        return false;
    return true;
}

bool sendMsgHackerNoReaction(const IUsePlayerHack* playerHack, al::HitSensor* receiver,
                             al::HitSensor* sender) {
    return sendMsgPlayerItemGetAll(receiver, sender);
}

void sendMsgHackerNoReactionWithoutShine(const IUsePlayerHack* playerHack, al::HitSensor* receiver,
                                         al::HitSensor* sender) {
    al::sendMsgPlayerItemGet(receiver, sender);
}

bool isMsgHackerDamageAndCancel(const al::SensorMsg* message) {
    return !isMsgEnemyAttack2D(message) && isMsgPlayerDamage(message);
}

bool isMsgEnableMapCheckPointWarpCollidedGround(const al::SensorMsg* message,
                                                const al::LiveActor* actor) {
    return isMsgEnableMapCheckPointWarp(message) && al::isCollidedGround(actor);
}

bool isMsgEnableMapCheckPointWarpCollidedGround(const al::SensorMsg* message,
                                                const IUsePlayerCollision* playerCollider) {
    return isMsgEnableMapCheckPointWarp(message) && isCollidedGround(playerCollider);
}

bool receiveMsgRequestTransferHack(const al::SensorMsg* message, IUsePlayerHack* playerHack,
                                   al::HitSensor* hitSensor) {
    return isMsgRequestTransferHack(message) &&
           playerHack->getPlayerHackKeeper()->receiveRequestTransferHack(hitSensor);
}

void hideHackCap(IUsePlayerHack* playerHack) {
    PlayerHackKeeper* hackKeeper = playerHack->getPlayerHackKeeper();
    if (hackKeeper->getHackCap())
        hackKeeper->getHackCap()->setIsHide(true);
}

void showHackCap(IUsePlayerHack* playerHack) {
    PlayerHackKeeper* hackKeeper = playerHack->getPlayerHackKeeper();
    if (hackKeeper->getHackCap())
        hackKeeper->getHackCap()->setIsHide(false);
}

void hideShadowHackCap(IUsePlayerHack* playerHack) {
    PlayerHackKeeper* hackKeeper = playerHack->getPlayerHackKeeper();
    if (hackKeeper->getHackCap())
        al::invalidateShadow(hackKeeper->getHackCap());
}

bool isHackCapSeparateFlying(IUsePlayerHack* playerHack) {
    PlayerHackKeeper* hackKeeper = playerHack->getPlayerHackKeeper();
    if (hackKeeper->getHackCap())
        return hackKeeper->getHackCap()->isSeparateFlying();
    return false;
}

f32 calcGroundHeightHackerDepthShadowMapLength(al::LiveActor* actor,
                                               const IUsePlayerHack* playerHack,
                                               const sead::Vector3f& param3, f32 param4,
                                               f32 maxHeight) {
    IUsePlayerHeightCheck* heightCheck = playerHack->getPlayerHackKeeper()->getHeightCheck();
    if (!heightCheck->isAboveGround())
        return maxHeight;

    const sead::Vector3f& up = -al::getGravity(actor);

    f32 dotUpDir = (al::getTrans(actor) - getPlayerPos(actor)).dot(up);
    f32 dropHeight = heightCheck->getShadowDropHeight() * 1.2f;
    f32 dotup3 = param3.dot(up);
    f32 sqrt = sead::Mathf::sqrt(sead::Mathf::clampMin(1.0f - dotup3 * dotup3, 0.0f)) * 300.0f;

    return sead::Mathf::clamp(dropHeight + sqrt + param4 + dotUpDir, 1.0f, maxHeight);
}

void cutGroundHeightHackerDepthShadowMapLength(al::LiveActor* actor,
                                               const IUsePlayerHack* playerHack,
                                               const char* mapName, const sead::Vector3f& param3,
                                               f32 param4, f32 maxHeight) {
    al::setDepthShadowMapLength(
        actor,
        calcGroundHeightHackerDepthShadowMapLength(actor, playerHack, param3, param4, maxHeight),
        mapName);
}

bool checkExistHeightSpaceAboveGround(f32* out, const IUsePlayerHack* playerHack, f32 maxHeight) {
    IUsePlayerHeightCheck* heightCheck = playerHack->getPlayerHackKeeper()->getHeightCheck();
    if (!heightCheck->isAboveGround())
        return true;

    f32 height = heightCheck->getGroundHeight();
    if (height < maxHeight) {
        *out = height;
        return false;
    }

    *out = maxHeight;
    return true;
}

bool requestRecoverySafetyPoint(IUsePlayerHack* playerHack, const sead::Vector3f& pos,
                                const sead::Vector3f& normal) {
    PlayerHackKeeper* hackKeeper = playerHack->getPlayerHackKeeper();
    if (isKidsMode(hackKeeper->getParent())) {
        hackKeeper->getRecoverySafePoint()->setSafetyPoint(pos, normal, nullptr);
        return true;
    }
    return false;
}

bool trySnapTransToSnapMoveArea(al::LiveActor* actor, bool param2, const sead::Vector3f& param3,
                                f32 param4) {
    sead::Vector3f snapTrans = {0.0f, 0.0f, 0.0f};

    if (calcSnapTransSnapMoveAreaNoCollider(&snapTrans, actor, param2, param3,
                                            al::getVelocity(actor), param4)) {
        al::setTrans(actor, al::getTrans(actor) + snapTrans);
        return true;
    }

    return false;
}

void updateHackActorVisibilityBySeparatePlay(al::LiveActor* actor, IUsePlayerHack* playerHack,
                                             const char* animName,
                                             const char* animSeparateFlyingName) {
    if (isHackCapSeparateFlying(playerHack)) {
        if (!al::isVisAnimPlaying(actor, animSeparateFlyingName))
            al::startVisAnim(actor, animSeparateFlyingName);
        return;
    }

    if (!al::isVisAnimPlaying(actor, animName))
        al::startVisAnim(actor, animName);
}

void startReset(al::LiveActor* actor) {
    if (actor->getAudioKeeper())
        actor->getAudioKeeper()->kill();

    if (actor->getEffectKeeper())
        actor->getEffectKeeper()->deleteAndClearEffectAll();
}

void endReset(al::LiveActor* actor) {
    if (actor->getAudioKeeper())
        actor->getAudioKeeper()->appear();
}

}  // namespace rs
