#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IUseEventFlowData.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class EventFlowChart;
class EventFlowDataHolder;
class EventFlowMovement;
class EventFlowNode;
class EventFlowScareCtrlBase;
class HitSensor;
class LiveActor;

struct EventFlowExecutorState {
    bool isStopMovement = false;
    bool isInvalidUiCollisionCheck = false;
    bool hasLookAtJointCtrl = false;
};

class EventFlowExecutor : public HioNode, public IUseEventFlowData {
public:
    EventFlowExecutor();

    void init(LiveActor*, EventFlowChart*, EventFlowDataHolder*);
    void initMovement(EventFlowMovement*);
    void initScareCtrl(EventFlowScareCtrlBase*);
    void initAfterPlacement();
    void start(const char*);
    void execute();
    void attackSensor(HitSensor* self, HitSensor* other);
    void stopMovement();
    void restartMovement();
    bool isExistEntry(const char*) const;
    bool isWaitAtPointMovement() const;
    EventFlowDataHolder* getEventFlowDataHolder() const override;

    LiveActor* getActor() const { return mActor; }

    const char* getCurrentEntryName() const { return mCurrentEntryName; }

    EventFlowNode* getCurrentNode() const { return mCurrentNode; }

    EventFlowMovement* getMovement() const { return mMovement; }

    EventFlowExecutorState* getState() const { return mState; }

    EventFlowScareCtrlBase* getScareCtrl() const { return mScareCtrl; }

private:
    LiveActor* mActor = nullptr;
    EventFlowChart* mEventFlowChart = nullptr;
    const char* mCurrentEntryName = nullptr;
    EventFlowDataHolder* mEventFlowDataHolder = nullptr;
    EventFlowNode* mCurrentNode = nullptr;
    EventFlowMovement* mMovement = nullptr;
    EventFlowExecutorState* mState = nullptr;
    EventFlowScareCtrlBase* mScareCtrl = nullptr;
};

static_assert(sizeof(EventFlowExecutorState) == 0x3);
static_assert(sizeof(EventFlowExecutor) == 0x48);
}  // namespace al
