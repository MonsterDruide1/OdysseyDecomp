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
    void invalidate(s32 damageInvalidCount);
    void invalidateIncludePush(s32 damageInvalidCountAbyss);
    void damage(s32 damageInvalidCount);
    void resetKidsModeLifeRecovery();
    bool tryShowKidsModeLifeOneCapMessage();
    void damageForce(s32 damageInvalidCount);
    void dead();
    void reset(IPlayerModelChanger* modelChanger);
    void activatePreventDamage();
    void update(IPlayerModelChanger* modelChanger, bool isRecover, bool isPreventDamage);
    void updatePreventDamage();
    void updateKidsModeLifeRecovery(bool isRecover);

private:
    al::LiveActor* mPlayer = nullptr;
    PlayerEffect* mPlayerEffect = nullptr;
    bool mIsUpdateModel = false;
    s32 mDamageInvalidCount = 0;
    s32 mDamageInvalidCountAbyss = 0;
    bool mIsPreventDamage = false;
    s32 mInvincibilityTimer = 0;
    s32 mKidsModeHealth = 0;            // used to reset healing counter if mismatches player health
    s32 mKidsModeRecoveryCooldown = 0;  // usually [0-60], resets on healing and movement
    s32 mKidsModeStandStillCounter = 0;  // [0-120], resets on movement
};

static_assert(sizeof(PlayerDamageKeeper) == 0x30);
