#pragma once

#include <math/seadVector.h>

#include "Library/Camera/CameraPoser.h"

class CameraPoserKoopaFireBeamStart : public al::CameraPoser {
public:
    CameraPoserKoopaFireBeamStart(const char* name);

    void prepareStart(const sead::Vector3f& vec1, const sead::Vector3f& vec2);

private:
    char filler_140[0x28];
};

static_assert(sizeof(CameraPoserKoopaFireBeamStart) == 0x168);
