#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserTalk : public CameraPoser {
public:
    CameraPoserTalk(const char* name);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void setMinDistance(f32);

private:
    s8 filler[0x148 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserTalk) == 0x148, "al::CameraPoserTalk size");
