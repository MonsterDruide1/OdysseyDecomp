#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class AreaObj;
struct ActorInitInfo;
class CollisionPartsFilterBase;
class HitSensor;
}  // namespace al
class HackCap;
class IUseDimension;

class PlayerRecoverySafetyPoint {
public:
    PlayerRecoverySafetyPoint(const al::LiveActor*, const HackCap*, const al::ActorInitInfo&,
                              const IUseDimension*, al::CollisionPartsFilterBase*, al::HitSensor*);
    void reset();
    void setSafetyPoint(const sead::Vector3f&, const sead::Vector3f&, const al::AreaObj*);
    void noticeRequestSafetyPoint(const sead::Vector3f&, const sead::Vector3f&, const al::AreaObj*);
    void noticeDangerousPoint(const sead::Vector3f&, bool);
    void slideLastSafetyPoint(sead::Vector3f*, sead::Vector3f*, bool, const sead::Vector3f&, bool,
                              al::CollisionPartsFilterBase*);
    bool isValid() const;
    bool isEnableRecovery() const;
    const sead::Vector3f& getSafetyPoint() const;
    const sead::Vector3f& getSafetyPointGravity() const;
    const sead::Vector3f& getSafetyPointArea() const;
    void updateRecoveryAreaValidity();
    void setRecoveryArea(const al::AreaObj*);
    bool isActiveRecoveryArea() const;
    void checkInvalidateArea();
    void startRecovery(f32);
    void updateRecoveryBubble();
    void startBubbleWait();
    void endRecovery();

private:
    al::LiveActor* mPlayer;
    HackCap* mHackCap;
    IUseDimension* mDimension;
    al::CollisionPartsFilterBase* mCollisionPartsFilter;
    al::HitSensor* mHitSensor;
    al::LiveActor* mBubble3D;
    al::LiveActor* mBubble2D;
    f32 mRecoveryTimer;
    f32 _34;
    bool mHasSafety3D;
    sead::Vector3f mSafetyPos3D;
    sead::Vector3f mSafetyNormal3D;
    sead::Vector3f mGravity3D;
    al::AreaObj* mArea3D;
    bool mHasSafety2D;
    sead::Vector3f mSafetyPos2D;
    sead::Vector3f mSafetyNormal2D;
    sead::Vector3f mGravity2D;
    al::AreaObj* mArea2D;
    bool mIsRecovering;
    al::AreaObj* mRecoveryArea;
    sead::Vector3f* mSafetyPoint;
};

static_assert(sizeof(PlayerRecoverySafetyPoint) == 0xb8);
