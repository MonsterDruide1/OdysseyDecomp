#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Message/IUseMessageSystem.h"

namespace al {
class EventFlowExecutor;
class FixMapParts;
class JointSpringControllerHolder;
class MessageSystem;
class MessageTagDataHolder;
}  // namespace al

class NpcEventStateScare;
class NpcStateReaction;
class TalkNpcParam;

class Poetter : public al::LiveActor, public al::IUseMessageSystem {
public:
    Poetter(const char* name);

    void init(const al::ActorInitInfo& initInfo) override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    const al::MessageSystem* getMessageSystem() const override;

    void exeWait();
    void exeEvent();
    void exeEventScare();
    void exeReaction();

    void startTalkNoMore();
    void resetYawnWait();

private:
    NpcEventStateScare* mScareState = nullptr;
    NpcStateReaction* mReactionState = nullptr;
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    TalkNpcParam* mTalkParam = nullptr;
    const char16* mHintMessage = u"＜シャイン名＞";
    const al::MessageSystem* mMessageSystem = nullptr;
    al::FixMapParts* mHome = nullptr;
    s32 mHintIndex = 0;
    s32 mCapWatchCount = 0;
    s32 mYawnWait = 0;
    al::JointSpringControllerHolder* mJointSpringHolder = nullptr;
};

static_assert(sizeof(Poetter) == 0x160);
