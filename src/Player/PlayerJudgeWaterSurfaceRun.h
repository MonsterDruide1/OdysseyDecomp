#pragma once

#include <basis/seadTypes.h>

#include "Player/IJudge.h"

namespace al {
class LiveActor;
class WaterSurfaceFinder;
}  // namespace al
class PlayerConst;
class PlayerCounterForceRun;

class PlayerJudgeWaterSurfaceRun : public IJudge {
public:
    PlayerJudgeWaterSurfaceRun(const al::LiveActor* player, const PlayerConst* pConst,
                               const al::WaterSurfaceFinder* waterSurfaceFinder,
                               const PlayerCounterForceRun* counterForceRun);

    f32 getBorderSpeedH() const;

    void update() override;
    void reset() override;
    bool judge() const override;

private:
    bool mIsWaterSurfaceRun = false;
    bool mIsEnable = false;
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const al::WaterSurfaceFinder* mWaterSurfaceFinder;
    const PlayerCounterForceRun* mCounterForceRun;
};
