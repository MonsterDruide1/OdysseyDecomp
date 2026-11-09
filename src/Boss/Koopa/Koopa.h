#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class ActorDimensionKeeper;
class IUsePlayerHack;
class KoopaPuppet;
class PeachOnKoopaShoulder;

// Layout verified from Koopa::init / control / exeDeadFall asm (name is a guess):
// a 0x14-byte POD struct, zero-initialized with plain `new` (no ctor call).
// +0x0 bool, +0x4 s32, +0x8 s32 (air frames), +0xc s32 (ground frames), +0x10 s32 counter.
struct KoopaMoveCounter {
    bool _0;
    u8 _1[3];
    s32 _4;
    s32 _8;
    s32 _c;
    s32 _10;
};

class Koopa : public al::LiveActor, public IUseDimension {
public:
    Koopa(const char*);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void appear() override;
    void kill() override;
    void makeActorDead() override;
    void movement() override;
    void calcAnim() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    void control() override;
    void updateCollider() override;

    ActorDimensionKeeper* getActorDimensionKeeper() const override { return mDimensionKeeper; }

    bool tryStartHackActionIfNotPlaying(const char* actionName, bool);
    void startHackAction(const char* actionName, bool);
    void exeSwoon();
    void exeSwoonReaction();
    void exeDemoHackFirst();
    void exeStart();
    void exeWait();
    void endWait();
    void exeTurnPoint();
    void exeMove();
    void exeMoveBrake();
    void exeMoveBrakeTurn();
    void exeJumpStart();
    void exeJump();
    void exeJumpEnd();
    void exeFall();
    void exe2D();
    void end2D();
    void exeBind();
    void exeBindHackStart();
    void exeStop();
    void exeDead();
    void exeDeadFall();
    void startBind();
    void endBind();
    void endBindAndPopUp(const sead::Vector3f& velocity);
    void hideBindModel();
    void resetBindPosition(const sead::Vector3f& pos);
    bool calcPuppetMoveDir(sead::Vector3f* dir, const sead::Vector3f& up) const;
    bool isTriggerSwing() const;
    bool isBindWait() const;
    void changeHackTutorialElectricWire();
    void closeHackTutorial();
    void changeBind2D();
    void changeBindHackStart(const sead::Vector3f& startTrans, const sead::Vector3f& bindFront);
    f32 getBindHackStartActionFrame() const;
    f32 getBindHackStartActionFrameMax() const;

    KoopaPuppet* getPuppet() const { return mPuppet; }

private:
    IUsePlayerHack* mPlayerHack = nullptr;
    char filler_118[0x10];
    KoopaPuppet* mPuppet = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    char filler_138[0x18];
    KoopaMoveCounter* mMoveCounter = nullptr;
    char filler_158[0x10];
    PeachOnKoopaShoulder* mPeachOnKoopaShoulder = nullptr;
    char filler_170[0x38];
    sead::Quatf _1a8 = sead::Quatf::unit;
    char filler_1b8[0x28];
    sead::Matrix34f _1e0 = sead::Matrix34f::ident;
    u16 _210 = 0;
    char filler_212[0x6];
    void* _218 = nullptr;
};

static_assert(sizeof(Koopa) == 0x220);
