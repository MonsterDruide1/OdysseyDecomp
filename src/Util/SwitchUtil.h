#pragma once

namespace al {
class LiveActor;
struct ActorInitInfo;
}  // namespace al

namespace rs {
bool isOnSwitchLinkSave(const al::LiveActor* actor, const al::ActorInitInfo& actorInitInfo);
bool isSaveSwitch(const al::ActorInitInfo& actorInitInfo);
}  // namespace rs
