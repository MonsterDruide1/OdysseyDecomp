#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

struct KeyMoveCameraInfo {
    KeyMoveCameraInfo();
};

class KeyMoveCameraBase : public CameraPoser {
public:
    KeyMoveCameraBase(const char* name);

    void load(const ByamlIter&) override;
    void start(const CameraStartInfo&) override;
    void movement() override;
    virtual bool isEndKeyMoveCamera() const;
    f32 calcRate() const;

private:
    s8 filler[0x168 - sizeof(CameraPoser)];
};

}  // namespace al
