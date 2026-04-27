#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserQuickTurn : public CameraPoser {
public:
    CameraPoserQuickTurn(const char* name);

    void setFollow();

    void setRotateFast() { mIsRotateFast = true; }

private:
    u8 _140[0x31];
    bool mIsRotateFast = false;
    u8 _172[0x6];
};

static_assert(sizeof(CameraPoserQuickTurn) == 0x178);

}  // namespace al
