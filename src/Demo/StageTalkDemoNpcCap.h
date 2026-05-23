#pragma once

#include <basis/seadTypes.h>

#include "Library/Event/IEventFlowEventReceiver.h"
#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
}  // namespace al

class StageTalkDemoNpcCap : public al::LiveActor, public al::IEventFlowEventReceiver {
public:
    StageTalkDemoNpcCap(const char*, s32, s32);

    void init(const al::ActorInitInfo&) override;
    void startDemo();
    bool isEnableShowDemoAfterOpenMoonRockFirst() const;
    bool isEnableShowDemoMoonRockMapWorld() const;
    bool isSceneDemo() const;
    void kill() override;
    bool receiveEvent(const al::EventFlowEventData*) override;
    bool tryStartDemo();
    void startDemoFromScene();
    bool isEnableStartMoonRockFindDemo() const;
    bool isEnableStartAfterBreakMoonRockDemo() const;
    void preEventFromSceneFirstMoonGet(const char*);
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
    s32 _110;
    void* _118 = nullptr;
    void* _120 = nullptr;
    u8 _128 = 1;
    void* _130 = nullptr;
    void* _138 = nullptr;
    void* _140 = nullptr;
    s32 _148 = 0;
    void* _150 = nullptr;
    void* _158 = nullptr;
    u8 _160 = 0;
    void* _168 = nullptr;
};

static_assert(sizeof(StageTalkDemoNpcCap) == 0x170);
