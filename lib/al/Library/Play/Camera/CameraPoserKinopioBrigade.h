#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserKinopioBrigade : public CameraPoser {
public:
    class V3bool;

    CameraPoserKinopioBrigade(const char* name);

    void start(const CameraStartInfo& info) override;
    void resetValues(f32, f32);
    void update() override;
    void calcDrcAngle();
    void calcBaseAngle();
    void convergeBaseAngle();
    void commitCamera();
    void followLookAt(f32, const sead::Vector3f&, const sead::Vector3f&, const V3bool&);
    void loadParam(const ByamlIter& iter) override;
    void transferParam(const CameraPoserKinopioBrigade*);
    bool isEnableRotateByPad() const override;

private:
    s8 filler[0x1F8 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserKinopioBrigade) == 0x1F8, "al::CameraPoserKinopioBrigade size");
