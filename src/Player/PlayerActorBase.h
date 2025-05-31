#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Player/IUsePlayerHack.h"

namespace al {
class DemoActor;
}

class IUsePlayerCollision;
class PlayerAnimator;
class PlayerInfo;
class PlayerInitInfo;
class PlayerHackKeeper;
class PlayerPuppet;

class PlayerActorBase : public al::LiveActor, public IUsePlayerHack {
public:
    PlayerActorBase(const char* name);

    void init(const al::ActorInitInfo& initInfo) override {}

    void movement() override;

    virtual void initPlayer(const al::ActorInitInfo& actorInitInfo,
                            const PlayerInitInfo& playerInitInfo) {}

    virtual u32 getPortNo() const;
    virtual sead::Matrix34f* getViewMtx() const;

    virtual IUsePlayerCollision* getPlayerCollision() const { return nullptr; }

    PlayerHackKeeper* getPlayerHackKeeper() const override { return nullptr; }

    virtual bool isEnableDemo() { return false; }

    virtual void startDemo() {}

    virtual void endDemo() {}

    virtual void startDemoPuppetable() {}

    virtual void endDemoPuppetable() {}

    virtual void startDemoShineGet() {}

    virtual void endDemoShineGet() {}

    virtual void startDemoMainShineGet() {}

    virtual void endDemoMainShineGet() {}

    virtual void startDemoHack() {}

    virtual void endDemoHack() {}

    virtual void startDemoKeepBind() {}

    virtual void noticeDemoKeepBindExecute() {}

    virtual void endDemoKeepBind() {}

    virtual void startDemoKeepCarry() {}

    virtual void endDemoKeepCarry() {}

    virtual al::DemoActor* getDemoActor() { return reinterpret_cast<al::DemoActor*>(this); }

    virtual PlayerAnimator* getDemoAnimator() { return nullptr; }

    virtual bool isDamageStopDemo() const { return false; }

    virtual PlayerPuppet* getPlayerPuppet() { return nullptr; }

    virtual PlayerInfo* getPlayerInfo() const { return nullptr; }

    virtual bool checkDeathArea();
    virtual void sendCollisionMsg();
    virtual bool receivePushMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self,
                                f32 maxPush);

private:
    sead::Matrix34f* mViewMtx = nullptr;
    u32 mPortNo = 0;
};
