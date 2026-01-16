#pragma once

#include <basis/seadTypes.h>

namespace al {
class ScreenCoverCtrl {
public:
    ScreenCoverCtrl();

    void update();
    void requestCaptureScreenCover(s32 coverFrames);

    s32 getCoverFrames() const { return mCoverFrames; }

    void resetCapture() { mCoverFrames = -1; }

    bool isRequestCapture() const { return mIsRequestCapture; }

    void resetRequestCapture() { mIsRequestCapture = false; }

private:
    s32 mCoverFrames = -1;
    bool mIsRequestCapture = false;
};

static_assert(sizeof(ScreenCoverCtrl) == 0x8);
}  // namespace al
