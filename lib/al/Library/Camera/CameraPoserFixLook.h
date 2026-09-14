#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class LiveActor;

class CameraPoserFixLook : public CameraPoser {
public:
    CameraPoserFixLook();

    void init() override;
    void start(const CameraStartInfo& startInfo) override;

    void setTransPtr(sead::Vector3f* trans) { mTrans = trans; }

private:
    sead::Vector3f* mTrans;
};

static_assert(sizeof(CameraPoserFixLook) == 0x148);

}  // namespace al
