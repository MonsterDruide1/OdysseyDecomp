#include "Library/Screen/ScreenCoverCtrl.h"

namespace al {
ScreenCoverCtrl::ScreenCoverCtrl() {}

void ScreenCoverCtrl::requestCaptureScreenCover(s32 coverForFrames) {
    if (mCoverFrames < coverForFrames) {
        if (mCoverFrames <= 0)
            mIsActive = true;
        mCoverFrames = coverForFrames;
    }
}

void ScreenCoverCtrl::update() {
    if (mCoverFrames >= 1) {
        mCoverFrames--;
        if (mCoverFrames == 0)
            mIsActive = false;
    }
}
}  // namespace al
