#include "Npc/SessionMusicianBgmCtrlObj.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveExecutor.h"

#include "Npc/SessionMusicianBgmController.h"

SessionBgmCtrlObj::SessionBgmCtrlObj(const char* name) : al::LiveActor(name) {}

void SessionBgmCtrlObj::init(const al::ActorInitInfo& initInfo) {
    al::initActorSceneInfo(this, initInfo);
    al::initExecutorMapObjMovement(this, initInfo);
    al::initActorSeKeeperWithout3D(this, initInfo, "SessionMusicianManager");

    mBgmController = new SessionMusicianBgmController(this, initInfo, true);
    makeActorAlive();
}

void SessionBgmCtrlObj::control() {
    mBgmController->updateNerve();
}
