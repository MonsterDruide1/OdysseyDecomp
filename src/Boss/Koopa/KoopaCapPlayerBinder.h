#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class IUsePlayerPuppet;
class KoopaLandPointHolder;

class KoopaCapPlayerBinder : public al::NerveExecutor {
public:
    KoopaCapPlayerBinder(const KoopaLandPointHolder* landPointHolder);

    void kill();
    bool tryReceiveMsgBindStart(const al::SensorMsg* message);
    bool startPrepareCatchKoopaCapAndTryStartPuppetImmediately(al::HitSensor* sensor);
    bool tryStartPuppetCatchKoopaCap(const al::SensorMsg* message, al::HitSensor* other,
                                     al::HitSensor* self);
    void startPrepareKnockBackPunchL(al::HitSensor* sensor);
    void startPrepareKnockBackPunchR(al::HitSensor* sensor);
    bool tryStartPuppetKnockBackPunch(const al::SensorMsg* message, al::HitSensor* other,
                                      al::HitSensor* self);
    void startPrepareFinishPunchL(al::HitSensor* sensor);
    void startPrepareFinishPunchR(al::HitSensor* sensor);
    bool tryStartPuppetFinishPunch(const al::SensorMsg* message, al::HitSensor* other,
                                   al::HitSensor* self);
    bool tryStartPunchAndFocusTarget(const al::SensorMsg* message, al::HitSensor* other,
                                     al::HitSensor* self, const sead::Vector3f& target);
    void startPunchFinishKeepBind(bool isLeft);
    bool tryCancelBind(const al::SensorMsg* message);
    void copyPuppetQT(al::LiveActor* actor) const;
    bool tryStartPunchAction(bool isLeft);
    void endPunchBind();
    bool tryCancelPunchFinishBind();

    void exeWait();
    void exeCatchKoopaCapPrepare();
    void exeCatchKoopaCap();
    void exePunchPrepare();
    void exePunch();
    void exePunchKnockBackPrepare();
    void exePunchKnockBack();
    void exePunchFinishPrepare();
    void exePunchFinish();

    bool isBinding() const { return mPlayerPuppet != nullptr; }

private:
    IUsePlayerPuppet* mPlayerPuppet = nullptr;
    al::HitSensor* mBindSensor = nullptr;
    s32 mPunchType = 0;
    al::HitSensor* mHitSensor = nullptr;
    const KoopaLandPointHolder* mLandPointHolder = nullptr;
};

static_assert(sizeof(KoopaCapPlayerBinder) == 0x38);
