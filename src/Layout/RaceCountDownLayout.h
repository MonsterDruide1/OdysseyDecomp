#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class RaceCountDownLayout : public al::LayoutActor {
public:
    RaceCountDownLayout(const al::LayoutInitInfo& info);

    void startCountDown(s32 count);
    void startFinish();
    bool isEndCountDown() const;
    bool isEndFinish() const;
    void reset();

    void exeWait();
    void exeCountDown();
    void exeCountDownEnd();
    void exeFinish();
    void exeFinishEnd();

private:
    al::LayoutActor* mCountLayout = nullptr;
    s32 mCount = 0;
};
