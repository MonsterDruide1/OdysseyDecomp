#pragma once

#include "Library/Camera/CameraOffsetCtrl.h"

namespace al {
class ByamlIter;
class CameraOffsetPreset;

class CameraOffsetCtrlPreset : public CameraOffsetCtrl {
public:
    CameraOffsetCtrlPreset();

    void load(const ByamlIter& iter) override;
    const sead::Vector3f& getOffset() const override;

private:
    CameraOffsetPreset* mPreset;
};

static_assert(sizeof(CameraOffsetCtrlPreset) == 0x10);
}  // namespace al
