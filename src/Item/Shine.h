#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class Shine : public al::LiveActor, public IUseDimension {
public:
    Shine(const char*);

    void init(const al::ActorInitInfo&) override;
    al::LiveActor* getCurrentModel();
    bool tryExpandShadowAndClipping();
    void initAppearDemo(const al::ActorInitInfo&);
    void onAppear();
    void offAppear();
    void hideAllModel();
    void invalidateKillSensor();
    void initAfterPlacement() override;
    void getDirect();
    void updateHintTrans(const sead::Vector3f&) const;
    void appear() override;
    void makeActorAlive() override;
    void makeActorDead() override;
    void control() override;
    void updateModelActorPose();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
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
    bool isGot() const;
    bool isEmptyShineForDemoGetGrand() const;
    void setShopShine();
    bool isEndAppear() const;
    bool isEndAppearGK() const;
    void onSwitchGet();
    s32 getColorFrame() const;
    void setHintPhotoShine(const al::ActorInitInfo&);
    bool appearCommon();
    bool tryChangeCoin();
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
    bool tryWaitCameraInterpole() const;
    bool tryStartAppearDemo();
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
    ActorDimensionKeeper* getActorDimensionKeeper() const override;

    const sead::Vector3f& get_16c() { return _16c; }

    void set_16c(const sead::Vector3f& newValue) { _16c.set(newValue); }

private:
    char filler_16c[92];
    sead::Vector3f _16c;
    void* filler_380[65];
};

static_assert(sizeof(Shine) == 0x380);
