#pragma once

#include "Library/Camera/CameraOffsetCtrl.h"

namespace al {
class CameraOffsetCtrlPreset : public CameraOffsetCtrl {
public:
    CameraOffsetCtrlPreset();

    void load(const ByamlIter& iter) override;
    f32 getOffset() const override;
};
}  // namespace al
