#pragma once

#include "al/LiveActor/LiveActor.h"
#include "al/iuse/IUsePlayerHack.h"

class IUsePlayerCollision;
class PlayerInfo;
class PlayerInitInfo;

class PlayerActorBase : public al::LiveActor, public al::IUsePlayerHack {
public:
    PlayerActorBase(const char*);

    virtual void init(const al::ActorInitInfo&) override;
    virtual void initPlayer(const al::ActorInitInfo&, const PlayerInitInfo&);
    virtual unsigned int getPortNo() const;
    virtual void* getViewMtx() const;  // NOTE: unknown return type
    virtual IUsePlayerCollision* getPlayerCollision() const;
    virtual al::PlayerHackKeeper* getPlayerHackKeeper() const override;
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
    virtual bool receivePushMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);

private:
    void* mViewMtx = nullptr;  // NOTE: unknown type
    unsigned int mPortNo = 0;
};
