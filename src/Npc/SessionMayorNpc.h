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

    void init(const al::ActorInitInfo& initInfo) override;

    void movement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    void initIntroductionCamera(const al::ActorInitInfo& initInfo,
                                sead::PtrArray<SessionMusicianNpc>* musicians);

    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;

    bool tryAppearMemberMusicians();

    void exeWait();
    void exeReaction();

private:
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    sead::PtrArray<SessionMusicianNpc> mMusicianArray;
    SessionMayorParam* mMayorParam = nullptr;
    TalkNpcParam* mTalkNpcParam = nullptr;
    al::PlacementId* mPlacementId = nullptr;
    NpcStateReaction* mNpcStateReaction = nullptr;
    NpcJointLookAtController* mNpcJointLookAtController = nullptr;
    al::JointSpringControllerHolder* mJointSpringControllerHolder = nullptr;
    al::AddDemoInfo* mDemoInfo = nullptr;
    TalkNpcActionAnimInfo* mTalkNpcActionAnimInfo = nullptr;
    RandomWaitActionUpdater* mRandomWaitActionUpdater = nullptr;
};

static_assert(sizeof(SessionMayorNpc) == 0x170);
