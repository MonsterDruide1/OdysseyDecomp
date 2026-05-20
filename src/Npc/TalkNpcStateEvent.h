#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class EventFlowExecutor;
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al
class ActorStateReactionBase;
class TalkNpcParam;

class TalkNpcStateEvent : public al::ActorStateBase {
public:
    TalkNpcStateEvent(const char* name, al::LiveActor* actor, const TalkNpcParam* param,
                      al::EventFlowExecutor* eventFlowExecutor, s32 entryId);
    ~TalkNpcStateEvent() override;

    void initStateReaction(ActorStateReactionBase* stateReaction);
    void initAfterPlacement();
    void appear() override;
    void kill() override;
    bool update() override;
    bool trySwitchEventEntry(const char* entryName);
    bool trySwitchEventEntryAndUpdate(const char* entryName);
    void switchEventEntry(const char* entryName);
    bool tryStartReaction(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self);
    bool tryStartReactionWithoutTrample(const al::SensorMsg* message, al::HitSensor* other,
                                        al::HitSensor* self);
    bool isCapReaction() const;
    bool isScare() const;
    void exeActive();
    void endActive();
    void exeReaction();
    void exeScareStart();
    void exeScare();
    void exeScareEnd();
    void exeDemo();
    void exeStopByTraffic();
    void exeBalloonAction();

    al::EventFlowExecutor* getEventFlowExecutor() const { return mEventFlowExecutor; }

private:
    const TalkNpcParam* mTalkNpcParam = nullptr;
    al::EventFlowExecutor* mEventFlowExecutor = nullptr;
    ActorStateReactionBase* mStateReaction = nullptr;
    void* _38 = nullptr;
    void* _40 = nullptr;
    void* _48 = nullptr;
    void* _50 = nullptr;
    s32 mEntryId = 0;
    u16 _5c = 0;
    bool _5e = false;
    u8 _5f = 0;
};

static_assert(sizeof(TalkNpcStateEvent) == 0x60);
