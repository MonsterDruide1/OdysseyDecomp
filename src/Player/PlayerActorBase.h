#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "Player/IUsePlayerHack.h"

class IUsePlayerCollision;
class PlayerInfo;
class PlayerInitInfo;
class PlayerHackKeeper;

class PlayerActorBase : public al::LiveActor, public IUsePlayerHack {
public:
    PlayerActorBase(const char*);

    virtual void init(const al::ActorInitInfo&) override;
    virtual void initPlayer(const al::ActorInitInfo&, const PlayerInitInfo&);
    virtual u32 getPortNo() const;
    virtual const sead::Matrix34f* getViewMtx() const;
    virtual IUsePlayerCollision* getPlayerCollision() const;
    virtual PlayerHackKeeper* getPlayerHackKeeper() const override;
    virtual bool isEnableDemo();
    virtual void startDemo();
    virtual void endDemo();
    virtual void startDemoPuppetable();
    virtual void endDemoPuppetable();
    virtual void startDemoShineGet();
    virtual void endDemoShineGet();
    virtual void startDemoMainShineGet();
    virtual void endDemoMainShineGet();
    virtual void startDemoHack();
    virtual void endDemoHack();
    virtual void startDemoKeepBind();
    virtual void noticeDemoKeepBindExecute();
    virtual void endDemoKeepBind();
    virtual void startDemoKeepCarry();
    virtual void endDemoKeepCarry();
    virtual void getDemoActor();
    virtual void* getDemoAnimator();            // NOTE: unknown return type
    virtual bool isDamageStopDemo() const;      // NOTE: unknown return type
    virtual void* getPlayerPuppet();            // NOTE: unknown return type
    virtual PlayerInfo* getPlayerInfo() const;  // NOTE: unknown return type
    virtual void movement() override;
    virtual bool checkDeathArea();
    virtual void sendCollisionMsg();
    virtual bool receivePushMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*, f32);

private:
    const sead::Matrix34f* mViewMtx = nullptr;
    u32 mPortNo = 0;
};
