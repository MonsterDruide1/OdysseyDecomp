#pragma once

#include <basis/seadTypes.h>

namespace al {

class PauseCameraCtrl {
public:
    PauseCameraCtrl(f32 v);

    void startCameraPause() { mIsCameraPause = true; }

    void endCameraPause() { mIsCameraPause = false; }

    bool isCameraPause() const { return mIsCameraPause; }

    f32 get_4() const { return _4; }

private:
    bool mIsCameraPause = false;
    f32 _4;
};

static_assert(sizeof(PauseCameraCtrl) == 0x8);

}  // namespace al
