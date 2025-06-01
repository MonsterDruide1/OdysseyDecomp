#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class Rail;
class RailRider;
}  // namespace al

class RabbitGraphRailKeeper : public al::LiveActor {
public:
    RabbitGraphRailKeeper(const char* name);
    RabbitGraphRailKeeper(const char* name, al::LiveActor* actor);

    void init(const al::ActorInitInfo& initInfo) override;
    void appearBySwitch();
    void killBySwitch();
    bool isRailPointIgnore(s32 index) const;
    bool isRailPointConnectToCollision(s32 index) const;
    void appear() override;
    void kill() override;
    void exeWait();
    al::RailRider* getRailRider() const override;

private:
    al::Rail* mRail = nullptr;
    al::RailRider* mRailRider = nullptr;
    bool mIsJump = false;
};
