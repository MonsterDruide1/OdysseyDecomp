#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class LiveActor;
}  // namespace al

class PlayGuideCamera : public al::LayoutActor {
public:
    PlayGuideCamera(const char* name, const al::LayoutInitInfo& info, const al::LiveActor* player);

    void hide();
    void start();

    void exeHide();
    bool tryAppear();
    void exeAppear();
    void exeWait();
    void exeEnd();

private:
    bool mIsShown = false;
    const al::LiveActor* mPlayer = nullptr;
};
