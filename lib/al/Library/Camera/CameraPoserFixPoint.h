#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class LiveActor;

class CameraPoserFixPoint : public CameraPoser {
public:
    CameraPoserFixPoint(const LiveActor* actor);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;
    void makeLookAtCamera(sead::LookAtCamera* lookAtCam) const override;

private:
    f32 mOffsetY;
    sead::Vector3f mCameraPos;
    bool mIsUsePrePoserPos;
    bool mIsKeepDistanceFromLookAt;
    f32 mKeepDistance;
};

static_assert(sizeof(CameraPoserFixPoint) == 0x158);

}  // namespace al
