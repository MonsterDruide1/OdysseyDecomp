#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class AddDemoInfo;
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
              al::EventFlowExecutor* eventFlowExecutor, KoopaCameraCtrl* cameraCtrl, Peach* peach);
    void initLv1(const al::ActorInitInfo& info, al::LiveActor* actor, KoopaCap* cap,
                 KoopaShip* ship);
    void registerDemoModel(al::LiveActor* actor);
    void initMoonChurch(const al::ActorInitInfo& info, ChurchDoor* churchDoor,
                        al::LiveActor* actor);
    void initLv2(const al::ActorInitInfo& info, al::LiveActor* actor, KoopaCap* cap,
                 al::LiveActor* demoModel);
    bool update();
    void startDemoAction(const char* actionName, bool isNoPadRumble);
    void skip();
    void killAll();
    bool tryStartChurchEnterDemo(IUseDemoSkip* demoSkip, al::AddDemoInfo* demoInfo);
    void start(const char* demoName);
    bool tryStartChurchStartDemo(IUseDemoSkip* demoSkip, al::AddDemoInfo* demoInfo);
    bool tryStartBattleStartDemo(IUseDemoSkip* demoSkip);
    bool tryStartBattleEndDemo(IUseDemoSkip* demoSkip);
    bool tryStartClashBasementDemo(IUseDemoSkip* demoSkip);

private:
    u8 _0[0x120];
};

static_assert(sizeof(KoopaDemoExecutor) == 0x120);
