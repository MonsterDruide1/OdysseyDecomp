#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class Shine : public al::LiveActor, public IUseDimension {
public:
    Shine(const char*);

    void init(const al::ActorInitInfo&) override;
    void getCurrentModel();
    void tryExpandShadowAndClipping();
    void initAppearDemo(const al::ActorInitInfo&);
    void onAppear();
    void offAppear();
    void hideAllModel();
    void invalidateKillSensor();
    void initAfterPlacement() override;
    void getDirect();
    void updateHintTrans(const sead::Vector3f&);
    void appear() override;
    void makeActorAlive() override;
    void makeActorDead() override;
    void control() override;
    void updateModelActorPose();
    void attackSensor(al::HitSensor*, al::HitSensor*) override;
    bool receiveMsg(const al::SensorMsg*, al::HitSensor*, al::HitSensor*) override;
    void showCurrentModel();
    void appearPopup();
    void addDemoActorWithModel();
    void get();
    void endClipped() override;
    void initAppearDemoFromHost(const al::ActorInitInfo&, const sead::Vector3f&);
    void initAppearDemoFromHost(const al::ActorInitInfo&);
    void initAppearDemoActorCamera(const al::ActorInitInfo&);
    void createShineEffectInsideObject(const al::ActorInitInfo&, const sead::Vector3f&,
                                       const char*);
    bool isGot();
    bool isEmptyShineForDemoGetGrand();
    void setShopShine();
    bool isEndAppear();
    bool isEndAppearGK();
    void onSwitchGet();
    void getColorFrame();
    void setHintPhotoShine(const al::ActorInitInfo&);
    void appearCommon();
    void tryChangeCoin();
    void tryAppearOrDemoAppear();

    void appearPopup(const sead::Vector3f&);
    void appearPopupDelay(s32);
    void appearPopupSlot(const sead::Vector3f&);
    void appearWarp(const sead::Vector3f&, const sead::Vector3f&);
    void appearStatic();
    void appearPopupWithoutDemo();
    void appearPopupGrandByBoss(s32);
    void appearPopupWithoutWarp();
    void appearAndJoinBossDemo(const char*, const sead::Quatf&, const sead::Vector3f&);
    void endBossDemo();
    void endBossDemoAndStartFall(f32);
    void appearWait();
    void appearWait(const sead::Vector3f&);

    void startHold();
    void startFall();
    void getDirectWithDemo();
    void addDemoModelActor();
    void setGrandShine();

    void exeWaitRequestDemo();
    void exeWaitKill();
    void exeDemoAppear();

    void tryWaitCameraInterpole();
    void tryStartAppearDemo();
    void calcCameraAt();

    void exeDemoMove();
    void updateIgnoreFrame();
    void exeDemoWait();
    void exeDemoGet();
    void exeDemoGetMain();
    void exeDemoGetGrand();

    void exeBossDemo();
    void exeBossDemoAfterFall();
    void exeBossDemoAfterLanding();
    void exeBossDemoFall();
    void exeBossDemoFallSlowdown();
    void exeBossDemoRise();
    void exeBossDemoRiseDamp();

    void exeAppearSlot();
    void exeAppearSlotDown();
    void exeAppear();
    void exeAppearWait();
    void exeAppearDown();
    void exeAppearStatic();
    void exeAppearEnd();
    void exeAppearWaitCameraInterpole();

    void exeWait();
    void exeGot();
    void exeHold();
    void exeFall();
    void exeDelay();
    void exeHide();
    void exeReaction();
    void exeCoin();

    void updateModelActorResetPosition();

    inline ActorDimensionKeeper* getActorDimensionKeeper() const override;

private:
    // TODO
};

namespace ShineFunction {
void getMovePointLinkName();
}
