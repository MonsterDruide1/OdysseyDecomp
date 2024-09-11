#pragma once

namespace al {
class ActorInitInfo;
class AreaObj;
class LiveActor;

bool isInAreaObj(const LiveActor*, const char*);
bool isInDeathArea(const LiveActor*);
bool isInWaterArea(const LiveActor*);

AreaObj* tryFindAreaObj(const LiveActor*, const char*);

void registerAreaHostMtx(LiveActor* actor, const ActorInitInfo& info);
}  // namespace al
