#include "Library/Area/SwitchAreaTargetInfo.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace al {
SwitchAreaTargetInfo::SwitchAreaTargetInfo(s32 playersPositionCount,
                                           s32 cameraLookAtPositionMaxCount)
    : mPlayersTargetPositionMaxCount(playersPositionCount),
      mPlayersPositionMaxCount(playersPositionCount),
      mCameraLookAtPositionMaxCount(cameraLookAtPositionMaxCount) {
    mPlayersTargetPositions = new sead::Vector3f[mPlayersTargetPositionMaxCount];
    mPlayersPositions = new sead::Vector3f[mPlayersPositionMaxCount];
    mCameraLookAtPositions = new sead::Vector3f[mCameraLookAtPositionMaxCount];
}

SwitchAreaTargetInfo::SwitchAreaTargetInfo(sead::Vector3f* mPlayersPositions,
                                           s32 playersPositionCount)
    : mPlayersTargetPositions(mPlayersPositions), mPlayersTargetPositionCount(playersPositionCount),
      mPlayersTargetPositionMaxCount(playersPositionCount), mPlayersPositions(mPlayersPositions),
      mPlayersPositionCount(playersPositionCount), mPlayersPositionMaxCount(playersPositionCount) {}

void SwitchAreaTargetInfo::update(const PlayerHolder* playerHolder,
                                  const SceneCameraInfo* sceneCameraInfo) {
    mPlayersTargetPositionCount = 0;
    mPlayersPositionCount = 0;

    s32 playerNumMax = getPlayerNumMax(playerHolder);
    for (s32 i = 0; i < playerNumMax; i++) {
        const sead::Vector3f& playerPos = getPlayerPos(playerHolder, i);

        mPlayersPositions[mPlayersPositionCount] = playerPos;
        mPlayersPositionCount++;

        if (!isPlayerDead(playerHolder, i) && isPlayerAreaTarget(playerHolder, i)) {
            mPlayersTargetPositions[mPlayersTargetPositionCount] = playerPos;
            mPlayersTargetPositionCount++;
        }
    }

    mCameraLookAtPositions[0] = getCameraAt(sceneCameraInfo, 0);
    mCameraLookAtPositionCount = 1;
}
}  // namespace al
