#include "Util/PlayerHackInputFunction.h"

#include "Library/Controller/InputFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Player/IJudge.h"
#include "Player/IUsePlayerHack.h"
#include "Player/PlayerActionFunction.h"
#include "Player/PlayerActionVelocityControl.h"
#include "Player/PlayerFunction.h"
#include "Player/PlayerHackKeeper.h"
#include "Player/PlayerInput.h"
#include "Util/JudgeUtil.h"

namespace rs {

void calcHackerMoveVec(sead::Vector3f* moveVec, const IUsePlayerHack* playerHack,
                       const sead::Vector3f& up) {
    playerHack->getPlayerHackKeeper()->getInput()->calcMoveInput(moveVec, up);
}

bool calcHackerMoveDir(sead::Vector3f* moveDir, const IUsePlayerHack* playerHack,
                       const sead::Vector3f& up) {
    playerHack->getPlayerHackKeeper()->getInput()->calcMoveInput(moveDir, up);
    if (al::tryNormalizeOrZero(moveDir))
        return true;

    moveDir->set(0.0f, 0.0f, 0.0f);
    return false;
}

void calcHackerWallInputVec(sead::Vector3f* inputVec, sead::Vector3f* wallNormal,
                            const IUsePlayerHack* playerHack, const sead::Vector3f& up) {
    sead::Vector2f stick = playerHack->getPlayerHackKeeper()->getInput()->getStickMoveRaw();
    calcHackerWallInputVec(inputVec, wallNormal, stick, up);
}

void calcHackerWallInputVec(sead::Vector3f* inputVec, sead::Vector3f* wallNormal,
                            const sead::Vector2f& stick, const sead::Vector3f& up) {
    sead::Vector3f wallInput;
    al::verticalizeVec(&wallInput, up, sead::Vector3f::ey);
    if (al::tryNormalizeOrZero(&wallInput)) {
        if (wallNormal->dot(wallInput) < 0.0f)
            wallInput.negate();
        wallNormal->set(wallInput);
    }

    sead::Vector3f side;
    side.setCross(wallInput, up);
    al::tryNormalizeOrZero(&side);

    sead::Vector3f input = side * stick.x + wallInput * stick.y;
    inputVec->set(input);
}

f32 getHackMoveStickRaw(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getStickMoveRaw().x;
}

bool calcHackerWallInputDir(sead::Vector3f* inputDir, sead::Vector3f* wallNormal,
                            const IUsePlayerHack* playerHack, const sead::Vector3f& up) {
    sead::Vector2f stick = playerHack->getPlayerHackKeeper()->getInput()->getStickMoveRaw();
    calcHackerWallInputVec(inputDir, wallNormal, stick, up);
    if (al::tryNormalizeOrZero(inputDir))
        return true;

    inputDir->set(0.0f, 0.0f, 0.0f);
    return false;
}

bool addHackActorAccelStick(al::LiveActor* actor, const IUsePlayerHack* playerHack,
                            sead::Vector3f* outDir, f32 accel, const sead::Vector3f& up) {
    sead::Vector3f moveInput;
    moveInput.set(0.0f, 0.0f, 0.0f);
    playerHack->getPlayerHackKeeper()->getInput()->calcMoveInput(&moveInput, up);

    sead::Vector3f moveDir;
    moveDir.set(0.0f, 0.0f, 0.0f);
    if (al::tryNormalizeOrZero(&moveDir, moveInput)) {
        if (outDir)
            outDir->set(moveDir);

        sead::Vector3f velocity = moveInput * accel;
        al::addVelocity(actor, velocity);
        return true;
    }

    if (outDir)
        outDir->set(0.0f, 0.0f, 0.0f);
    return false;
}

f32 calcHackMovePower(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getStickMoveRaw().length();
}

bool checkHackerMoveDir(const IUsePlayerHack* playerHack, const sead::Vector3f& up,
                        const sead::Vector3f& direction, f32 degree) {
    sead::Vector3f moveDir = sead::Vector3f::zero;
    playerHack->getPlayerHackKeeper()->getInput()->calcMoveInput(&moveDir, up);
    if (!al::tryNormalizeOrZero(&moveDir))
        moveDir.set(0.0f, 0.0f, 0.0f);

    if (al::isNearZero(moveDir, 0.001f))
        return false;
    return al::isNearDirection(moveDir, direction, degree);
}

void calcHackerTrampleJumpParam(f32* jumpSpeed, f32* gravity, s32* noGravityFrames,
                                al::LiveActor* actor, const IUsePlayerHack* playerHack,
                                f32 jumpSpeedMin, f32 jumpSpeedMax, f32 jumpPowerMin,
                                f32 jumpPowerMax, f32 fallBrakeRate, s32 fallFrames,
                                f32 jumpSpeedLow, f32 jumpSpeedHigh, f32 fallingGravity,
                                f32 jumpGravityLow, f32 jumpGravityHigh) {
    const sead::Vector3f& gravityDir = al::getGravity(actor);
    bool isMove = playerHack->getPlayerHackKeeper()->getInput()->isMove();
    const sead::Vector3f& velocity = al::getVelocity(actor);

    if (velocity.dot(gravityDir) < 0.0f) {
        if (isMove)
            al::scaleVelocityExceptDirection(actor, gravityDir, fallBrakeRate);
        else
            al::setVelocityZero(actor);

        PlayerActionVelocityControl velocityControl(actor, nullptr);
        *noGravityFrames = fallFrames;
        *jumpSpeed = PlayerActionFunction::calcJumpSpeed(
            velocityControl.getVelocityFront().length(), jumpSpeedMin, jumpSpeedMax, jumpPowerMin,
            jumpPowerMax);
        *gravity = fallingGravity;
        return;
    }

    if (!isMove)
        al::setVelocityZero(actor);

    *noGravityFrames = 0;
    *jumpSpeed = playerHack->getPlayerHackKeeper()->getInput()->isHoldHackJump() ? jumpSpeedHigh :
                                                                                   jumpSpeedLow;
    *gravity = playerHack->getPlayerHackKeeper()->getInput()->isHoldHackJump() ? jumpGravityHigh :
                                                                                 jumpGravityLow;
}

bool isOnHackMoveStick(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isMove();
}

bool isHoldHackJump(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isHoldHackJump();
}

bool isOnHackMoveStickDeepDown(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isMoveDeepDown();
}

bool isOnHackMoveStickGreater(const IUsePlayerHack* playerHack, f32 threshold) {
    return playerHack->getPlayerHackKeeper()->getInput()->getStickMoveRaw().length() > threshold;
}

bool isTriggerHackAction(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackAction();
}

bool isTriggerHackPreInputAction(const IUsePlayerHack* playerHack) {
    return judgeAndResetReturnTrue(playerHack->getPlayerHackKeeper()->getJudgePreInputAction());
}

bool isHoldHackAction(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isHoldHackAction();
}

bool isReleaseHackAction(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isReleaseHackAction();
}

bool isTriggerHackJump(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackJump();
}

bool isTriggerHackPreInputJump(const IUsePlayerHack* playerHack) {
    return judgeAndResetReturnTrue(playerHack->getPlayerHackKeeper()->getJudgePreInputJump());
}

bool isReleaseHackJump(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isReleaseHackJump();
}

bool isTriggerHackAnyButton(const IUsePlayerHack* playerHack) {
    if (playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackJump())
        return true;
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackAction();
}

bool isTriggerHackPreInputAnyButton(const IUsePlayerHack* playerHack) {
    IJudge* judgeJump = playerHack->getPlayerHackKeeper()->getJudgePreInputJump();
    IJudge* judgeAction = playerHack->getPlayerHackKeeper()->getJudgePreInputAction();

    if (!isJudge(judgeJump) && !isJudge(judgeAction))
        return false;

    resetJudge(judgeJump);
    resetJudge(judgeAction);
    return true;
}

bool isHoldHackAnyButton(const IUsePlayerHack* playerHack) {
    if (playerHack->getPlayerHackKeeper()->getInput()->isHoldHackJump())
        return true;
    return playerHack->getPlayerHackKeeper()->getInput()->isHoldHackAction();
}

bool isReleaseHackAnyButton(const IUsePlayerHack* playerHack) {
    if (playerHack->getPlayerHackKeeper()->getInput()->isReleaseHackJump())
        return true;
    return playerHack->getPlayerHackKeeper()->getInput()->isReleaseHackAction();
}

bool isTriggerHackSeparateJump(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackSeparateJump();
}

bool isHoldHackSeparateJump(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isHoldHackSeparateJump();
}

f32 getHackStickRotateSpeed(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getStickRotateSpeed();
}

bool isTriggerHackSwing(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingActionMario();
}

bool isTriggerHackSwingDirLeft(const IUsePlayerHack* playerHack) {
    if (!playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingActionMario())
        return false;
    return playerHack->getPlayerHackKeeper()->getInput()->isSwingDirLeft();
}

bool isTriggerHackSwingDirRight(const IUsePlayerHack* playerHack) {
    if (!playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingActionMario())
        return false;
    return playerHack->getPlayerHackKeeper()->getInput()->isSwingDirRight();
}

bool isTriggerHackSwingDoubleHand(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingDoubleHand();
}

bool isTriggerHackSwingDoubleHandReverseDir(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingDoubleHandReverseDir();
}

bool isTriggerHackSwingDoubleHandReverseInsideDir(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()
        ->getInput()
        ->isTriggerSwingDoubleHandReverseInsideDir();
}

bool isTriggerHackSwingDoubleHandReverseOutsideDir(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()
        ->getInput()
        ->isTriggerSwingDoubleHandReverseOutsideDir();
}

f32 getHackPoseRotZDegreeLeft(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getPoseRotZDegreeLeft();
}

f32 getHackPoseRotZDegreeRight(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getPoseRotZDegreeRight();
}

bool isTriggerTankCameraReset(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerCameraReset();
}

bool isTriggerHackSwingLeftHand(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingLeftHand();
}

bool isTriggerHackSwingRightHand(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingRightHand();
}

const sead::Vector2f& getHackSwingThrowDir(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getSwingThrowDir();
}

const sead::Vector2f& getHackSwingLeftHandDir(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getSwingLeftHandDir();
}

const sead::Vector2f& getHackSwingRightHandDir(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getSwingRightHandDir();
}

const sead::Vector2f& getHackSwingVelLeftHand(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getSwingVelLeftHand();
}

const sead::Vector2f& getHackSwingVelRightHand(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getSwingVelRightHand();
}

sead::Vector2f getHackCameraStickRaw(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getStickCameraRaw();
}

f32 getRadiconInputSteeringValue(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getRadiconInputSteeringValue();
}

f32 getRadiconInputThrottleValue(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->getStickMoveRaw().y;
}

bool isTriggerRecorderPlay(const IUsePlayerHack* playerHack) {
    return al::isPadTriggerX(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

bool isTriggerRecorderReverse(const IUsePlayerHack* playerHack) {
    return al::isPadTriggerY(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

bool isHoldRecorderPlay(const IUsePlayerHack* playerHack) {
    return al::isPadHoldX(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

bool isHoldRecorderReverse(const IUsePlayerHack* playerHack) {
    return al::isPadHoldY(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

bool isTriggerRecorderJump(const IUsePlayerHack* playerHack) {
    return al::isPadTriggerB(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

bool isTriggerRecorderCancel(const IUsePlayerHack* playerHack) {
    return al::isPadTriggerA(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

const sead::Vector2f& getHackGemyLeftStickRaw() {
    return al::getLeftStick(al::getMainControllerPort());
}

const sead::Vector2f& getHackGemyRightStickRaw() {
    return al::getRightStick(al::getMainControllerPort());
}

bool isTriggerGemyResetCamera(const IUsePlayerHack* playerHack) {
    return al::isPadHoldL(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

bool isTriggerGemyChangeCamera(const IUsePlayerHack* playerHack) {
    return al::isPadTriggerR(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

bool isTriggerSubjectiveCamera(const IUsePlayerHack* playerHack) {
    return al::isPadTriggerPressRightStick(
        PlayerFunction::getPlayerInputPort(playerHack->getPlayerHackKeeper()->getParent()));
}

bool isTriggerFlutter(const IUsePlayerHack* playerHack, f32 threshold) {
    if (al::isPadConnectedJoyPadDouble()) {
        if (al::isShakePadAcceleration(threshold, -1, 0))
            return al::isShakePadAcceleration(threshold, -1, 1);
        return false;
    }

    if (al::isShakePadAcceleration(threshold, al::getMainControllerPort(), 0))
        return true;
    if (playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackJump())
        return true;
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackAction();
}

bool isTriggerFukankunZoomOn(const IUsePlayerHack* playerHack) {
    if (playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackJump())
        return true;
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackAction();
}

bool isTriggerFukankunZoomOff(const IUsePlayerHack* playerHack) {
    if (playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackJump())
        return true;
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackAction();
}

bool isHoldFukankunZoom(const IUsePlayerHack* playerHack) {
    if (playerHack->getPlayerHackKeeper()->getInput()->isHoldHackJump())
        return true;
    return playerHack->getPlayerHackKeeper()->getInput()->isHoldHackAction();
}

void getPoleJumpPadPoseMtx(sead::Matrix33f* mtx) {
    al::getPadPoseMtx(mtx, al::getMainControllerPort(), 0);
}

bool getPoleJumpPadAcceleration(sead::Vector3f* acceleration) {
    if (al::isPadConnectedJoyPadDouble())
        return al::tryGetPadAcceleration(acceleration, -1, 1);
    return al::tryGetPadAcceleration(acceleration, al::getMainControllerPort(), 0);
}

bool isTriggerIceClimberClimbLeft(const IUsePlayerHack* playerHack) {
    if (al::isPadConnectedJoyPadDouble())
        return playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingLeftHand();
    return al::isPadTriggerZL(al::getMainControllerPort());
}

bool isTriggerIceClimberClimbRight(const IUsePlayerHack* playerHack) {
    if (al::isPadConnectedJoyPadDouble())
        return playerHack->getPlayerHackKeeper()->getInput()->isTriggerSwingRightHand();
    return al::isPadTriggerZR(al::getMainControllerPort());
}

bool isTriggerIceClimberFall(const IUsePlayerHack* playerHack) {
    bool isDouble = al::isPadConnectedJoyPadDouble();
    bool isTriggerJump = playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackJump();
    if (isTriggerJump || !isDouble)
        return isDouble || isTriggerJump;
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackAction();
}

}  // namespace rs

namespace HackFunction {

bool isTriggerHackEnd(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerHackEnd();
}

bool isTriggerCancelBubbleLauncher(const IUsePlayerHack* playerHack) {
    return playerHack->getPlayerHackKeeper()->getInput()->isTriggerGetOff();
}

}  // namespace HackFunction
