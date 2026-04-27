#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserFixPoint : public CameraPoser {
public:
    CameraPoserFixPoint(const char* name);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;
    void makeLookAtCamera(sead::LookAtCamera* lookAtCam) const override;

    void validateUsePreCameraPos() { mIsUsePreCameraPos = true; }

private:
    f32 mOffsetY;
    sead::Vector3f mCameraPos;
    bool mIsUsePreCameraPos;
    bool mIsKeepDistanceFromLookAt;
    f32 mKeepDistance;
};

static_assert(sizeof(CameraPoserFixPoint) == 0x158);

}  // namespace al
