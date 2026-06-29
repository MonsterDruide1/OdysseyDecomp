#pragma once

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

namespace StageEventDemoFunction {
void initStageEventDemoBase(al::LiveActor* actor, const al::ActorInitInfo& info);
}
