#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/Event/IEventFlowQueryJudge.h"
#include "Library/LiveActor/LiveActor.h"

class IItemExistanceJudge;
class NpcJointLookAtController;
class NpcStateReaction;
class Shine;
class TalkNpcParam;

namespace al {
class EventFlowExecutor;
class EventFlowEventData;
class HitSensor;
class SensorMsg;
}  // namespace al

class CapSlotNpc : public al::LiveActor,
                   public al::IEventFlowEventReceiver,
                   public al::IEventFlowQueryJudge {
public:
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool receiveEvent(const al::EventFlowEventData* event) override;
    const char* judgeQuery(const char* query) const override;
    void movement() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;

    void init(const al::ActorInitInfo& info) override;
    void exeWait();
    void exeReaction();

private:
    s32 _118;
    TalkNpcParam* mTalkNpcParam;
    NpcStateReaction* mNpcStateReaction;
    al::EventFlowExecutor* mEventFlowExecutor;
    NpcJointLookAtController* mNpcJointLookAtController;
    Shine* mShine;
    const IItemExistanceJudge* mItemExistanceJudge;
};

static_assert(sizeof(CapSlotNpc) == 0x150);
