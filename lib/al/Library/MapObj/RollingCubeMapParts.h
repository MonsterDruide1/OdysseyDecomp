#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class PartsModel;
class RollingCubePoseKeeper;

class RollingCubeMapParts : public LiveActor {
public:
    RollingCubeMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void kill() override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void control() override;
    void appearAndSetStart();
    void setNerveNextMovement(bool isNextFallKey);
    bool isNextFallKey() const;
    void exeWait();
    void exeStart();
    void exeRotate();
    s32 getMovementTime() const;
    void setNerveNextLand();
    void exeSlide();
    bool updateSlide();
    void exeFall();
    void setNerveNextFallLand();
    void exeLand();
    s32 getLandTime() const;
    void exeFallLand();
    void exeStop();
    bool isStop() const;

private:
    RollingCubePoseKeeper* mRollingCubePoseKeeper = nullptr;
    sead::Matrix34f* mMoveLimitMtx = nullptr;
    PartsModel* mMoveLimitPartsModel = nullptr;
    sead::Matrix34f mLandEffectMtx = sead::Matrix34f::ident;
    sead::Quatf mInitialPoseQuat = sead::Quatf::unit;
    sead::Vector3f mInitialPoseTrans = sead::Vector3f::zero;
    sead::Quatf mCurrentPoseQuat = sead::Quatf::unit;
    sead::Vector3f mCurrentPoseTrans = sead::Vector3f::zero;
    sead::Vector3f mClippingTrans = sead::Vector3f::zero;
    s32 mMovementTime = 0;
    bool mIsStoppable = false;
};

static_assert(sizeof(RollingCubeMapParts) == 0x1a0);
}  // namespace al
