#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class Indicator : public al::LiveActor {
public:
    Indicator(const char* name);

    void init(const al::ActorInitInfo& info) override;

    void update();
    void setLightLevel(s32 lightLevel);
    void setLightLevel(s32 lightLevel, s32 duration);
    void tryDecreaseLightLevel(s32 minLightLevel);
    void exeWait();

private:
    s32 mLightLevel = 0;
    s32 mIgnoreLevel = 0;
    f32 mLightLevelStep = 0.0f;
    f32 mLightLevelFloat = 0.0f;
    s32 mTargetLightLevel = 0;
};

static_assert(sizeof(Indicator) == 0x120);
