#pragma once

#include <cstdint>
#include "math/seadVector.h"

#include "Library/Area/AreaObj.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActor.h"

#include "Player/HackCap.h"

#include "System/GameDataUtil.h"
#include "Util/PlayerUtil.h"

class HackCap;
class IUseDimension;

namespace al {
class LiveActor;
struct ActorInitInfo;
class CollisionPartsFilterBase;
class AreaObj;
class HitSensor;
class ActorActionKeeper;
}  // namespace al

class PlayerRecoverySafetyPoint {
public:
    const sead::Vector3f* getSafetyPoint() const;
    const sead::Vector3f* getSafetyPointGravity() const;
    const al::AreaObj* getSafetyPointArea() const;

    void setRecoveryArea(const al::AreaObj* area);

    void checkInvalidateArea();

    void startBubbleWait();

    void endRecovery();

    bool isActiveRecoveryArea() const;
    bool isEnableRecovery() const;
    bool isValid() const;

    void updateRecoveryAreaValidity();

    PlayerRecoverySafetyPoint(const al::LiveActor* actor, const HackCap* hackCap,
                              const al::ActorInitInfo* initInfo, const IUseDimension* dimension,
                              al::CollisionPartsFilterBase* colFilter, al::HitSensor* hitSensor);

    void reset();

private:
    const al::LiveActor* mActor;
    const HackCap* mHackCap;
    const IUseDimension* mDimension;
    al::CollisionPartsFilterBase* mColFilter;
    al::HitSensor* mHitSensor;

    al::LiveActor* mBubble3D;
    al::LiveActor* mBubble2D;

    float mRecoveryTimer;  // unused

    uint8_t _pad01[4];

    alignas(4) bool mHasSafety3D;
    sead::Vector3f mSafetyPos3D;
    sead::Vector3f mSafetyNormal3D;
    sead::Vector3f mGravity3D;
    al::AreaObj* mArea3D;

    alignas(4) bool mHasSafety2D;
    sead::Vector3f mSafetyPos2D;
    sead::Vector3f mSafetyNormal2D;
    sead::Vector3f mGravity2D;
    al::AreaObj* mArea2D;

    alignas(4) bool mIsRecovering;

    uint8_t _pad02[4];

    const al::AreaObj* mRecoveryArea;
};
