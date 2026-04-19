#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserRace : public CameraPoser {
public:
    CameraPoserRace(const char* name);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void calcTargetFrontLocal(sead::Vector3f*, bool) const;
    void update() override;

private:
    s8 filler[0x170 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserRace) == 0x170, "al::CameraPoserRace size");
