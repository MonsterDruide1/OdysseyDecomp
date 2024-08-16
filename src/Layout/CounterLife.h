#pragma once

#include <basis/seadTypes.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class CounterLife : public al::LayoutActor {
public:
    CounterLife(const char* name, const char* resName, const al::LayoutInitInfo& info);

    void appear() override;
    void kill() override;

    void startGauge(f32 goalFrame);
    void startGaugeWithFrame(f32 goalFrame, f32 frames);
    void setEmpty();
    void setGaugeAnim();
    void setCount(f32 frame);

    void start();
    void end();
    void wait();
    bool isWait() const;

    void exeNone();
    void exeAppear();
    void exeWait();
    void exeEnd();
    void exeGauge();
    void exeGaugeWait();

private:
    f32 mCurFrame = 0.0f;
    f32 mGoalFrame = 0.0f;
    f32 mStep = 1.0f;
    bool mIsCounterUp = false;
};
