#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class AreaObj;
class ActorInitInfo;
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
    void* _0[0xb8 / 8];
};

static_assert(sizeof(PlayerRecoverySafetyPoint) == 0xb8);
