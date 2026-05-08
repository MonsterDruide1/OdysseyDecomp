#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class HitSensor;
class SensorMsg;
class SubActorLodExecutor;
struct ActorInitInfo;

void registSupportFreezeSyncGroup(LiveActor* actor, const ActorInitInfo& info);
}  // namespace al

class SubActorLodFixPartsScenarioAction : public al::LiveActor {
public:
    SubActorLodFixPartsScenarioAction(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void movement() override;
    void calcAnim() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void listenStartSnapShotMode();
    void listenEndSnapShotMode();

private:
    al::SubActorLodExecutor* mSubActorLodExecutor = nullptr;
    bool mIsActionStarted = false;
};

static_assert(sizeof(SubActorLodFixPartsScenarioAction) == 0x118);
