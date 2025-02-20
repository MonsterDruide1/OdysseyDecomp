#pragma once

#include "Library/Movement/MoveType.h"

namespace al {
class ActorInitInfo;
class PlacementInfo;
class KeyPose;
class LiveActor;

class KeyPoseKeeper {
public:
    KeyPoseKeeper();

    void init(const PlacementInfo& info);
    const KeyPose& getKeyPose(s32 idx) const;
    const KeyPose& getCurrentKeyPose() const;
    const KeyPose& getNextKeyPose() const;
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

    s32 getKeyPoseCurrentIdx() const { return mKeyPoseCurrentIdx; }

private:
    KeyPose* mKeyPoses = nullptr;
    s32 mKeyPoseCount = 0;
    s32 mKeyPoseCurrentIdx = 0;
    MoveType mMoveType = MoveType::Loop;
    bool mIsForward = true;
    bool mIsStop = false;
    bool mIsRestart = false;
};

static_assert(sizeof(KeyPoseKeeper) == 0x18);
}  // namespace al
