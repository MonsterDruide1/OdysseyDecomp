#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IUseEventFlowData.h"
#include "Library/Nerve/IUseNerve.h"

namespace al {
struct ActorInitInfo;
class LiveActor;
class Nerve;

class EventFlowMovement : public IUseEventFlowData, public IUseNerve {
public:
    EventFlowMovement(const char* name, LiveActor* actor);

    EventFlowDataHolder* getEventFlowDataHolder() const override;

    virtual void init(const ActorInitInfo& info) {}

    virtual void appear();
    virtual void kill();
    virtual void control();
    NerveKeeper* getNerveKeeper() const override;
    virtual bool isTurnMovement() const;
    virtual bool isWaitAtPointMovement() const;

    void movement();
    void initNerve(const Nerve* nerve, s32 maxStates);

    void setEventFlowDataHolder(EventFlowDataHolder* dataHolder) {
        mEventFlowDataHolder = dataHolder;
    }

private:
    const char* mName;
    LiveActor* mActor;
    NerveKeeper* mNerveKeeper;
    EventFlowDataHolder* mEventFlowDataHolder;
};

class EventFlowMovementRail : public EventFlowMovement {
public:
    EventFlowMovementRail(LiveActor* actor);

    void init(const ActorInitInfo& info) override;
    void appear() override;
    void exeMove();
};

class EventFlowMovementTurnSeparate : public EventFlowMovement {
public:
    EventFlowMovementTurnSeparate(LiveActor* actor);

    void init(const ActorInitInfo& info) override;
    void appear() override;
    void exeWaitFar();
    void exeWaitNear();
    void exeTurn();
    bool isTurnMovement() const override;
    bool isWaitAtPointMovement() const override;

private:
    void* _30 = nullptr;
    void* _38 = nullptr;
    void* _40 = nullptr;
    void* _48 = nullptr;
    void* _50 = nullptr;
    s32 _58 = 0;
};

class EventFlowMovementWait : public EventFlowMovement {
public:
    EventFlowMovementWait(LiveActor* actor);

    void appear() override;
    bool isWaitAtPointMovement() const override;

private:
    bool _30 = true;
};

class EventFlowMovementWander : public EventFlowMovement {
public:
    EventFlowMovementWander(LiveActor* actor);

    void init(const ActorInitInfo& info) override;
    void appear() override;
    void kill() override;
    void exeWait();
    void exeTurnOneTime();
    void exeTurnLoop();
    void exeWalk();

private:
    void* _30 = nullptr;
    void* _38 = nullptr;
    void* _40 = nullptr;
    void* _48 = nullptr;
    void* _50 = nullptr;
    void* _58 = nullptr;
    f32 _60 = 500.0f;
};

static_assert(sizeof(EventFlowMovement) == 0x30);
static_assert(sizeof(EventFlowMovementRail) == 0x30);
static_assert(sizeof(EventFlowMovementTurnSeparate) == 0x60);
static_assert(sizeof(EventFlowMovementWait) == 0x38);
static_assert(sizeof(EventFlowMovementWander) == 0x68);
}  // namespace al
