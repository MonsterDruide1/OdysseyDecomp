#pragma once

#include <basis/seadTypes.h>

namespace al {
class ScreenCoverCtrl {
public:
    ScreenCoverCtrl();

    void update();
    void reqeustCaptureScreenCover(s32 coverFrames);

private:
    s32 mCoverFrames = -1;
    bool mIsActive = false;
};

static_assert(sizeof(ScreenCoverCtrl) == 0x8);
}  // namespace al
