#include "al/LiveActor/PlayerActorBase.h"

PlayerActorBase::PlayerActorBase(const char *name) : LiveActor(name) {}
void PlayerActorBase::init(const al::ActorInitInfo&) {}
void PlayerActorBase::initPlayer(const al::ActorInitInfo&, const PlayerInitInfo&) {}
void* PlayerActorBase::getPlayerCollision() const { return nullptr; }
al::PlayerHackKeeper* PlayerActorBase::getPlayerHackKeeper() const { return nullptr; }
bool PlayerActorBase::isEnableDemo() { return false; }
void PlayerActorBase::startDemo() {}
void PlayerActorBase::endDemo() {}
void PlayerActorBase::startDemoPuppetable() {}
void PlayerActorBase::endDemoPuppetable() {}
void PlayerActorBase::startDemoShineGet() {}
void PlayerActorBase::endDemoShineGet() {}
void PlayerActorBase::startDemoMainShineGet() {}
void PlayerActorBase::endDemoMainShineGet() {}
void PlayerActorBase::startDemoHack() {}
void PlayerActorBase::endDemoHack() {}
void PlayerActorBase::startDemoKeepBind() {}
void PlayerActorBase::noticeDemoKeepBindExecute() {}
void PlayerActorBase::endDemoKeepBind() {}
void PlayerActorBase::startDemoKeepCarry() {}
void PlayerActorBase::endDemoKeepCarry() {}
void PlayerActorBase::getDemoActor() {}
void* PlayerActorBase::getDemoAnimator() { return nullptr; }
bool PlayerActorBase::isDamageStopDemo() const { return false; }
void* PlayerActorBase::getPlayerPuppet() { return nullptr; }
void* PlayerActorBase::getPlayerInfo() const { return nullptr; }

unsigned int PlayerActorBase::getPortNo() const {
    return mPortNo;
}
void* PlayerActorBase::getViewMtx() const {
    return mViewMtx;
}
