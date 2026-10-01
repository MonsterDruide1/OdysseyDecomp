#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class StageTalkDemoNpcCap : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    StageTalkDemoNpcCap(const char* name, s32 demoType, s32 flowType = 0);

    void init(const al::ActorInitInfo& info) override;
    void startDemo();
    bool isEnableShowDemoAfterOpenMoonRockFirst() const;
    bool isEnableShowDemoMoonRockMapWorld() const;
    bool isSceneDemo() const;
    void kill() override;
    bool receiveEvent(const al::EventFlowEventData* event) override;
    bool tryStartDemo();
    void startDemoFromScene();
    bool isEnableStartMoonRockFindDemo() const;
    bool isEnableStartAfterBreakMoonRockDemo() const;
    void preEventFromSceneFirstMoonGet(const char* eventName);
    bool isEnableStageStartDemo() const;
    bool isEndDemo() const;
    bool tryStartDemoAfterMoonRockBreak();
    void exeWait();
    void exeWaitSwitch();
    void exeDelay();
    void exeDemo();
    void endDemoCommon();
    void exeDemoMoonRockMap();

private:
    s32 mFlowType = 0;
    const char* mEventFlowName = nullptr;
    void* mStageStartInfo = nullptr;
    bool mIsEnableDemo = true;
    void* mAddDemoInfo = nullptr;
    al::LiveActor* mMoonRock = nullptr;
    void* mSaveObjInfo = nullptr;
    s32 _148 = 0;
    void* mDemoState = nullptr;
    al::LiveActor* mCapManHero = nullptr;
    bool mIsStartedFromScene = false;
    al::LiveActor* mRippleGeneratePoint = nullptr;
};

static_assert(sizeof(StageTalkDemoNpcCap) == 0x170);
