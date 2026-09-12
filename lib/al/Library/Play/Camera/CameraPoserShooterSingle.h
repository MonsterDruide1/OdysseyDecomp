#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserShooterSingle : public CameraPoser {
public:
    CameraPoserShooterSingle(const char* name);

    void loadParam(const ByamlIter& iter) override;
    void update() override;

private:
    u8 _140[0x20];
};

static_assert(sizeof(CameraPoserShooterSingle) == 0x160);

}  // namespace al
