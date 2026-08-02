#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class ByamlIter;

struct CameraOffsetPresetData {
    const char* name;
    sead::Vector3f offset;
};

static_assert(sizeof(CameraOffsetPresetData) == 0x18);

class CameraOffsetPreset {
public:
    CameraOffsetPreset();
    CameraOffsetPreset(const CameraOffsetPresetData* presetData, s32 presetCount);

    void loadParam(const ByamlIter& iter);

    const sead::Vector3f& getOffset() const { return mPresetData[mCurrentPresetIndex].offset; }

private:
    const CameraOffsetPresetData* mPresetData;
    s32 mPresetCount;
    s32 mCurrentPresetIndex = 0;
};

static_assert(sizeof(CameraOffsetPreset) == 0x10);

}  // namespace al
