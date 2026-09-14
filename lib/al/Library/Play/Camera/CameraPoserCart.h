#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserCart : public CameraPoser {
public:
    CameraPoserCart(const char* name);

    void stop();
    void restart();

private:
    u8 _140[0x10];
};

static_assert(sizeof(CameraPoserCart) == 0x150);

}  // namespace al
