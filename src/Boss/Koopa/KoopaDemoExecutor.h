#pragma once

#include <basis/seadTypes.h>

namespace al {
class AddDemoInfo;
struct ActorInitInfo;
class EventFlowExecutor;
class LiveActor;
}  // namespace al

class ChurchDoor;
class IUseDemoSkip;
class KoopaCameraCtrl;
class KoopaCap;
class KoopaShip;
class Peach;

class KoopaDemoExecutor {
public:
    KoopaDemoExecutor();

    void init(al::LiveActor* actor, const al::ActorInitInfo& info,
              al::EventFlowExecutor* flowExecutor, KoopaCameraCtrl* cameraCtrl, Peach* peach);
    void initLv1(const al::ActorInitInfo& info, al::LiveActor* demoModel, KoopaCap* cap,
                 KoopaShip* ship);
    void registerDemoModel(al::LiveActor* actor);
    void initMoonChurch(const al::ActorInitInfo& info, ChurchDoor* churchDoor,
                        al::LiveActor* actor);
    void initLv2(const al::ActorInitInfo& info, al::LiveActor* demoModel, KoopaCap* cap,
                 al::LiveActor* ship);
    bool update();
    bool startDemoAction(const char* actionName, bool isResetEventDemoPlayerDynamics);
    void skip();
    void killAll();
    bool tryStartChurchEnterDemo(IUseDemoSkip* demoSkip, al::AddDemoInfo* demoInfo);
    void start(const char* eventName);
    bool tryStartChurchStartDemo(IUseDemoSkip* demoSkip, al::AddDemoInfo* demoInfo);
    bool tryStartBattleStartDemo(IUseDemoSkip* demoSkip);
    bool tryStartBattleEndDemo(IUseDemoSkip* demoSkip);
    bool tryStartClashBasementDemo(IUseDemoSkip* demoSkip);

    bool isRunningDemo() const { return mDemoState != 0; }

private:
    u8 _0[0x18];
    void* _18 = nullptr;
    u8 _20[0xb0];
    void* _d0 = nullptr;
    s64 _d8 = 0;
    u8 _e0[0x30];
    u8 mDemoState = 0;
    u8 _111 = 0;
    u8 _112 = 0;
    u8 _113 = 0;
    u8 _114 = 0;
    bool mIsStartEventFlow = false;
    u8 _116[2];
    s64 _118 = 0;
};

static_assert(sizeof(KoopaDemoExecutor) == 0x120);
