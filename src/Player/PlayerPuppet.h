#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

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

class PlayerPuppet {
public:
    PlayerPuppet(al::LiveActor*, HackCap*, PlayerAnimator*, IUsePlayerCollision*,
                 ActorDimensionKeeper*, IPlayerModelChanger*, WorldEndBorderKeeper*,
                 PlayerCounterForceRun*, PlayerDamageKeeper*, PlayerEffect*, const PlayerInput*,
                 const PlayerConst*);

    virtual void start(al::HitSensor*, al::HitSensor*);
    virtual void end();
    virtual void cancel();
    virtual void setTrans(const sead::Vector3f&);
    virtual void setPose(const sead::Quatf&);
    virtual void setVelocity(const sead::Vector3f&);
    virtual void resetPosition(const sead::Vector3f&);
    virtual const sead::Vector3f& getTrans() const;
    virtual const sead::Vector3f& getVelocity() const;
    virtual const sead::Vector3f& getGravity() const;
    virtual void calcFront(sead::Vector3f*) const;
    virtual void calcUp(sead::Vector3f*) const;
    virtual void startAction(const sead::SafeStringBase<char>&) const;
    virtual bool isActionEnd() const;
    virtual bool isActionPlaying(const char*) const;
    virtual void setAnimRate(f32) const;
    virtual f32 getAnimFrameMax() const;
    virtual void hide();
    virtual void show();
    virtual bool isHidden() const;
    virtual void hideSilhouette();
    virtual void showSilhouette();
    virtual void hideShadow();
    virtual void showShadow();
    virtual void requestDamage();
    virtual void clearRequestDamage();
    virtual bool isRequestDamage() const;
    virtual void setBindEndOnGround();
    virtual bool isBindEndOnGround() const;
    virtual void setBindEndJump(const sead::Vector3f&, s32);
    virtual bool isBindEndJump() const;
    virtual void validateCollisionCheck();
    virtual void invalidateCollisionCheck();
    virtual bool isValidCollisionCheck();
    virtual bool isCollidedGround();
    virtual const sead::Vector3f& getCollidedGroundNormal();
    virtual void validateSensor();
    virtual void invalidateSensor();

    void startPlayerHitReaction(const char*);
    void setBindEndWallJump(const sead::Vector3f&, s32);
    void validate2D();
    void keepOn2D();
    void endKeepOn2D();
    void requestUpdateRecoveryInfo(bool, bool, const sead::Vector3f&, const sead::Vector3f&,
                                   const al::AreaObj*);
    void tryUpdateRecoveryInfo(bool*, bool*, sead::Vector3f*, sead::Vector3f*, const al::AreaObj**);
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
