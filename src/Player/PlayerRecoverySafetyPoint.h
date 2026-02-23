#pragma once

#include <math/seadVector.h>

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

struct SafetyPoint {
    bool hasSafety = false;
    sead::Vector3f safetyPos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f safetyNormal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f gravity = {0.0f, 0.0f, 0.0f};
    const al::AreaObj* area = nullptr;

    inline void reset() {
        hasSafety = false;
        safetyPos = {0.0f, 0.0f, 0.0f};
        safetyNormal = {0.0f, 0.0f, 0.0f};
        gravity = {0.0f, 0.0f, 0.0f};
        area = nullptr;
    }

    inline void set(const sead::Vector3f& rSafetyPos, const sead::Vector3f& rSafetyNormal,
                    const sead::Vector3f& rGravity, const al::AreaObj* pAreaObj) {
        hasSafety = true;
        safetyPos.set(rSafetyPos);
        safetyNormal.set(rSafetyNormal);
        gravity.set(rGravity);
        area = pAreaObj;
    }
};

class PlayerRecoverySafetyPoint {
public:
    const sead::Vector3f& getSafetyPoint() const;
    const sead::Vector3f& getSafetyPointGravity() const;
    const al::AreaObj* getSafetyPointArea() const;

    void setSafetyPoint(const sead::Vector3f& safetyPos, const sead::Vector3f& safetyNormal,
                        const al::AreaObj* areaObj);
    void setRecoveryArea(const al::AreaObj* area);
    void slideLastSafetyPoint(sead::Vector3f* safetyPos, sead::Vector3f* safetyNormal,
                              bool hasSafety, const sead::Vector3f& lastSafetyPos,
                              bool doValidateRay, al::CollisionPartsFilterBase* colFilter);
    void noticeDangerousPoint(const sead::Vector3f& pos, bool adjustFlag);
    void noticeRequestSafetyPoint(const sead::Vector3f& safetyNormal, const sead::Vector3f&,
                                  const al::AreaObj* areaObj);

    void checkInvalidateArea();

    void startBubbleWait();

    void startRecovery(f32 height);
    void endRecovery();

    bool isActiveRecoveryArea() const;
    bool isEnableRecovery() const;
    bool isValid() const;

    void updateRecoveryAreaValidity();
    void updateRecoveryBubble();

    PlayerRecoverySafetyPoint(const al::LiveActor* actor, const HackCap* hackCap,
                              const al::ActorInitInfo& initInfo, const IUseDimension* dimension,
                              al::CollisionPartsFilterBase* colFilter, al::HitSensor* hitSensor);

    void reset();

private:
    const al::LiveActor* mActor;
    const HackCap* mHackCap;
    const IUseDimension* mDimension;
    al::CollisionPartsFilterBase* mColFilter;
    al::HitSensor* mHitSensor;

    al::LiveActor* mTractorBubble = nullptr;
    al::LiveActor* mTractorBubble2D = nullptr;

    f32 mBubbleHeight = 0.0f;

    SafetyPoint mSafety3D;
    SafetyPoint mSafety2D;
    bool mIsRecovering = false;

    const al::AreaObj* mRecoveryArea = nullptr;
    sead::Vector3f* mDefaultSafetyPos = nullptr;  // Unsure of actual use
};

static_assert(sizeof(PlayerRecoverySafetyPoint) == 0xb8);
