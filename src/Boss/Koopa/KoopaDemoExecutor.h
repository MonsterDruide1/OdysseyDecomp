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

    void init(al::LiveActor*, const al::ActorInitInfo&, al::EventFlowExecutor*, KoopaCameraCtrl*,
              Peach*);
    void initLv1(const al::ActorInitInfo&, al::LiveActor*, KoopaCap*, KoopaShip*);
    void registerDemoModel(al::LiveActor*);
    void initMoonChurch(const al::ActorInitInfo&, ChurchDoor*, al::LiveActor*);
    void initLv2(const al::ActorInitInfo&, al::LiveActor*, KoopaCap*, al::LiveActor*);
    bool update();
    void startDemoAction(const char*, bool);
    void skip();
    void killAll();
    bool tryStartChurchEnterDemo(IUseDemoSkip*, al::AddDemoInfo*);
    void start(const char*);
    bool tryStartChurchStartDemo(IUseDemoSkip*, al::AddDemoInfo*);
    bool tryStartBattleStartDemo(IUseDemoSkip*);
    bool tryStartBattleEndDemo(IUseDemoSkip* demoSkip);
    bool tryStartClashBasementDemo(IUseDemoSkip*);

private:
    u8 _0[0x120];
};

static_assert(sizeof(KoopaDemoExecutor) == 0x120);
