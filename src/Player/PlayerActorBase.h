#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

#include "Player/IUsePlayerHack.h"

namespace al {
struct ActorInitInfo;
class DemoActor;
class HitSensor;
class SensorMsg;
}  // namespace al

class IUsePlayerCollision;
class PlayerAnimator;
class PlayerInfo;
class PlayerInitInfo;
class PlayerPuppet;

class PlayerActorBase : public al::LiveActor, public IUsePlayerHack {
public:
    PlayerActorBase(const char*);

    void init(const al::ActorInitInfo&) override;
    void movement() override;
    PlayerHackKeeper* getPlayerHackKeeper() const override;

    virtual void initPlayer(const al::ActorInitInfo&, const PlayerInitInfo&);
    virtual u32 getPortNo() const;
    virtual void* getViewMtx() const;  // NOTE: unknown return type
    virtual IUsePlayerCollision* getPlayerCollision() const;
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
    virtual al::DemoActor* getDemoActor();
    virtual PlayerAnimator* getDemoAnimator();
    virtual bool isDamageStopDemo() const;
    virtual PlayerPuppet* getPlayerPuppet();
    virtual PlayerInfo* getPlayerInfo() const;
    virtual bool checkDeathArea();
    virtual void sendCollisionMsg();
    virtual bool receivePushMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*, f32);

private:
    sead::Matrix34f* mViewMtx = nullptr;
    u32 mPortNo = 0;
};
