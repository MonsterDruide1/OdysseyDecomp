#pragma once

#include "Player/IJudge.h"

namespace al {
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al
class PlayerCounterForceRun;

class PlayerJudgeStartWaterSurfaceRun : public IJudge {
public:
    PlayerJudgeStartWaterSurfaceRun(const al::LiveActor* player,
                                    const al::WaterSurfaceFinder* waterSurfaceFinder,
                                    const PlayerCounterForceRun* counterForceRun);

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    const al::LiveActor* mPlayer;
    const al::WaterSurfaceFinder* mWaterSurfaceFinder;
    const PlayerCounterForceRun* mCounterForceRun;
};
