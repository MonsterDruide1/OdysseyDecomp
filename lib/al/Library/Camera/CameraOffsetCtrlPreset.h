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
<<<<<<< HEAD

private:
    CameraOffsetPreset* mPreset = nullptr;
=======
>>>>>>> 4474465b (Stashing)
};

static_assert(sizeof(CameraOffsetCtrlPreset) == 0x10);

class CameraOffsetCtrlY : public CameraOffsetCtrl {
public:
    void load(const ByamlIter& iter) override;

    const sead::Vector3f& getOffset() const override { return mOffset; }

private:
    sead::Vector3f mOffset = sead::Vector3f::zero;
};

}  // namespace al
