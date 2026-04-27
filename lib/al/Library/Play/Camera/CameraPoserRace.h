#pragma once

#include <math/seadVector.h>

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserRace : public CameraPoser {
public:
    CameraPoserRace(const char* name);

    void setFrontDirPtr(const sead::Vector3f* frontDirPtr) { mFrontDirPtr = frontDirPtr; }

    void setOffsetY(f32 offsetY) { mOffsetY = offsetY; }

    void setDistance(f32 distance) { mDistance = distance; }

    void setAngleDegreeV(f32 angleV) { mAngleDegreeV = angleV; }

private:
    const sead::Vector3f* mFrontDirPtr = nullptr;
    u8 _148[0x10];
    f32 mOffsetY = 0.0f;
    f32 mDistance = 0.0f;
    f32 mAngleDegreeV = 0.0f;
    u8 _164[0xC];
};

static_assert(sizeof(CameraPoserRace) == 0x170);

}  // namespace al
