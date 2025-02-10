#pragma once

#include <basis/seadTypes.h>

namespace al {
class ScreenCoverCtrl {
public:
    ScreenCoverCtrl();
    void requestCaptureScreenCover(s32 coverFrames);
    void update();

private:
    s32 mCoverFrames = -1;
    bool mIsCover = false;
};
}  // namespace al
