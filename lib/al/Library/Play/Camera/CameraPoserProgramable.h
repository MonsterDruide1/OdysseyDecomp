#pragma once

#include <math/seadVector.h>

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserProgramable : public CameraPoser {
public:
    CameraPoserProgramable();
    CameraPoserProgramable(const sead::Vector3f* pos, const sead::Vector3f* at,
                           const sead::Vector3f* up);

    void update() override;
    void setPose(const sead::Vector3f& pos, const sead::Vector3f& at, const sead::Vector3f& up);

private:
    const sead::Vector3f* mPositionPtr;
    const sead::Vector3f* mLookAtPtr;
    const sead::Vector3f* mUpPtr;
};

class CameraPoserProgramableAngle : public CameraPoser {
public:
    CameraPoserProgramableAngle(const sead::Vector3f* pos, const f32* angleH, const f32* angleV,
                                const f32* distance);

    void update() override;

private:
    const sead::Vector3f* mPositionPtr;
    const f32* mAngleHPtr;
    const f32* mAngleVPtr;
    const f32* mDistancePtr;
};

class CameraPoserProgramableKeepColliderPreCamera : public CameraPoser {
public:
    CameraPoserProgramableKeepColliderPreCamera(const sead::Vector3f* pos, const sead::Vector3f* at,
                                                const sead::Vector3f* up);

    void init() override;
    void start(const CameraStartInfo& startInfo) override;

private:
    const sead::Vector3f* mPositionPtr;
    const sead::Vector3f* mLookAtPtr;
    const sead::Vector3f* mUpPtr;
};

static_assert(sizeof(CameraPoserProgramable) == 0x158);
static_assert(sizeof(CameraPoserProgramableAngle) == 0x160);
static_assert(sizeof(CameraPoserProgramableKeepColliderPreCamera) == 0x158);

}  // namespace al
