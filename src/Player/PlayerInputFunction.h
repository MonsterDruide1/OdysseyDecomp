#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class IUseSceneObjHolder;
}  // namespace al

class PlayerInputFunction {
public:
    static bool isSeparatePlaySingleJoy(const al::LiveActor* actor, s32 port);

    static bool isTriggerJump(const al::LiveActor* actor, s32 port);
    static bool isHoldJump(const al::LiveActor* actor, s32 port);
    static bool isReleaseJump(const al::LiveActor* actor, s32 port);

    static bool isTriggerAction(const al::LiveActor* actor, s32 port);
    static bool isHoldAction(const al::LiveActor* actor, s32 port);
    static bool isReleaseAction(const al::LiveActor* actor, s32 port);

    static bool isTriggerSubAction(const al::LiveActor* actor, s32 port);
    static bool isHoldSubAction(const al::LiveActor* actor, s32 port);
    static bool isReleaseSubAction(const al::LiveActor* actor, s32 port);
    static bool isHoldBalloonSet(const al::LiveActor* actor, s32 port);

    static bool isTriggerTalk(const al::LiveActor* actor, s32 port);
    static bool isTriggerStartWorldWarp(const al::LiveActor* actor, s32 port);
    static bool isTriggerCancelWorldWarp(const al::LiveActor* actor, s32 port);

    static sead::Vector2f getMoveInputStick(const al::LiveActor* actor, s32 port, s32 isCameraMove);
    static bool isHoldCameraReset(const al::LiveActor* actor, s32 port, s32 isCameraMove);
    static f32 getRadiconInputSteeringValue(const al::LiveActor* actor, s32 port);
    static s32 getNoInputJudgeKeyMask(const al::LiveActor* actor, s32 port);
    static bool isInputLeftStickNoCameraMove(const al::LiveActor* actor, s32 port, f32 threshold);
    static bool isTriggerCameraReset(const al::LiveActor* actor, s32 port);
    static bool isTriggerSeparateCameraReset(const al::LiveActor* actor, s32 port,
                                             s32 isCameraMove);
    static bool isTriggerCameraSubjective(const al::LiveActor* actor, s32 port);
    static bool isHoldCameraZoom(const al::LiveActor* actor, s32 port);
    static bool isHoldCameraSnapShotZoomIn(const al::LiveActor* actor, s32 port);
    static bool isHoldCameraSnapShotZoomOut(const al::LiveActor* actor, s32 port);
    static bool isHoldCameraSnapShotRollLeft(const al::LiveActor* actor, s32 port);
    static bool isHoldCameraSnapShotRollRight(const al::LiveActor* actor, s32 port);
    static sead::Vector2f getCameraMoveInput(const al::LiveActor* actor, s32 port, s32 isCameraMove,
                                             bool isValidPressLeftStick);
};

namespace rs {
bool isSeparatePlay(const al::IUseSceneObjHolder* holder);
}  // namespace rs
