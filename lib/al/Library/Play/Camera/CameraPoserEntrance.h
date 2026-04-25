#pragma once

#include <math/seadVector.h>

namespace al {

class CameraPoserEntrance {
public:
    void initParam(f32 distance, const sead::Vector3f& cameraPos, const sead::Vector3f& lookAtPos);
    void initLookAtPosDirect(const sead::Vector3f& lookAtPos);
};

}  // namespace al
