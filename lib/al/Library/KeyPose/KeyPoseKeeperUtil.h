#pragma once

#include <math/seadQuat.h>

namespace al {
struct ActorInitInfo;
class KeyPoseKeeper;
class LiveActor;
class PlacementInfo;

KeyPoseKeeper* createKeyPoseKeeper(const ActorInitInfo& info);
KeyPoseKeeper* createKeyPoseKeeper(const PlacementInfo& info);
void resetKeyPose(KeyPoseKeeper* keyPoseKeeper);
void nextKeyPose(KeyPoseKeeper* keyPoseKeeper);
void restartKeyPose(KeyPoseKeeper* keyPoseKeeper, sead::Vector3f* pos, sead::Quatf* orientation);
void reverseKeyPose(KeyPoseKeeper* keyPoseKeeper);
const sead::Vector3f& getCurrentKeyTrans(const KeyPoseKeeper* keyPoseKeeper);
const sead::Vector3f& getNextKeyTrans(const KeyPoseKeeper* keyPoseKeeper);
const sead::Quatf& getCurrentKeyQuat(const KeyPoseKeeper* keyPoseKeeper);
const sead::Quatf& getNextKeyQuat(const KeyPoseKeeper* keyPoseKeeper);
const PlacementInfo& getCurrentKeyPlacementInfo(const KeyPoseKeeper* keyPoseKeeper);
const PlacementInfo& getNextKeyPlacementInfo(const KeyPoseKeeper* keyPoseKeeper);
s32 getKeyPoseCount(const KeyPoseKeeper* keyPoseKeeper);
void getKeyPoseTrans(sead::Vector3f* out, const KeyPoseKeeper* keyPoseKeeper, s32 idx);
void getKeyPoseQuat(sead::Quatf* out, const KeyPoseKeeper* keyPoseKeeper, s32 idx);
void calcLerpKeyTrans(sead::Vector3f* out, const KeyPoseKeeper* keyPoseKeeper, f32 rate);
void calcSlerpKeyQuat(sead::Quatf* out, const KeyPoseKeeper* keyPoseKeeper, f32 rate);
bool isMoveSignKey(const KeyPoseKeeper* keyPoseKeeper);
bool isLastKey(const KeyPoseKeeper* keyPoseKeeper);
bool isFirstKey(const KeyPoseKeeper* keyPoseKeeper);
bool isGoingToEnd(const KeyPoseKeeper* keyPoseKeeper);
bool isStop(const KeyPoseKeeper* keyPoseKeeper);
bool isRestart(const KeyPoseKeeper* keyPoseKeeper);
f32 calcDistanceNextKeyTrans(const KeyPoseKeeper* keyPoseKeeper);
s32 calcTimeToNextKeyMove(const KeyPoseKeeper* keyPoseKeeper, f32 speed);
void calcDirToNextKey(sead::Vector3f* out, const KeyPoseKeeper* keyPoseKeeper);
f32 calcKeyMoveSpeed(const KeyPoseKeeper* keyPoseKeeper);
f32 calcKeyMoveSpeedByTime(const KeyPoseKeeper* keyPoseKeeper);
s32 calcKeyMoveWaitTime(const KeyPoseKeeper* keyPoseKeeper);
s32 calcKeyMoveMoveTime(const KeyPoseKeeper* keyPoseKeeper);
void calcKeyMoveClippingInfo(sead::Vector3f*, f32*, const KeyPoseKeeper*, f32);
void setKeyMoveClippingInfo(LiveActor*, sead::Vector3f*, const KeyPoseKeeper*);
}  // namespace al
