#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserFollowSimple : public CameraPoser {
public:
    CameraPoserFollowSimple(const char* name);

    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void reset();
    void update() override;

private:
    u8 _140[0x10];
};

static_assert(sizeof(CameraPoserFollowSimple) == 0x150);

}  // namespace al
