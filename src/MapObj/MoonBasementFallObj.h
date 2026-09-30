#pragma once

#include <math/seadQuat.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class LiveActorGroup;
}  // namespace al

class MoonBasementFallObj : public al::LiveActor {
public:
    MoonBasementFallObj(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void movement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    bool isEnableStartFall() const;
    void exeFall();

private:
    al::LiveActorGroup* mFallObjPieces = nullptr;
    sead::Quatf mStartQuat = sead::Quatf::unit;
    bool mIsOnCalcEffect = false;
    u8 _121[7] = {};
};

namespace MoonBasementFallObjFunction {
void startOnOffCalcEffect(bool* isOnCalcEffect, al::LiveActor* actor);
void updateOnOffCalcEffect(bool* isOnCalcEffect, al::LiveActor* actor);
void forceOnCalcEffect(bool* isOnCalcEffect, al::LiveActor* actor);
}  // namespace MoonBasementFallObjFunction

static_assert(sizeof(MoonBasementFallObj) == 0x128);
