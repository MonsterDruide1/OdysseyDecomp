#pragma once

namespace al {
class ScreenCoverCtrl {
public:
    ScreenCoverCtrl();

    void update();
    void requestCaptureScreenCover(s32 coverFrames);

private:
    s32 mCoverFrames = -1;
    bool mIsCover = false;
};

static_assert(sizeof(ScreenCoverCtrl) == 0x8);
}  // namespace al
