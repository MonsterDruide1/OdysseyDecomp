#pragma once

#include "Project/Poser/KeyMoveCameraBase.h"

namespace al {

class KeyMoveCameraFix : public KeyMoveCameraBase {
public:
    KeyMoveCameraFix(const char* name);

    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo&) override;
    void update() override;

private:
    s8 filler[0x180 - sizeof(KeyMoveCameraBase)];
};

}  // namespace al

static_assert(sizeof(al::KeyMoveCameraFix) == 0x180, "al::KeyMoveCameraFix size");
