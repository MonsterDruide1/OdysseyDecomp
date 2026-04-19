#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserFixPoint : public CameraPoser {
public:
    CameraPoserFixPoint(const char* name);

    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void update() override;
    void makeLookAtCamera(sead::LookAtCamera* cam) const override;

private:
    char _140[24];
};

static_assert(sizeof(CameraPoserFixPoint) == 0x158);

}  // namespace al
