#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IUseEventFlowData.h"
#include "Library/HostIO/HioNode.h"
#include "Library/Message/IUseMessageSystem.h"
#include "Library/Nerve/IUseNerve.h"

namespace al {
class Nerve;
class LiveActor;
class SceneEventFlowMsg;
class EventFlowNodeHolder;
class EventFlowNodeCaseEventHolder;
class EventFlowNodeInitInfo;

class EventFlowNode : public IUseEventFlowData,
                      public IUseMessageSystem,
                      public IUseNerve,
                      public IUseHioNode {
public:
    EventFlowNode(const char* name);

    EventFlowDataHolder* getEventFlowDataHolder() const override { return mEventFlowDataHolder; }

    virtual void init(const EventFlowNodeInitInfo& initInfo);

    virtual void initAfterChart() {}

    virtual void initAfterPlacement() {}

    virtual void start() { mIsStarted = true; }

    virtual void end() { mIsStarted = false; }

    virtual void control() {}

    virtual s32 getNextId() const { return mNextId; }

    virtual const char* getNextEntry() const { return nullptr; }

    const MessageSystem* getMessageSystem() const override { return mMessageSystem; }

    NerveKeeper* getNerveKeeper() const override { return mNerveKeeper; }

    virtual bool isEnableEndFork() const { return false; }

    void initNerve(const Nerve* nerve, s32 maxStates);
    void execute();

private:
    LiveActor* mActor = nullptr;
    s32 mId = -1;
    s32 mNextId = -1;
    const char* mName;
    bool mIsStarted = false;
    EventFlowDataHolder* mEventFlowDataHolder = nullptr;
    EventFlowNodeHolder* mEventFlowNodeHolder = nullptr;
    EventFlowNodeCaseEventHolder* mEventFlowNodeCaseEventHolder = nullptr;
    SceneEventFlowMsg* mSceneEventFlowMsg = nullptr;
    MessageSystem* mMessageSystem = nullptr;
    NerveKeeper* mNerveKeeper = nullptr;
};

static_assert(sizeof(EventFlowNode) == 0x68);
}  // namespace al
