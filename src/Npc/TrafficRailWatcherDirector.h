#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Scene/ISceneObj.h"

class TrafficRailWatcher;

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class TrafficRailWatcherDirector : public al::HioNode, public al::ISceneObj {
public:
    TrafficRailWatcherDirector();

    void* registerActor(const al::LiveActor* actor, const al::ActorInitInfo& info);
    TrafficRailWatcher* findActorRailWatcher(const al::LiveActor* actor) const;
    const char* getSceneObjName() const override;

private:
    s32 mWatcherCount = 0;
    TrafficRailWatcher** mWatchers = nullptr;
};

static_assert(sizeof(TrafficRailWatcherDirector) == 0x18);

namespace rs {
void* registerTrafficRailWatcher(const al::LiveActor* actor, const al::ActorInitInfo& info);
void stopTrafficRailByTraffic(const al::LiveActor* actor);
void restartTrafficRailByTraffic(const al::LiveActor* actor);
bool tryStopTrafficRailByOtherNpc(const al::LiveActor* actor);
bool tryRestartTrafficRailByOtherNpc(const al::LiveActor* actor);
}  // namespace rs
