#pragma once

#include <math/seadVector.h>

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserKinopioBrigade : public CameraPoser {
public:
    struct V3bool {
        bool x;
        bool y;
        bool z;
    };

    CameraPoserKinopioBrigade(const char* name);

    void start(const CameraStartInfo& startInfo) override;
    void resetValues(f32 distance, f32 angleDegreeV);
    void update() override;
    void calcDrcAngle();
    void calcBaseAngle();
    void convergeBaseAngle();
    f32 commitCamera();
    f32 followLookAt(f32 rate, const sead::Vector3f& lookAt, const sead::Vector3f& offset,
                     const V3bool& isFollowAxis);
    void loadParam(const ByamlIter& iter) override;
    void transferParam(const CameraPoserKinopioBrigade* other);
    bool isEnableRotateByPad() const override;

private:
    u8 _140[0xB8];
};

static_assert(sizeof(CameraPoserKinopioBrigade) == 0x1F8);

}  // namespace al
