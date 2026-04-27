#pragma once

#include <math/seadVector.h>

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserParallelSimple : public CameraPoser {
public:
    CameraPoserParallelSimple(const char* name);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void update() override;

    void setLookAtOffset(const sead::Vector3f& offset) { mLookAtOffset.set(offset); }

    void setDistance(f32 distance) { mDistance = distance; }

    void setAngleH(f32 angleH) { mAngleH = angleH; }

    void setAngleV(f32 angleV) { mAngleV = angleV; }

private:
    sead::Vector3f mLookAtOffset;
    f32 mDistance;
    f32 mAngleH;
    f32 mAngleV;
};

static_assert(sizeof(CameraPoserParallelSimple) == 0x158);

}  // namespace al
