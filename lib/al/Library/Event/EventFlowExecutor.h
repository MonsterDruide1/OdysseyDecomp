#pragma once

namespace al {
class EventFlowChart;
class EventFlowDataHolder;
class EventFlowMovement;
class EventFlowNode;
class EventFlowScareCtrlBase;
class HitSensor;
class LiveActor;

class EventFlowExecutor {
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
    virtual EventFlowDataHolder* getEventFlowDataHolder() const;

    LiveActor* getActor() const { return mActor; }

private:
    LiveActor* mActor;
    EventFlowChart* mEventFlowChart;
    const char* mName;
    EventFlowDataHolder* mEventFlowDataHolder;
    EventFlowNode* mEventFlowNode;
    EventFlowMovement* mEventFlowMovement;
    void* _38;
    EventFlowScareCtrlBase* mEventFlowScareCtrlBase;
};
}  // namespace al
