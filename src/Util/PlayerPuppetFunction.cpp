#include "Util/PlayerPuppetFunction.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/HackCap.h"
#include "Player/PlayerActorBase.h"
#include "Player/PlayerAnimator.h"
#include "Player/PlayerCapFunction.h"
#include "Player/PlayerConst.h"
#include "Player/PlayerCounterForceRun.h"
#include "Player/PlayerEffect.h"
#include "Player/PlayerInput.h"
#include "Player/PlayerPuppet.h"
#include "Util/JudgeUtil.h"
#include "Util/PlayerCollisionUtil.h"
#include "Util/SensorMsgFunction.h"

namespace rs {
IUsePlayerPuppet* startPuppet(al::HitSensor* actorHitSensor, al::HitSensor* playerHitSensor) {
    PlayerPuppet* playerPuppet =
        static_cast<PlayerActorBase*>(al::getSensorHost(playerHitSensor))->getPlayerPuppet();
    playerPuppet->start(actorHitSensor, playerHitSensor);
    return playerPuppet;
}

void endBindAndPuppetNull(IUsePlayerPuppet** playerPuppet) {
    (*playerPuppet)->end();
    *playerPuppet = nullptr;
}

void endBindOnGroundAndPuppetNull(IUsePlayerPuppet** playerPuppet) {
    (*playerPuppet)->setBindEndOnGround();
    (*playerPuppet)->end();
    *playerPuppet = nullptr;
}

void endBindJumpAndPuppetNull(IUsePlayerPuppet** playerPuppet, const sead::Vector3f& endTrans,
                              s32 timeBeforeEnd) {
    (*playerPuppet)->setBindEndJump(endTrans, timeBeforeEnd);
    (*playerPuppet)->end();
    *playerPuppet = nullptr;
}

void endBindWallJumpAndPuppetNull(IUsePlayerPuppet** playerPuppet, const sead::Vector3f& endTrans,
                                  s32 timeBeforeEnd) {
    PlayerPuppet* puppet = static_cast<PlayerPuppet*>(*playerPuppet);
    puppet->setBindEndWallJump(endTrans, timeBeforeEnd);
    puppet->end();
    *playerPuppet = nullptr;
}

void endBindCapThrow(IUsePlayerPuppet** playerPuppet) {
    PlayerPuppet* puppet = static_cast<PlayerPuppet*>(*playerPuppet);
    puppet->setBindEndCapThrow();
    puppet->end();
    *playerPuppet = nullptr;
}

bool tryReceiveBindCancelMsgAndPuppetNull(IUsePlayerPuppet** playerPuppet,
                                          const al::SensorMsg* msg) {
    if (!al::isMsgBindCancel(msg))
        return false;

    *playerPuppet = nullptr;
    return true;
}

void setPuppetTrans(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& trans) {
    playerPuppet->setTrans(trans);
}

void setPuppetVelocity(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& velocity) {
    playerPuppet->setVelocity(velocity);
}

void setPuppetFront(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& front) {
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    playerPuppet->calcUp(&up);
    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatFrontUp(&quat, front, up);
    playerPuppet->setPose(quat);
}

void calcPuppetUp(sead::Vector3f* up, const IUsePlayerPuppet* playerPuppet) {
    playerPuppet->calcUp(up);
}

void setPuppetUp(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& up) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    playerPuppet->calcFront(&front);
    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatUpFront(&quat, up, front);
    playerPuppet->setPose(quat);
}

void calcPuppetFront(sead::Vector3f* front, const IUsePlayerPuppet* playerPuppet) {
    playerPuppet->calcFront(front);
}

void setPuppetQuat(IUsePlayerPuppet* playerPuppet, const sead::Quatf& quat) {
    playerPuppet->setPose(quat);
}

void resetPuppetPosition(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& trans) {
    playerPuppet->resetPosition(trans);
}

void addPuppetVelocityFall(IUsePlayerPuppet* playerPuppet) {
    const sead::Vector3f& gravity = playerPuppet->getGravity();
    sead::Vector3f velocity = playerPuppet->getVelocity();
    const PlayerConst* playerConst = static_cast<PlayerPuppet*>(playerPuppet)->getPlayerConst();
    sead::Vector3f fallVelocity = gravity * playerConst->getGravityAir();
    al::addVectorLimit(&velocity, fallVelocity, playerConst->getFallSpeedMax());
    playerPuppet->setVelocity(velocity);
}

const sead::Vector3f& getPuppetGravity(const IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->getGravity();
}

const sead::Vector3f& getPuppetVelocity(const IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->getVelocity();
}

const sead::Vector3f& getPuppetTrans(const IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->getTrans();
}

void calcPuppetQuat(sead::Quatf* quat, const IUsePlayerPuppet* playerPuppet) {
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    playerPuppet->calcFront(&front);
    playerPuppet->calcUp(&up);
    al::makeQuatUpFront(quat, up, front);
}

void startPuppetAction(IUsePlayerPuppet* playerPuppet, const char* action) {
    playerPuppet->startAction(action);
}

bool isPuppetActionEnd(const IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->isActionEnd();
}

bool isPuppetActionPlaying(const IUsePlayerPuppet* playerPuppet, const char* action) {
    return playerPuppet->isActionPlaying(action);
}

void setPuppetAnimRate(IUsePlayerPuppet* playerPuppet, f32 animRate) {
    playerPuppet->setAnimRate(animRate);
}

void setPuppetAnimBlendWeight(IUsePlayerPuppet* playerPuppet, f32 weight1, f32 weight2, f32 weight3,
                              f32 weight4, f32 weight5, f32 weight6) {
    static_cast<PlayerPuppet*>(playerPuppet)
        ->getPlayerAnimator()
        ->setBlendWeight(weight1, weight2, weight3, weight4, weight5, weight6);
}

f32 getPuppetAnimFrameMax(const IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->getAnimFrameMax();
}

void startPuppetHitReactionLand(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->startPlayerHitReaction("着地");
}

void hidePuppet(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->hide();
}

void showPuppet(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->show();
}

bool isPuppetHidden(const IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->isHidden();
}

void syncPuppetVisibility(al::LiveActor* actor, const IUsePlayerPuppet* playerPuppet) {
    bool isPuppetHidden = playerPuppet->isHidden();
    bool isModelHidden = al::isHideModel(actor);
    if (isPuppetHidden) {
        if (!isModelHidden)
            al::hideModel(actor);
    } else if (isModelHidden) {
        al::showModel(actor);
    }
}

void hidePuppetSilhouette(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->hideSilhouette();
}

void showPuppetSilhouette(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->showSilhouette();
}

void hidePuppetShadow(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->hideShadow();
}

void showPuppetShadow(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->showShadow();
}

void validatePuppetSensor(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->validateSensor();
}

void invalidatePuppetSensor(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->invalidateSensor();
}

void invalidatePuppetCollider(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->invalidateCollisionCheck();
}

bool isCollidedPuppetGround(IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->isCollidedGround();
}

const sead::Vector3f& getCollidedPuppetGroundNormal(IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->getCollidedGroundNormal();
}

void validatePuppetCollider(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->validateCollisionCheck();
}

void requestDamage(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->requestDamage();
}

void clearStainEffect(IUsePlayerPuppet* playerPuppet) {
    PlayerEffect* effect = static_cast<PlayerPuppet*>(playerPuppet)->getPlayerEffect();
    effect->validateStainEffect();
    effect->clearStainEffect();
}

void copyPuppetDitherAlpha(IUsePlayerPuppet* playerPuppet, const al::LiveActor* actor) {
    PlayerPuppet* puppet = static_cast<PlayerPuppet*>(playerPuppet);
    PlayerAnimator* animator = puppet->getPlayerAnimator();
    animator->setModelAlpha(al::getModelAlphaMask(actor));
    puppet->markDitherAlphaCopied();
}

void validatePuppetLookAt(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->validateLookAt();
}

void invalidatePuppetLookAt(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->invalidateLookAt();
}

void validatePuppetForceLookAt(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& lookAt) {
    static_cast<PlayerPuppet*>(playerPuppet)->validateForceLookAt(lookAt);
}

void invalidatePuppetForceLookAt(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->invalidateForceLookAt();
}

void validatePuppetSeparateMode(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->validateSeparateMode();
}

void invalidatePuppetSeparateMode(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->invalidateSeparateMode();
}

void validatePuppetReceivePush(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->validateReceivePush();
}

void invalidatePuppetReceivePush(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->invalidateReceivePush();
}

void forcePutOnPuppetCap(IUsePlayerPuppet* playerPuppet) {
    PlayerPuppet* puppet = static_cast<PlayerPuppet*>(playerPuppet);
    CapFunction::putOnCapPlayer(puppet->getHackCap(), puppet->getPlayerAnimator());
    puppet->invalidateSeparateMode();
}

void forcePutOnPuppetCapWithEffect(IUsePlayerPuppet* playerPuppet) {
    PlayerPuppet* puppet = static_cast<PlayerPuppet*>(playerPuppet);
    HackCap* hackCap = puppet->getHackCap();
    bool isHide = hackCap->isHide();
    CapFunction::putOnCapPlayer(puppet->getHackCap(), puppet->getPlayerAnimator());
    puppet->invalidateSeparateMode();
    if (!isHide)
        al::startHitReaction(hackCap, "エフェクト付き強制装着");
}

void setForceRunAfterEndBind(IUsePlayerPuppet* playerPuppet, s32 frame, f32 speed) {
    static_cast<PlayerPuppet*>(playerPuppet)
        ->getPlayerCounterForceRun()
        ->setupForceRun(frame, speed);
}

void validateRecoveryArea(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->validateRecoveryArea();
}

void invalidateRecoveryArea(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->invalidateRecoveryArea();
}

void validateGuideArrow(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->validateGuideArrow();
}

void invalidateGuideArrow(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->invalidateGuideArrow();
}

void validateWaterSurfaceShadow(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->validateWaterSurfaceShadow();
}

void invalidateWaterSurfaceShadow(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->invalidateWaterSurfaceShadow();
}

bool isPuppetInputCapThrow(const IUsePlayerPuppet* playerPuppet) {
    const PlayerInput* input = static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput();
    if (input)
        return input->isTriggerSpinCap();
    return false;
}

bool isPuppetInputSwing(const IUsePlayerPuppet* playerPuppet) {
    const PlayerInput* input = static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput();
    if (!input)
        return false;
    if (input->isTriggerSwingLeftHand())
        return true;
    return input->isTriggerSwingRightHand();
}

BindKeepDemoInfo* initBindKeepDemoInfo() {
    return new BindKeepDemoInfo;
}

bool isActiveBindKeepDemo(const BindKeepDemoInfo* bindKeepDemoInfo) {
    return bindKeepDemoInfo->isActive;
}

bool tryStartBindKeepDemo(BindKeepDemoInfo* bindKeepDemoInfo, const al::SensorMsg* msg,
                          const IUsePlayerPuppet* playerPuppet) {
    if (!isMsgBindKeepDemoStart(msg))
        return false;

    bindKeepDemoInfo->isActive = true;
    bindKeepDemoInfo->trans.set(playerPuppet->getTrans());
    sead::Vector3f front = {0.0f, 0.0f, 0.0f};
    sead::Vector3f up = {0.0f, 0.0f, 0.0f};
    sead::Quatf* quat = &bindKeepDemoInfo->quat;
    playerPuppet->calcFront(&front);
    playerPuppet->calcUp(&up);
    al::makeQuatUpFront(quat, up, front);
    return true;
}

bool tryEndBindKeepDemo(BindKeepDemoInfo* bindKeepDemoInfo, const al::SensorMsg* msg,
                        IUsePlayerPuppet* playerPuppet) {
    if (!isMsgBindKeepDemoEnd(msg))
        return false;

    playerPuppet->setPose(bindKeepDemoInfo->quat);
    playerPuppet->resetPosition(bindKeepDemoInfo->trans);
    bindKeepDemoInfo->isActive = false;
    bindKeepDemoInfo->trans.x = 0.0f;
    bindKeepDemoInfo->trans.y = 0.0f;
    bindKeepDemoInfo->trans.z = 0.0f;
    bindKeepDemoInfo->quat.w = 1.0f;
    bindKeepDemoInfo->quat.x = 0.0f;
    bindKeepDemoInfo->quat.y = 0.0f;
    bindKeepDemoInfo->quat.z = 0.0f;
    return true;
}

bool isPuppetTrigJumpButton(const IUsePlayerPuppet* playerPuppet) {
    return judgeAndResetReturnTrue(
        static_cast<const PlayerPuppet*>(playerPuppet)->getJumpButtonJudge());
}

bool isPuppetHoldJumpButton(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput()->isHoldJump();
}

bool isPuppetTrigActionButton(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput()->isTriggerAction();
}

bool isPuppetHoldActionButton(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput()->isHoldAction();
}

bool isPuppetTriggerAnyButton(const IUsePlayerPuppet* playerPuppet) {
    const PlayerPuppet* puppet = static_cast<const PlayerPuppet*>(playerPuppet);
    if (puppet->getPlayerInput()->isTriggerAction())
        return true;
    return judgeAndResetReturnTrue(puppet->getJumpButtonJudge());
}

bool isPuppetHoldAnyButton(const IUsePlayerPuppet* playerPuppet) {
    const PlayerPuppet* puppet = static_cast<const PlayerPuppet*>(playerPuppet);
    if (puppet->getPlayerInput()->isHoldAction())
        return true;
    return puppet->getPlayerInput()->isHoldJump();
}

bool isPuppetMoveStickOn(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput()->isMove();
}

bool isPuppetStartWorldWarp(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)
        ->getPlayerInput()
        ->isTriggerStartWorldWarp();
}

bool isPuppetCancelWorldWarp(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)
        ->getPlayerInput()
        ->isTriggerCancelWorldWarp();
}

const sead::Vector2f* getPuppetMoveStick(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput()->getStickMoveRaw();
}

bool isTriggerGetOff(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput()->isTriggerGetOff();
}

f32 getPuppetPoseRotZDegreeLeft(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)
        ->getPlayerInput()
        ->getPoseRotZDegreeLeft();
}

f32 getPuppetPoseRotZDegreeRight(const IUsePlayerPuppet* playerPuppet) {
    return static_cast<const PlayerPuppet*>(playerPuppet)
        ->getPlayerInput()
        ->getPoseRotZDegreeRight();
}

void requestUpdateSafetyPoint(IUsePlayerPuppet* playerPuppet, const al::LiveActor* actor,
                              const IUsePlayerCollision* playerCollision) {
    PlayerPuppet* puppet = static_cast<PlayerPuppet*>(playerPuppet);
    sead::Vector3f recoveryTrans;
    sead::Vector3f recoveryNormal;
    sead::Vector3f* safetyPoint;
    const al::AreaObj* areaObj;
    sead::Vector3f* recoveryTransPtr = &recoveryTrans;
    al::HitSensor* playerHitSensor = puppet->getPlayerHitSensor();
    const IUsePlayerCollision* collision = playerCollision;
    const al::LiveActor* actorPtr = actor;
    sead::Vector3f* recoveryNormalPtr = &recoveryNormal;
    const al::AreaObj** areaObjPtr = &areaObj;
    sead::Vector3f** safetyPointPtr = &safetyPoint;

    recoveryTrans = {0.0f, 0.0f, 0.0f};
    recoveryNormal = {0.0f, 0.0f, 0.0f};
    safetyPoint = nullptr;
    areaObj = nullptr;

    if (calcActorCollidedGroundSafetyPoint(recoveryTransPtr, recoveryNormalPtr, areaObjPtr,
                                           safetyPointPtr, actorPtr, playerHitSensor, collision)) {
        puppet->requestUpdateRecoveryInfo(true, false, recoveryTrans, recoveryNormal, areaObj);
    } else {
        bool isNoSafetyPoint = false;
        recoveryTrans = al::getTrans(actor);
        calcActorCollisionCodeNoSafetyPointPos(&recoveryTrans, &isNoSafetyPoint, actor,
                                               playerCollision);
        puppet->requestUpdateRecoveryInfo(false, isNoSafetyPoint, recoveryTrans, recoveryNormal,
                                          areaObj);
    }
}

void requestForceSafetyPoint(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& recoveryTrans,
                             const sead::Vector3f& recoveryNormal) {
    static_cast<PlayerPuppet*>(playerPuppet)
        ->requestUpdateRecoveryInfo(true, false, recoveryTrans, recoveryNormal, nullptr);
}
}  // namespace rs
