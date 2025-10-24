#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}
class IPlayerModelChanger;
class PlayerEffect;

class PlayerDamageKeeper {
public:
    PlayerDamageKeeper(al::LiveActor* actor, PlayerEffect* playerEffect);
    void invalidate(s32 value);
    void invalidateIncludePush(s32 value);
    void damage(s32 level);
    void resetKidsModeLifeRecovery();
    bool tryShowKidsModeLifeOneCapMessage();
    void damageForce(s32 level);
    void dead();
    void reset(IPlayerModelChanger* modelChanger);
    void activatePreventDamage();
    void update(IPlayerModelChanger* modelChanger, bool unk, bool isPreventDamage);
    void updatePreventDamage();
    void updateKidsModeLifeRecovery(bool unk);

private:
    al::LiveActor* mPlayerActor = nullptr;
    PlayerEffect* mPlayerEffect = nullptr;
    bool mIsUpdateModel = false;
    s32 mCurrentHealth = 0;
    s32 mPush = 0;
    bool mIsPreventDamage = false;
    s32 mInvincibilityTimer = 0;
    s32 mHitPoint = 0;
    s32 mRecoveryCooldown = 0;
    s32 mKidsModeTimerForAutoHeal = 0;
};

static_assert(sizeof(PlayerDamageKeeper) == 0x30);
