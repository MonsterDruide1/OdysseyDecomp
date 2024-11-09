#pragma once

#include <math/seadBoundBox.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class RollingCubePoseKeeper;
class RollingCubePose;
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
void calcMtxLandEffect(sead::Matrix34f*, const RollingCubePoseKeeper*, const sead::Quatf&,
                       const sead::Vector3f&);
void calcRollingCubeClippingInfo(sead::Vector3f*, f32*, const RollingCubePoseKeeper*, f32);
RollingCubePoseKeeper* createRollingCubePoseKeeper(const LiveActor* actor,
                                                   const ActorInitInfo& info);
RollingCubePoseKeeper* createRollingCubePoseKeeper(const sead::BoundBox3f& boundBox,
                                                   const ActorInitInfo& info);

class RollingCubePoseKeeper {
public:
    RollingCubePoseKeeper();

    void setCubeSize(const sead::BoundBox3f& cubeSize);
    bool isMoveTypeTurn() const;
    bool isMoveTypeLoop() const;
    void init(const ActorInitInfo& info);
    bool nextKey();
    void setStart();
    void setKeyIndex(s32 index);
    RollingCubePose* getCurrentPose() const;
    RollingCubePose* getPose(s32 index) const;
    void calcBoundingBoxCenter(sead::Vector3f*, const sead::Quatf&, const sead::Vector3f&) const;

private:
    RollingCubePose** mRollingCubePoses;
    s32 mRollingCubePoseCount;
    s32 mCurrentKey;
    sead::BoundBox3f _10;
    u32 mMoveType;
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
