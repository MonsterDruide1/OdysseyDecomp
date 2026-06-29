#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class ExternalForceKeeper;
class IUsePlayerCollision;

class PlayerExternalVelocity {
public:
    PlayerExternalVelocity(const al::LiveActor* actor, const IUsePlayerCollision* collision,
                           const sead::Vector3f* trans);

    bool receiveMsgPlayer(const al::SensorMsg* msg, al::HitSensor* self, al::HitSensor* other);
    bool receiveMsgCap(const al::SensorMsg* msg, al::HitSensor* self, al::HitSensor* other);
    void requestApplyLastGroundInertia();
    void cancelAndFeedbackLastGroundInertia(al::LiveActor* actor, f32 strength, bool isFrontOnly);
    void update();
    void updatePadRumbleExternalForce();
    void updateLastGroundForce(bool isOnGround);
    void reset();
    bool isExistForce() const;
    bool isExistSnapForce() const;

private:
    struct FloatParam {
        f32 onGround;
        f32 offGround;

        f32 getValue(bool isOnGround) const { return isOnGround ? onGround : offGround; }
    };

    ExternalForceKeeper* mExternalForceKeeper = nullptr;
    sead::Vector3f mExternalVelocity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mAreaVelocity = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mTotalVelocity = {0.0f, 0.0f, 0.0f};
    const al::LiveActor* mActor;
    const IUsePlayerCollision* mCollision;
    const sead::Vector3f* mTrans;
    bool mIsRumbleActive = false;
    sead::Vector3f mExternalForce = {0.0f, 0.0f, 0.0f};

    FloatParam mExternalDrag = {0.95f, 0.955f};
    FloatParam mAreaDrag;
    FloatParam mSensitivity = {0.275f, 0.275f};

    sead::Vector3f mSnapForce = {0.0f, 0.0f, 0.0f};
    s32 mApplyLastGroundInertiaFrames = 0;
    sead::Vector3f mMovePowerForce = {0.0f, 0.0f, 0.0f};
    sead::Vector3f mLastGroundInertia = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(PlayerExternalVelocity) == 0x98);
