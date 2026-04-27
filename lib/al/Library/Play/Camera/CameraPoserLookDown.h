#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserLookDown : public CameraPoser {
public:
    CameraPoserLookDown(const char* name);

    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void update() override;
    void makeLookAtCamera(sead::LookAtCamera* camera) const override;

private:
    u8 _140[0x10];
};

static_assert(sizeof(CameraPoserLookDown) == 0x150);

}  // namespace al
