#include "Library/Screen/ScreenCoverCtrl.h"

namespace al {
ScreenCoverCtrl::ScreenCoverCtrl() {}

void ScreenCoverCtrl::requestCaptureScreenCover(s32 coverFrames) {
    if (mCoverFrames < coverFrames) {
        if (mCoverFrames <= 0)
            mIsCover = true;
        mCoverFrames = coverFrames;
    }
}

void ScreenCoverCtrl::update() {
    if (mCoverFrames >= 1) {
        mCoverFrames--;
        if (mCoverFrames == 0)
            mIsCover = false;
    }
}
}  // namespace al
