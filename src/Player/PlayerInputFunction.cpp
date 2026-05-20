#include "Player/PlayerInputFunction.h"

#include "Library/Controller/InputFunction.h"
#include "Library/LiveActor/LiveActor.h"

#include "Util/StageInputFunction.h"

namespace {
constexpr s32 cNoInputJudgeKeyMaskDefault = 0xf1fff;
constexpr s32 cNoInputJudgeKeyMaskSingleJoy = 0xf7ff7;
}  // namespace

bool PlayerInputFunction::isSeparatePlaySingleJoy(const al::LiveActor* actor, s32 port) {
    return rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port);
}

bool PlayerInputFunction::isTriggerJump(const al::LiveActor* actor, s32 port) {
    return al::isPadTriggerA(port) || al::isPadTriggerB(port);
}

bool PlayerInputFunction::isHoldJump(const al::LiveActor* actor, s32 port) {
    return al::isPadHoldA(port) || al::isPadHoldB(port);
}

bool PlayerInputFunction::isReleaseJump(const al::LiveActor* actor, s32 port) {
    return al::isPadReleaseA(port) || al::isPadReleaseB(port);
}

bool PlayerInputFunction::isTriggerAction(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return al::isPadTriggerY(port);
    return al::isPadTriggerX(port) || al::isPadTriggerY(port);
}

bool PlayerInputFunction::isHoldAction(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return al::isPadHoldY(port);
    return al::isPadHoldX(port) || al::isPadHoldY(port);
}

bool PlayerInputFunction::isReleaseAction(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return al::isPadReleaseY(port);
    return al::isPadReleaseX(port) || al::isPadReleaseY(port);
}

bool PlayerInputFunction::isTriggerSubAction(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return al::isPadTriggerL(port) || al::isPadTriggerR(port);
    return al::isPadTriggerZL(port) || al::isPadTriggerZR(port);
}

bool PlayerInputFunction::isHoldSubAction(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return al::isPadHoldL(port) || al::isPadHoldR(port);
    return al::isPadHoldZL(port) || al::isPadHoldZR(port);
}

bool PlayerInputFunction::isReleaseSubAction(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return al::isPadReleaseL(port) || al::isPadReleaseR(port);
    return al::isPadReleaseZL(port) || al::isPadReleaseZR(port);
}

bool PlayerInputFunction::isHoldBalloonSet(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        if (al::isPadHoldL(port))
            return al::isPadHoldR(port);
    } else if (al::isPadHoldZL(port)) {
        return al::isPadHoldZR(port);
    }
    return false;
}

bool PlayerInputFunction::isTriggerTalk(const al::LiveActor* actor, s32 port) {
    return al::isPadTriggerA(port);
}

bool PlayerInputFunction::isTriggerStartWorldWarp(const al::LiveActor* actor, s32 port) {
    return al::isPadTriggerA(port);
}

bool PlayerInputFunction::isTriggerCancelWorldWarp(const al::LiveActor* actor, s32 port) {
    if (al::isPadTriggerB(port))
        return true;
    if (al::isPadTriggerY(port))
        return true;
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return false;
    return al::isPadTriggerX(port);
}

sead::Vector2f PlayerInputFunction::getMoveInputStick(const al::LiveActor* actor, s32 port,
                                                      s32 isCameraMove) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
            bool isHoldX = al::isPadHoldX(port);
            if (isCameraMove != 0)
                return al::getLeftStick(port);
            if (!isHoldX)
                return al::getLeftStick(port);
            return sead::Vector2f::zero;
        } else if (al::isPadHoldL(port) || al::isPadHoldR(port)) {
            return sead::Vector2f::zero;
        }
    }
    return al::getLeftStick(port);
}

bool PlayerInputFunction::isHoldCameraReset(const al::LiveActor* actor, s32 port,
                                            s32 isCameraMove) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        bool isHoldX = al::isPadHoldX(port);
        return (isCameraMove == 0) & isHoldX;
    }
    return al::isPadHoldL(port) || al::isPadHoldR(port);
}

f32 PlayerInputFunction::getRadiconInputSteeringValue(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        if (al::isPadHoldY(port))
            return -1.0f;
        return al::isPadHoldA(port) ? 1.0f : 0.0f;
    }
    return al::getRightStick(port).x;
}

s32 PlayerInputFunction::getNoInputJudgeKeyMask(const al::LiveActor* actor, s32 port) {
    s32 keyMask = cNoInputJudgeKeyMaskDefault;
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        keyMask = cNoInputJudgeKeyMaskSingleJoy;
    return keyMask;
}

bool PlayerInputFunction::isInputLeftStickNoCameraMove(const al::LiveActor* actor, s32 port,
                                                       f32 threshold) {
    const sead::Vector2f& leftStick = al::getLeftStick(port);
    if (leftStick.length() < threshold)
        return false;

    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        if (al::isPadHoldX(port))
            return false;
    }
    return true;
}

bool PlayerInputFunction::isTriggerCameraReset(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return al::isPadTriggerX(port);
    return al::isPadTriggerL(port) || al::isPadTriggerR(port);
}

bool PlayerInputFunction::isTriggerSeparateCameraReset(const al::LiveActor* actor, s32 port,
                                                       s32 isCameraMove) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        if (isCameraMove)
            return al::isPadReleaseX(port);
        return false;
    }
    return al::isPadTriggerL(port) || al::isPadTriggerR(port);
}

bool PlayerInputFunction::isTriggerCameraSubjective(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return false;
    return al::isPadTriggerPressRightStick(port);
}

bool PlayerInputFunction::isHoldCameraZoom(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return false;
    return al::isPadHoldZL(port) || al::isPadHoldZR(port);
}

bool PlayerInputFunction::isHoldCameraSnapShotZoomIn(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return false;
    return al::isPadHoldX(port);
}

bool PlayerInputFunction::isHoldCameraSnapShotZoomOut(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return false;
    return al::isPadHoldA(port);
}

bool PlayerInputFunction::isHoldCameraSnapShotRollLeft(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return false;
    return al::isPadHoldZL(port);
}

bool PlayerInputFunction::isHoldCameraSnapShotRollRight(const al::LiveActor* actor, s32 port) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port))
        return false;
    return al::isPadHoldZR(port);
}

sead::Vector2f PlayerInputFunction::getCameraMoveInput(const al::LiveActor* actor, s32 port,
                                                       s32 isCameraMove,
                                                       bool isValidPressLeftStick) {
    if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
        if (rs::isSeparatePlay(actor) && al::isPadTypeJoySingle(port)) {
            s32 isHoldX = al::isPadHoldX(port);
            if (isCameraMove == 0) {
                if (isHoldX)
                    return al::getLeftStick(port);
            }
        } else if (al::isPadHoldL(port) || al::isPadHoldR(port)) {
            return al::getLeftStick(port);
        }

        if (isValidPressLeftStick && al::isPadHoldPressLeftStick(port)) {
            const sead::Vector2f& input = al::getLeftStick(port);
            return sead::Vector2f(input.x, 0);
        }
        return sead::Vector2f::zero;
    }
    return al::getRightStick(port);
}
