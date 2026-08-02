#include "Library/Camera/CameraOffsetPreset.h"

#include "Library/Base/StringUtil.h"
#include "Library/Yaml/ByamlUtil.h"

namespace al {
const CameraOffsetPresetData gCameraOffsetPresets[18] = {
    {"デフォルト", {0.0, 180.0, 0.0}},     // Default
    {"[テスト]なし", {0.0, 0.0, 0.0}},     // [Test] None
    {"Y0.5m", {0.0, 50.0, 0.0}},           // Y 0.5m
    {"[テスト]Y1m", {0.0, 100.0, 0.0}},    // [Test] Y 1m
    {"[テスト]Y3m", {0.0, 300.0, 0.0}},    // [Test] Y 3m
    {"[テスト]Y4m", {0.0, 400.0, 0.0}},    // [Test] Y 4m
    {"[テスト]Y5m", {0.0, 500.0, 0.0}},    // [Test] Y 5m
    {"[テスト]-Y1m", {0.0, -100.0, 0.0}},  // [Test] -Y1m
    {"[テスト]-Y2m", {0.0, -200.0, 0.0}},  // [Test] -Y2m
    {"[テスト]-Y3m", {0.0, -300.0, 0.0}},  // [Test] -Y3m
    {"[テスト]-Y4m", {0.0, -400.0, 0.0}},  // [Test] -Y4m
    {"[テスト]-Y5m", {0.0, -500.0, 0.0}},  // [Test] -Y5m
    {"Y7m", {0.0, 700.0, 0.0}},            // Y 7m
    {"ゴーレム用", {0.0, 850.0, 0.0}},     // For Golem
    {"クッパ用", {0.0, 360.0, 0.0}},       // For Bowser
    {"崩落クッパ用", {0.0, 450.0, 0.0}},   // For Collapsing Bowser
    {"映像撮影用", {0.0, 210.0, 0.0}},     // For Video Recording
    {"森ボス用", {0.0, 916.0, 0.0}}        // For Forest Boss
};

CameraOffsetPreset::CameraOffsetPreset()
    : mPresetData(gCameraOffsetPresets),
      mPresetCount(sizeof(gCameraOffsetPresets) / sizeof(CameraOffsetPresetData)) {}

CameraOffsetPreset::CameraOffsetPreset(const CameraOffsetPresetData* presetData, s32 presetCount)
    : mPresetData(presetData), mPresetCount(presetCount) {}

void CameraOffsetPreset::loadParam(const ByamlIter& iter) {
    const char* offsetName = tryGetByamlKeyStringOrNULL(iter, "OffsetName");
    if (offsetName == nullptr)
        return;

    mCurrentPresetIndex = -1;

    for (s32 i = 0; i < mPresetCount; i++) {
        if (isEqualString(mPresetData[i].name, offsetName)) {
            mCurrentPresetIndex = i;
            break;
        }
    }
}

}  // namespace al
