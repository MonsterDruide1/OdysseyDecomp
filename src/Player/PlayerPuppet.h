#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "Player/IUsePlayerPuppet.h"

namespace al {
class AreaObj;
class HitSensor;
class LiveActor;
}  // namespace al

class ActorDimensionKeeper;
class HackCap;
class IJudge;
class IPlayerModelChanger;
class IUsePlayerCollision;
class PlayerAnimator;
class PlayerDamageKeeper;
class PlayerConst;
class PlayerCounterForceRun;
class PlayerEffect;
class PlayerInput;
class WorldEndBorderKeeper;

class PlayerPuppet : public IUsePlayerPuppet {
public:
    PlayerPuppet(al::LiveActor*, HackCap*, PlayerAnimator*, IUsePlayerCollision*,
                 ActorDimensionKeeper*, IPlayerModelChanger*, WorldEndBorderKeeper*,
                 PlayerCounterForceRun*, PlayerDamageKeeper*, PlayerEffect*, const PlayerInput*,
                 const PlayerConst*);

    void start(al::HitSensor*, al::HitSensor*) override;
    void end() override;
    void cancel() override;
    void setTrans(const sead::Vector3f&) override;
    void setPose(const sead::Quatf&) override;
    void setVelocity(const sead::Vector3f&) override;
    void resetPosition(const sead::Vector3f&) override;
    const sead::Vector3f& getTrans() const override;
    const sead::Vector3f& getVelocity() const override;
    const sead::Vector3f& getGravity() const override;
    void calcFront(sead::Vector3f*) const override;
    void calcUp(sead::Vector3f*) const override;
    void startAction(const sead::SafeString&) const override;
    bool isActionEnd() const override;
    bool isActionPlaying(const char*) const override;
    void setAnimRate(f32) const override;
    f32 getAnimFrameMax() const override;
    void hide() override;
    void show() override;
    bool isHidden() const override;
    void hideSilhouette() override;
    void showSilhouette() override;
    void hideShadow() override;
    void showShadow() override;
    void requestDamage() override;
    void clearRequestDamage() override;
    bool isRequestDamage() const override;
    void setBindEndOnGround() override;
    bool isBindEndOnGround() const override;
    void setBindEndJump(const sead::Vector3f&, s32) override;
    bool isBindEndJump() const override;
    void validateCollisionCheck() override;
    void invalidateCollisionCheck() override;
    bool isValidCollisionCheck() override;
    bool isCollidedGround() override;
    const sead::Vector3f& getCollidedGroundNormal() override;
    void validateSensor() override;
    void invalidateSensor() override;

    void startPlayerHitReaction(const char*);
    void setBindEndWallJump(const sead::Vector3f&, s32);
    void validate2D();
    void keepOn2D();
    void endKeepOn2D();
    void requestUpdateRecoveryInfo(bool, bool, const sead::Vector3f&, const sead::Vector3f&,
                                   const al::AreaObj*);
    bool tryUpdateRecoveryInfo(bool*, bool*, sead::Vector3f*, sead::Vector3f*, const al::AreaObj**);
    bool isBinding() const;
    bool isNoCollide() const;

    PlayerAnimator* getPlayerAnimator() const { return mPlayerAnimator; }

    HackCap* getHackCap() const { return mHackCap; }

    PlayerCounterForceRun* getPlayerCounterForceRun() const { return mPlayerCounterForceRun; }

    PlayerEffect* getPlayerEffect() const { return mPlayerEffect; }

    IJudge* getJumpButtonJudge() const { return mJumpButtonJudge; }

    const PlayerInput* getPlayerInput() const { return mPlayerInput; }

    const PlayerConst* getPlayerConst() const { return mPlayerConst; }

    al::HitSensor* getPlayerHitSensor() const { return mPlayerHitSensor; }

    void setBindEndCapThrow() { mIsBindEndCapThrow = true; }

    void validateLookAt() { mIsLookAtValid = true; }

    void invalidateLookAt() { mIsLookAtValid = false; }

    void validateForceLookAt(const sead::Vector3f& lookAt) {
        mIsForceLookAtValid = true;
        mForceLookAt.x = lookAt.x;
        mForceLookAt.y = lookAt.y;
        mForceLookAt.z = lookAt.z;
    }

    void invalidateForceLookAt() { mIsForceLookAtValid = false; }

    void validateSeparateMode() { mIsSeparateMode = true; }

    void invalidateSeparateMode() { mIsSeparateMode = false; }

    void validateReceivePush() { mIsReceivePushInvalid = false; }

    void invalidateReceivePush() { mIsReceivePushInvalid = true; }

    void validateRecoveryArea() { mIsRecoveryAreaValid = true; }

    void invalidateRecoveryArea() { mIsRecoveryAreaValid = false; }

    void validateGuideArrow() { mIsGuideArrowValid = true; }

    void invalidateGuideArrow() { mIsGuideArrowValid = false; }

    void validateWaterSurfaceShadow() { mIsWaterSurfaceShadowValid = true; }

    void invalidateWaterSurfaceShadow() { mIsWaterSurfaceShadowValid = false; }

    void markDitherAlphaCopied() { mIsDitherAlphaCopied = true; }

private:
    struct BindEndInfo {
        sead::Vector3f trans;
        s32 timeBeforeEnd;
        f32 moveSpeed;
    };

    static_assert(sizeof(BindEndInfo) == 0x14);

    al::LiveActor* mActor;
    HackCap* mHackCap;
    PlayerAnimator* mPlayerAnimator;
    IUsePlayerCollision* mIUsePlayerCollision;
    ActorDimensionKeeper* mActorDimensionKeeper;
    IPlayerModelChanger* mIPlayerModelChanger;
    WorldEndBorderKeeper* mWorldEndBorderKeeper;
    PlayerCounterForceRun* mPlayerCounterForceRun;
    PlayerDamageKeeper* mPlayerDamageKeeper;
    PlayerEffect* mPlayerEffect;
    IJudge* mJumpButtonJudge;
    const PlayerInput* mPlayerInput;
    const PlayerConst* mPlayerConst;
    al::HitSensor* mActorHitSensor;
    al::HitSensor* mPlayerHitSensor;
    bool mHasRecoveryInfo;
    bool mHasRecoveryInfoSafetyPoint;
    bool mIsRecoveryInfoNoSafetyPoint;
    sead::Vector3f mRecoveryTrans;
    sead::Vector3f mRecoveryNormal;
    const al::AreaObj* mRecoveryAreaObj;
    bool mIsBindEndOnGround;
    bool mIsBindEndJump;
    bool mIsBindEndCapThrow;
    bool mIsValidCollisionCheck;
    bool mIsReceivePushInvalid;
    bool mIsRequestDamage;
    bool _ae;
    bool mIsSensorValid;
    bool mIsLookAtValid;
    bool mIsForceLookAtValid;
    bool mIsRecoveryAreaValid;
    bool mIsSeparateMode;
    bool mIsGuideArrowValid;
    bool mIsWaterSurfaceShadowValid;
    bool mIsDitherAlphaCopied;
    sead::Vector3f mForceLookAt;
    BindEndInfo* mBindEndInfo;
};

static_assert(sizeof(PlayerPuppet) == 0xd0);
