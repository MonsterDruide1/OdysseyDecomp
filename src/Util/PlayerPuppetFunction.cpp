#include "Util/PlayerPuppetFunction.h"

#include <math/seadQuat.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/HackCap.h"
#include "Player/IUsePlayerPuppet.h"
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

class BindKeepDemoInfo {
public:
    BindKeepDemoInfo() : mIsActive(false), mTrans(0.0f, 0.0f, 0.0f), mQuat(sead::Quatf::unit) {}

    bool mIsActive;
    sead::Vector3f mTrans;
    sead::Quatf mQuat;
};

static_assert(sizeof(BindKeepDemoInfo) == 0x20);

namespace rs {

void setPuppetTrans(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& trans) {
    playerPuppet->setTrans(trans);
}

void setPuppetVelocity(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& velocity) {
    playerPuppet->setVelocity(velocity);
}

void setPuppetQuat(IUsePlayerPuppet* playerPuppet, const sead::Quatf& quat) {
    playerPuppet->setPose(quat);
}

void resetPuppetPosition(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& trans) {
    playerPuppet->resetPosition(trans);
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

f32 getPuppetAnimFrameMax(const IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->getAnimFrameMax();
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

void requestDamage(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->requestDamage();
}

void validatePuppetCollider(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->validateCollisionCheck();
}

void invalidatePuppetCollider(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->invalidateCollisionCheck();
}

bool isCollidedPuppetGround(IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->isCollidedGround();
}

void validatePuppetSensor(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->validateSensor();
}

void invalidatePuppetSensor(IUsePlayerPuppet* playerPuppet) {
    playerPuppet->invalidateSensor();
}

void calcPuppetUp(sead::Vector3f* up, const IUsePlayerPuppet* playerPuppet) {
    playerPuppet->calcUp(up);
}

bool tryReceiveBindCancelMsgAndPuppetNull(IUsePlayerPuppet** playerPuppet,
                                          const al::SensorMsg* msg) {
    if (al::isMsgBindCancel(msg)) {
        *playerPuppet = nullptr;
        return true;
    }
    return false;
}

BindKeepDemoInfo* initBindKeepDemoInfo() {
    return new BindKeepDemoInfo();
}

bool isActiveBindKeepDemo(const BindKeepDemoInfo* bindKeepDemoInfo) {
    return bindKeepDemoInfo->mIsActive;
}

bool tryStartBindKeepDemo(BindKeepDemoInfo* bindKeepDemoInfo, const al::SensorMsg* msg,
                          const IUsePlayerPuppet* playerPuppet) {
    if (isMsgBindKeepDemoStart(msg)) {
        bindKeepDemoInfo->mIsActive = true;
        bindKeepDemoInfo->mTrans.set(playerPuppet->getTrans());
        sead::Quatf* quat = &bindKeepDemoInfo->mQuat;
        sead::Vector3f front(0.0f, 0.0f, 0.0f);
        sead::Vector3f up(0.0f, 0.0f, 0.0f);
        playerPuppet->calcFront(&front);
        playerPuppet->calcUp(&up);
        al::makeQuatUpFront(quat, up, front);
        return true;
    }
    return false;
}

bool tryEndBindKeepDemo(BindKeepDemoInfo* bindKeepDemoInfo, const al::SensorMsg* msg,
                        IUsePlayerPuppet* playerPuppet) {
    if (isMsgBindKeepDemoEnd(msg)) {
        playerPuppet->setPose(bindKeepDemoInfo->mQuat);
        playerPuppet->resetPosition(bindKeepDemoInfo->mTrans);
        bindKeepDemoInfo->mIsActive = false;
        bindKeepDemoInfo->mTrans.set(0.0f, 0.0f, 0.0f);
        bindKeepDemoInfo->mQuat.set(1.0f, 0.0f, 0.0f, 0.0f);
        return true;
    }
    return false;
}

void syncPuppetVisibility(al::LiveActor* actor, const IUsePlayerPuppet* playerPuppet) {
    bool isHidden = playerPuppet->isHidden();
    bool isHideModel = al::isHideModel(actor);
    if (isHidden) {
        if (!isHideModel)
            al::hideModel(actor);
    } else {
        if (isHideModel)
            al::showModel(actor);
    }
}

void calcPuppetFront(sead::Vector3f* front, const IUsePlayerPuppet* playerPuppet) {
    playerPuppet->calcFront(front);
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

void startPuppetAction(IUsePlayerPuppet* playerPuppet, const char* action) {
    playerPuppet->startAction(action);
}

const sead::Vector3f& getCollidedPuppetGroundNormal(IUsePlayerPuppet* playerPuppet) {
    return playerPuppet->getCollidedGroundNormal();
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

void startPuppetHitReactionLand(IUsePlayerPuppet* playerPuppet) {
    static_cast<PlayerPuppet*>(playerPuppet)->startPlayerHitReaction("着地");
}

void setPuppetAnimBlendWeight(IUsePlayerPuppet* playerPuppet, f32 weight1, f32 weight2, f32 weight3,
                              f32 weight4, f32 weight5, f32 weight6) {
    static_cast<PlayerPuppet*>(playerPuppet)
        ->getPlayerAnimator()
        ->setBlendWeight(weight1, weight2, weight3, weight4, weight5, weight6);
}

void setForceRunAfterEndBind(IUsePlayerPuppet* playerPuppet, s32 frames, f32 speed) {
    static_cast<PlayerPuppet*>(playerPuppet)
        ->getPlayerCounterForceRun()
        ->setupForceRun(frames, speed);
}

bool isPuppetInputCapThrow(const IUsePlayerPuppet* playerPuppet) {
    const PlayerInput* input = static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput();
    if (input == nullptr)
        return false;
    return input->isTriggerSpinCap();
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

sead::Vector2f getPuppetMoveStick(const IUsePlayerPuppet* playerPuppet) {
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

bool isPuppetTrigJumpButton(const IUsePlayerPuppet* playerPuppet) {
    return judgeAndResetReturnTrue(
        static_cast<const PlayerPuppet*>(playerPuppet)->getJudgePreInputJump());
}

bool isPuppetTriggerAnyButton(const IUsePlayerPuppet* playerPuppet) {
    const PlayerPuppet* puppet = static_cast<const PlayerPuppet*>(playerPuppet);
    return puppet->getPlayerInput()->isTriggerAction() ||
           judgeAndResetReturnTrue(puppet->getJudgePreInputJump());
}

bool isPuppetHoldAnyButton(const IUsePlayerPuppet* playerPuppet) {
    const PlayerPuppet* puppet = static_cast<const PlayerPuppet*>(playerPuppet);
    return puppet->getPlayerInput()->isHoldAction() || puppet->getPlayerInput()->isHoldJump();
}

IUsePlayerPuppet* startPuppet(al::HitSensor* actorHitSensor, al::HitSensor* playerHitSensor) {
    PlayerPuppet* puppet =
        static_cast<PlayerActorBase*>(al::getSensorHost(playerHitSensor))->getPlayerPuppet();
    puppet->start(actorHitSensor, playerHitSensor);
    return puppet;
}

void addPuppetVelocityFall(IUsePlayerPuppet* playerPuppet) {
    const sead::Vector3f& gravity = playerPuppet->getGravity();
    sead::Vector3f velocity = playerPuppet->getVelocity();
    const PlayerConst* playerConst = static_cast<PlayerPuppet*>(playerPuppet)->getPlayerConst();
    sead::Vector3f fallVelocity = gravity * playerConst->getGravityAir();
    al::addVectorLimit(&velocity, fallVelocity, playerConst->getFallSpeedMax());
    playerPuppet->setVelocity(velocity);
}

void setPuppetFront(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& front) {
    sead::Vector3f up(0.0f, 0.0f, 0.0f);
    playerPuppet->calcUp(&up);
    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatFrontUp(&quat, front, up);
    playerPuppet->setPose(quat);
}

void setPuppetUp(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& up) {
    sead::Vector3f front(0.0f, 0.0f, 0.0f);
    playerPuppet->calcFront(&front);
    sead::Quatf quat = sead::Quatf::unit;
    al::makeQuatUpFront(&quat, up, front);
    playerPuppet->setPose(quat);
}

void calcPuppetQuat(sead::Quatf* quat, const IUsePlayerPuppet* playerPuppet) {
    sead::Vector3f front(0.0f, 0.0f, 0.0f);
    sead::Vector3f up(0.0f, 0.0f, 0.0f);
    playerPuppet->calcFront(&front);
    playerPuppet->calcUp(&up);
    al::makeQuatUpFront(quat, up, front);
}

bool isPuppetInputSwing(const IUsePlayerPuppet* playerPuppet) {
    const PlayerInput* input = static_cast<const PlayerPuppet*>(playerPuppet)->getPlayerInput();
    if (input == nullptr)
        return false;
    return input->isTriggerSwingLeftHand() || input->isTriggerSwingRightHand();
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

void clearStainEffect(IUsePlayerPuppet* playerPuppet) {
    PlayerEffect* playerEffect = static_cast<PlayerPuppet*>(playerPuppet)->getPlayerEffect();
    playerEffect->requestClearStainEffect();
    playerEffect->clearStainEffect();
}

void copyPuppetDitherAlpha(IUsePlayerPuppet* playerPuppet, const al::LiveActor* actor) {
    PlayerPuppet* puppet = static_cast<PlayerPuppet*>(playerPuppet);
    puppet->getPlayerAnimator()->setModelAlpha(al::getModelAlphaMask(actor));
    puppet->setCopyDitherAlpha();
}

void requestUpdateSafetyPoint(IUsePlayerPuppet* playerPuppet, const al::LiveActor* actor,
                              const IUsePlayerCollision* playerCollision) {
    PlayerPuppet* puppet = static_cast<PlayerPuppet*>(playerPuppet);
    al::HitSensor* hitSensor = puppet->getHitSensor();
    sead::Vector3f safetyPoint(0.0f, 0.0f, 0.0f);
    sead::Vector3f safetyPointNormal(0.0f, 0.0f, 0.0f);
    sead::Vector3f* recoveryTrans = nullptr;
    const al::AreaObj* areaObj = nullptr;
    if (calcActorCollidedGroundSafetyPoint(&safetyPoint, &safetyPointNormal, &areaObj,
                                           &recoveryTrans, actor, hitSensor, playerCollision)) {
        puppet->requestUpdateRecoveryInfo(true, false, safetyPoint, safetyPointNormal, areaObj);
    } else {
        bool collisionCode = false;
        safetyPoint = al::getTrans(actor);
        calcActorCollisionCodeNoSafetyPointPos(&safetyPoint, &collisionCode, actor,
                                               playerCollision);
        puppet->requestUpdateRecoveryInfo(false, collisionCode, safetyPoint, safetyPointNormal,
                                          areaObj);
    }
}

void requestForceSafetyPoint(IUsePlayerPuppet* playerPuppet, const sead::Vector3f& recoveryTrans,
                             const sead::Vector3f& recoveryNormal) {
    static_cast<PlayerPuppet*>(playerPuppet)
        ->requestUpdateRecoveryInfo(true, false, recoveryTrans, recoveryNormal, nullptr);
}

}  // namespace rs
