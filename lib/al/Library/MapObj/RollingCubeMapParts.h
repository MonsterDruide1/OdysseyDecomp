#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class RollingCubePoseKeeper;
class PartsModel;
class PlacementInfo;

// TODO: Move this in correct header
void fittingToCurrentKeyBoundingBox(sead::Quatf* quat, sead::Vector3f* trans,
                                    const RollingCubePoseKeeper* rollingCubePoseKeeper);
void calcCurrentKeyQT(sead::Quatf* outQuat, sead::Vector3f* outTrans,
                      const RollingCubePoseKeeper* rollingCubePoseKeeper, const sead::Quatf&,
                      const sead::Vector3f&, f32 rate);
bool nextRollingCubeKey(RollingCubePoseKeeper* rollingCubePoseKeeper);
bool isMovementCurrentKeyRotate(const RollingCubePoseKeeper* rollingCubePoseKeeper);
const PlacementInfo& getCurrentKeyPlacementInfo(const RollingCubePoseKeeper* rollingCubePoseKeeper);

class RollingCubePoseKeeper {
public:
    void setStart();

private:
    unsigned char padding[0x30];
};

class RollingCubeMapParts : public LiveActor {
public:
    RollingCubeMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void kill() override;
    bool receiveMsg(const SensorMsg* message, HitSensor* source, HitSensor* target) override;
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
    sead::Matrix34f* _110 = nullptr;
    PartsModel* mPartsModel = nullptr;
    sead::Matrix34f _120 = sead::Matrix34f::ident;
    sead::Quatf _150 = sead::Quatf::unit;
    sead::Vector3f _160 = sead::Vector3f::zero;
    sead::Quatf _16c = sead::Quatf::unit;
    sead::Vector3f _17c = sead::Vector3f::zero;
    sead::Vector3f _188 = sead::Vector3f::zero;
    s32 mMovementTime = 0;
    bool _198 = false;
};

static_assert(sizeof(RollingCubeMapParts) == 0x1a0);
}  // namespace al
