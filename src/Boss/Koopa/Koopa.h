#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "Util/IUseDimension.h"

class ActorDimensionKeeper;
class KoopaPuppet;

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
    char filler_110[0x18];
    KoopaPuppet* mPuppet = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    char filler_138[0x70];
    sead::Quatf _1a8 = sead::Quatf::unit;
    char filler_1b8[0x28];
    sead::Matrix34f _1e0 = sead::Matrix34f::ident;
    u16 _210 = 0;
    char filler_212[0x6];
    void* _218 = nullptr;
};

static_assert(sizeof(Koopa) == 0x220);
