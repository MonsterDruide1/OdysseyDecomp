#pragma once

namespace al {
class ActorInitInfo;
class AreaObj;
class LiveActor;
class SwitchKeepOnAreaGroup;
class SwitchOnAreaGroup;

bool isInAreaObj(const LiveActor*, const char*);
bool isInDeathArea(const LiveActor*);
bool isInWaterArea(const LiveActor*);

AreaObj* tryFindAreaObj(const LiveActor*, const char*);

void registerAreaHostMtx(LiveActor* actor, const ActorInitInfo& info);

SwitchKeepOnAreaGroup* tryCreateSwitchKeepOnAreaGroup(LiveActor* actor, const ActorInitInfo& info);
SwitchOnAreaGroup* tryCreateSwitchOnAreaGroup(LiveActor* actor, const ActorInitInfo& info);
}  // namespace al
