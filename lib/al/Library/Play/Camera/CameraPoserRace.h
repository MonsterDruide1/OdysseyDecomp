#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserRace : public CameraPoser {
public:
    CameraPoserRace(const char* name);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void calcTargetFrontLocal(sead::Vector3f*, bool) const;
    void update() override;

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

}  // namespace al

static_assert(sizeof(al::CameraPoserRace) == 0x170, "al::CameraPoserRace size");
