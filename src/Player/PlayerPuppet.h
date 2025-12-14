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

private:
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
    void* _58;
    PlayerInput* mPlayerInput;
    PlayerConst* mPlayerConst;
    al::HitSensor* _70;
    al::HitSensor* _78;
    bool _80;
    bool _81;
    bool _82;
    sead::Vector3f _84;
    sead::Vector3f _90;
    al::AreaObj* mAreaObj;
    bool mIsBindEndOnGround;
    bool mIsBindEndJump;
    bool _aa;
    bool mIsValidCollisionCheck;
    bool _ac;
    bool mIsRequestDamage;
    bool _ae;
    bool mIsSensorValid;
    bool _b0;
    bool _b1;
    bool _b2;
    bool _b3;
    bool _b4;
    bool _b5;
    bool _b6;

    char filler[0x8];

    sead::Vector3f _c8;
};

static_assert(sizeof(PlayerPuppet) == 0xd0);
