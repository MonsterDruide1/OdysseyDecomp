#pragma once

#include "Library/Placement/PlacementId.h"

namespace al {
struct ActorInitInfo;
class EventFlowDataHolder;
class EventFlowNodeFactory;
class LiveActor;
class MessageSystem;
class SceneEventFlowMsg;

class EventFlowChartInitInfo {
public:
    EventFlowChartInitInfo(LiveActor* actor, EventFlowDataHolder* dataHolder,
                           const ActorInitInfo& actorInitInfo, const char* eventFileName,
                           const char* eventObjName, const char* eventFlowName,
                           const EventFlowNodeFactory& nodeFactory, SceneEventFlowMsg* sceneMsg,
                           const char* stageName);

private:
    LiveActor* mActor;
    EventFlowDataHolder* mDataHolder;
    const ActorInitInfo* mActorInitInfo;
    const char* mEventFileName;
    const char* mEventObjName;
    const char* mEventFlowName;
    PlacementId mPlacementId;
    const EventFlowNodeFactory* mNodeFactory;
    SceneEventFlowMsg* mSceneMsg;
    const MessageSystem* mMessageSystem;
    const char* mStageName;
};

static_assert(sizeof(EventFlowChartInitInfo) == 0x70);
}  // namespace al
