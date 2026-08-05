#pragma once

#include <container/seadPtrArray.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Scene/DemoDirector.h"

namespace al {
class EventFlowExecutor;
class IEventFlowEventReceiver;
class JointSpringControllerHolder;
class PlacementId;
}  // namespace al

struct SessionMayorParam;
class NpcJointLookAtController;
class NpcStateReaction;
class RandomWaitActionUpdater;
class SessionMusicianNpc;
class TalkNpcActionAnimInfo;
class TalkNpcParam;

class SessionMayorNpc : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    SessionMayorNpc(const char* name) : al::LiveActor(name) {}

    void init(const al::ActorInitInfo& initInfo);

    void movement();
    void attackSensor(al::HitSensor* self, al::HitSensor* other);

    void initIntroductionCamera(const al::ActorInitInfo& initInfo,
                                sead::PtrArray<SessionMusicianNpc>* musicians);

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool receiveEvent(const al::EventFlowEventData* event);

    bool tryAppearMemberMusicians();

    void exeWait();
    void exeReaction();

private:
    al::EventFlowExecutor* mEventFlowExecutor;
    sead::PtrArray<SessionMusicianNpc> mMusicians;
    SessionMayorParam* mMayorParam;
    TalkNpcParam* mTalkNpcParam;
    al::PlacementId* mPlacementId;
    NpcStateReaction* mNpcStateReaction;
    NpcJointLookAtController* mNpcJointLookAtController;
    al::JointSpringControllerHolder* mJointSpringControllerHolder;
    al::AddDemoInfo* mDemoInfo;
    TalkNpcActionAnimInfo* mTalkNpcActionAnimInfo;
    RandomWaitActionUpdater* mRandomWaitActionUpdater;
};

static_assert(sizeof(SessionMayorNpc) == 0x170);
