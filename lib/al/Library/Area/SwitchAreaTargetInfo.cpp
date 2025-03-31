#include "Library/Area/SwitchAreaTargetInfo.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace al {
SwitchAreaTargetInfo::SwitchAreaTargetInfo(s32 _0_10Count, s32 _20Count)
    : _c(_0_10Count), _1c(_0_10Count), _2c(_20Count) {
    _0 = new sead::Vector3f[_c];
    _10 = new sead::Vector3f[_1c];
    _20 = new sead::Vector3f[_2c];
}

SwitchAreaTargetInfo::SwitchAreaTargetInfo(sead::Vector3f* _0_10, s32 _0_10Count)
    : _0(_0_10), _8(_0_10Count), _c(_0_10Count), _10(_0_10), _18(_0_10Count), _1c(_0_10Count) {}

void SwitchAreaTargetInfo::update(const PlayerHolder* playerHolder,
                                  const SceneCameraInfo* sceneCameraInfo) {
    _8 = 0;
    _18 = 0;

    s32 playerNumMax = getPlayerNumMax(playerHolder);
    for (s32 i = 0; i < playerNumMax; i++) {
        const sead::Vector3f& playerPos = getPlayerPos(playerHolder, i);

        _10[_18] = playerPos;
        _18++;

        if (!isPlayerDead(playerHolder, i) && isPlayerAreaTarget(playerHolder, i)) {
            _0[_8] = playerPos;
            _8++;
        }
    }

    _20[0] = getCameraAt(sceneCameraInfo, 0);
    _28 = 1;
}
}  // namespace al
