#pragma once

#include <prim/seadSafeString.h>

#include "Library/Demo/DemoActor.h"

class PlayerHackStartShaderCtrl;
struct PlayerHackStartShaderParam;

class DemoActorHack : public al::DemoActor {
public:
    DemoActorHack(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    void makeActorAlive() override;
    void makeActorDead() override;

private:
    PlayerHackStartShaderCtrl* mPlayerHackStartShaderCtrl = nullptr;
    PlayerHackStartShaderParam* mPlayerHackStartShaderParam = nullptr;
    sead::FixedSafeString<32> mHackStartActionName;
    bool mIsKoopaHack = false;
};

static_assert(sizeof(DemoActorHack) == 0x1c0);
