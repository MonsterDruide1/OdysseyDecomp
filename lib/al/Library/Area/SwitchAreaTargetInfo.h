#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class PlayerHolder;
class SceneCameraInfo;

class SwitchAreaTargetInfo {
public:
    /**
     * @info mCameraLookAtPositions will be set to an array of cameraLookAtPositionMaxCount
     * sead::Vector3f but only one is used/updated.
     */
    SwitchAreaTargetInfo(s32 playersPositionCount, s32 cameraLookAtPositionMaxCount);
    /**
     * @warning Using this ctor will cause a crash when calling update because
     * mCameraLookAtPositions is nullptr by default.
     */
    SwitchAreaTargetInfo(sead::Vector3f* mPlayersPositions, s32 playersPositionCount);

    void update(const PlayerHolder* playerHolder, const SceneCameraInfo* sceneCameraInfo);

    const sead::Vector3f* getPlayersTargetPositions() const { return mPlayersTargetPositions; }

    s32 getPlayersTargetPositionCount() const { return mPlayersTargetPositionCount; }

private:
    sead::Vector3f* mPlayersTargetPositions = nullptr;
    s32 mPlayersTargetPositionCount = 0;
    s32 mPlayersTargetPositionMaxCount = 0;
    sead::Vector3f* mPlayersPositions = nullptr;
    s32 mPlayersPositionCount = 0;
    s32 mPlayersPositionMaxCount = 0;
    sead::Vector3f* mCameraLookAtPositions = nullptr;
    s32 mCameraLookAtPositionCount = 0;
    s32 mCameraLookAtPositionMaxCount = 0;
};

static_assert(sizeof(SwitchAreaTargetInfo) == 0x30);
}  // namespace al
