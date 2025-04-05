#include "Library/Area/SwitchAreaTargetInfo.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace al {
SwitchAreaTargetInfo::SwitchAreaTargetInfo(s32 playerPositionCount, s32 cameraLookAtPositionSize)
    : mPlayerTargetPositionSize(playerPositionCount), mPlayerPositionSize(playerPositionCount),
      mCameraLookAtPositionSize(cameraLookAtPositionSize) {
    mPlayerTargetPositions = new sead::Vector3f[mPlayerTargetPositionSize];
    mPlayerPositions = new sead::Vector3f[mPlayerPositionSize];
    mCameraLookAtPositions = new sead::Vector3f[mCameraLookAtPositionSize];
}

SwitchAreaTargetInfo::SwitchAreaTargetInfo(sead::Vector3f* playerPositions, s32 playerPositionCount)
    : mPlayerTargetPositions(playerPositions), mPlayerTargetPositionCount(playerPositionCount),
      mPlayerTargetPositionSize(playerPositionCount), mPlayerPositions(playerPositions),
      mPlayerPositionCount(playerPositionCount), mPlayerPositionSize(playerPositionCount) {}

void SwitchAreaTargetInfo::update(const PlayerHolder* playerHolder,
                                  const SceneCameraInfo* sceneCameraInfo) {
    mPlayerTargetPositionCount = 0;
    mPlayerPositionCount = 0;

    s32 playerNumMax = getPlayerNumMax(playerHolder);
    for (s32 i = 0; i < playerNumMax; i++) {
        const sead::Vector3f& playerPos = getPlayerPos(playerHolder, i);

        mPlayerPositions[mPlayerPositionCount] = playerPos;
        mPlayerPositionCount++;

        if (!isPlayerDead(playerHolder, i) && isPlayerAreaTarget(playerHolder, i)) {
            mPlayerTargetPositions[mPlayerTargetPositionCount] = playerPos;
            mPlayerTargetPositionCount++;
        }
    }

    mCameraLookAtPositions[0] = getCameraAt(sceneCameraInfo, 0);
    mCameraLookAtPositionCount = 1;
}
}  // namespace al
