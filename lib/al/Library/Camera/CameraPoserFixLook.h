#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class LiveActor;

class CameraPoserFixLook : public CameraPoser {
public:
    CameraPoserFixLook(const LiveActor* actor);

    void init() override;
    void start(const CameraStartInfo& startInfo) override;

    sead::Vector3f* get_140() { return _140; }

private:
    sead::Vector3f* _140;
};

static_assert(sizeof(CameraPoserFixLook) == 0x148);

}  // namespace al
