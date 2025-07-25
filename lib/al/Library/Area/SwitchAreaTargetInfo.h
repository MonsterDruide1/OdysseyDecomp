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
    SwitchAreaTargetInfo(s32 maxPlayerPositions, s32 maxCameraLookAtPositions);
    /**
     * @warning Using this ctor will cause a crash when calling update because
     * mCameraLookAtPositions is nullptr by default.
     */
    SwitchAreaTargetInfo(sead::Vector3f* playerPositions, s32 maxPlayerPositions);

    void update(const PlayerHolder* playerHolder, const SceneCameraInfo* sceneCameraInfo);

    sead::Vector3f* getPlayerTargetPositions() const { return mPlayerTargetPositions; }

    s32 getPlayerTargetPositionCount() const { return mPlayerTargetPositionCount; }

    sead::Vector3f* getCameraLookAtPositions() const { return mCameraLookAtPositions; }

    s32 getCameraLookAtPositionCount() const { return mCameraLookAtPositionCount; }

private:
    sead::Vector3f* mPlayerTargetPositions = nullptr;
    s32 mPlayerTargetPositionCount = 0;
    s32 mPlayerTargetPositionSize = 0;
    sead::Vector3f* mPlayerPositions = nullptr;
    s32 mPlayerPositionCount = 0;
    s32 mPlayerPositionSize = 0;
    sead::Vector3f* mCameraLookAtPositions = nullptr;
    s32 mCameraLookAtPositionCount = 0;
    s32 mCameraLookAtPositionSize = 0;
};

static_assert(sizeof(SwitchAreaTargetInfo) == 0x30);
}  // namespace al
