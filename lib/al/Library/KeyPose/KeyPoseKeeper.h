#pragma once

#include "Library/LiveActor/ActorInitInfo.h"

namespace al {
class KeyPose;

enum MoveType {
    MoveType_Loop,
    MoveType_Turn,
    MoveType_Stop,
    MoveType_Restart,
};

class KeyPoseKeeper {
public:
    KeyPoseKeeper();

    void init(const PlacementInfo& info);
    KeyPose* getKeyPose(s32 idx) const;
    KeyPose* getCurrentKeyPose() const;
    KeyPose* getNextKeyPose() const;
    s32 calcNextPoseIndex() const;
    void reset();
    void next();
    bool isLastKey() const;
    void reverse();
    bool isFirstKey() const;
    void setMoveTypeLoop();
    void setMoveTypeTurn();
    void setMoveTypeStop();
    void setMoveTypeRestart();

private:
    KeyPose** mKeyPoses = nullptr;
    s32 mKeyPoseCount = 0;
    s32 mKeyPoseIdx = 0;
    MoveType mMoveType = MoveType_Loop;
    bool mIsForward = true;
    bool uknBool1 = false;
    bool uknBool2 = false;
};

static_assert(sizeof(KeyPoseKeeper) == 0x18);

KeyPoseKeeper* createKeyPoseKeeper(const ActorInitInfo& info);
KeyPoseKeeper* createKeyPoseKeeper(const PlacementInfo& info);
void resetKeyPose(KeyPoseKeeper* keyPoseKeeper);
void nextKeyPose(KeyPoseKeeper* keyPoseKeeper);
void restartKeyPose(KeyPoseKeeper* keyPoseKeeper, sead::Vector3f* pos, sead::Quatf* orientation);
void reverseKeyPose(KeyPoseKeeper* keyPoseKeeper);
sead::Vector3f* getCurrentKeyTrans(const KeyPoseKeeper* keyPoseKeeper);
sead::Vector3f* getNextKeyTrans(const KeyPoseKeeper* keyPoseKeeper);
sead::Quatf* getCurrentKeyQuat(const KeyPoseKeeper* keyPoseKeeper);
sead::Quatf* getNextKeyQuat(const KeyPoseKeeper* keyPoseKeeper);
PlacementInfo* getCurrentKeyPlacementInfo(const KeyPoseKeeper* keyPoseKeeper);
PlacementInfo* getNextKeyPlacementInfo(const KeyPoseKeeper* keyPoseKeeper);
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
