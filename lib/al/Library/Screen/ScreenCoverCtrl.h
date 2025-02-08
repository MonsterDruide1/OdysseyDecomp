#pragma once

#include <basis/seadTypes.h>

namespace al {
class ScreenCoverCtrl {
public:
    ScreenCoverCtrl();
    void requestCaptureScreenCover(s32 coverForFrames);
    void update();

private:
    s32 mCoverFrames = -1;
    bool mIsActive = false;
};
}  // namespace al
