#include "Player/PlayerActorBase.h"

PlayerActorBase::PlayerActorBase(const char* name) : LiveActor(name) {}
void PlayerActorBase::init(const al::ActorInitInfo&) {}
void PlayerActorBase::initPlayer(const al::ActorInitInfo&, const PlayerInitInfo&) {}
IUsePlayerCollision* PlayerActorBase::getPlayerCollision() const {
    return nullptr;
}
PlayerHackKeeper* PlayerActorBase::getPlayerHackKeeper() const {
    return nullptr;
}
bool PlayerActorBase::isEnableDemo() {
    return false;
}
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
al::DemoActor* PlayerActorBase::getDemoActor() {
    return reinterpret_cast<al::DemoActor*>(this);
}
PlayerAnimator* PlayerActorBase::getDemoAnimator() {
    return nullptr;
}
bool PlayerActorBase::isDamageStopDemo() const {
    return false;
}
PlayerPuppet* PlayerActorBase::getPlayerPuppet() {
    return nullptr;
}
PlayerInfo* PlayerActorBase::getPlayerInfo() const {
    return nullptr;
}

u32 PlayerActorBase::getPortNo() const {
    return mPortNo;
}
void* PlayerActorBase::getViewMtx() const {
    return mViewMtx;
}
