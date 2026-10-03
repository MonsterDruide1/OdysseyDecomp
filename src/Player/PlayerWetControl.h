#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}
class PlayerAreaChecker;
class PlayerEffect;

class PlayerWetControl {
public:
    PlayerWetControl(const al::LiveActor* player, al::LiveActor* model,
                     const PlayerAreaChecker* areaChecker);

    bool isWet() const;
    void reset();
    void updateModelRoughness(f32 roughness);
    void recordInWater();
    void recordWet();
    void recordWaterSurface();
    void recordHeavyLandPuddle();
    void recordPuddleRolling();
    void recordForestWaterFall();
    void recordWaterSplash();
    void recordWetBySensor();
    void update();

private:
    const al::LiveActor* mPlayer;
    PlayerEffect* mEffect = nullptr;
    al::LiveActor* mModel;
    const PlayerAreaChecker* mAreaChecker;
    bool mIsInWetArea = false;
    s32 mInWaterCounter = -1;
    s32 mWetCounter = -1;
    f32 mRoughness = 1.0f;
    bool mIsWaterSurface = false;
};

static_assert(sizeof(PlayerWetControl) == 0x38);
