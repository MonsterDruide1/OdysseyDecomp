#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class IUseSceneObjHolder;
}  // namespace al

namespace PlayerInputFunction {

bool isTriggerAction(const al::LiveActor* actor, s32 port);
bool isHoldAction(const al::LiveActor* actor, s32 port);
bool isReleaseAction(const al::LiveActor* actor, s32 port);

bool isTriggerJump(const al::LiveActor* actor, s32 port);
bool isHoldJump(const al::LiveActor* actor, s32 port);
bool isReleaseJump(const al::LiveActor* actor, s32 port);

bool isTriggerSubAction(const al::LiveActor* actor, s32 port);
bool isHoldSubAction(const al::LiveActor* actor, s32 port);

bool isTriggerTalk(const al::LiveActor* actor, s32 port);
bool isTriggerStartWorldWarp(const al::LiveActor* actor, s32 port);
bool isTriggerCancelWorldWarp(const al::LiveActor* actor, s32 port);

sead::Vector2f getMoveInputStick(const al::LiveActor* actor, s32 port1, s32 port2);

}  // namespace PlayerInputFunction

namespace rs {

bool isSeparatePlay(const al::IUseSceneObjHolder*);

}
