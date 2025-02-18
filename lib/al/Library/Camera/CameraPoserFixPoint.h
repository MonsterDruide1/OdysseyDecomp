#pragma once

#include "Library/Camera/CameraPoserFix.h"

namespace al {

class CameraPoserFixPoint : public CameraPoser {
public:
    CameraPoserFixPoint(const LiveActor* actor);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;
    void makeLookAtCamera(sead::LookAtCamera* lookAtCam) const override;

private:
    char _140[24];
};

static_assert(sizeof(CameraPoserFixPoint) == 0x158);

}  // namespace al
