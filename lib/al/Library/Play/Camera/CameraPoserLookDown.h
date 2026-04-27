#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserLookDown : public CameraPoser {
public:
    CameraPoserLookDown(const char* name);

    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void update() override;
    void makeLookAtCamera(sead::LookAtCamera* cam) const override;

private:
    s8 filler[0x150 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserLookDown) == 0x150, "al::CameraPoserLookDown size");
