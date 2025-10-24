#include "Player/PlayerDamageKeeper.h"

#include <math/seadMathCalcCommon.h>

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Bgm/BgmPlayingRequest.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Se/SeFunction.h"

#include "MapObj/CapMessageShowInfo.h"
#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerEffect.h"
#include "Player/PlayerFunction.h"
#include "Sequence/GameSequenceInfo.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolder.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"
#include "Util/SpecialBuildUtil.h"

PlayerDamageKeeper::PlayerDamageKeeper(al::LiveActor* actor, PlayerEffect* playerEffect)
    : mPlayerActor(actor), mPlayerEffect(playerEffect) {}

void PlayerDamageKeeper::invalidate(s32 minHealth) {
    mIsUpdateModel = true;
    mCurrentHealth = sead::Mathi::clampMin(mCurrentHealth, minHealth);
}

void PlayerDamageKeeper::invalidateIncludePush(s32 push) {
    invalidate(push);
    mPush = push;
}

void PlayerDamageKeeper::damage(s32 level) {
    mIsUpdateModel = true;
    mCurrentHealth = level;
    if (mIsPreventDamage)
        return;

    GameDataFunction::damagePlayer(mPlayerActor);
    resetKidsModeLifeRecovery();
    tryShowKidsModeLifeOneCapMessage();
}

void PlayerDamageKeeper::resetKidsModeLifeRecovery() {
    mHitPoint = GameDataFunction::getPlayerHitPoint(mPlayerActor);
    mRecoveryCooldown = 0;
    if (mKidsModeTimerForAutoHeal < 120)
        mKidsModeTimerForAutoHeal = 0;
}

bool PlayerDamageKeeper::tryShowKidsModeLifeOneCapMessage() {
    if (rs::isKidsMode(mPlayerActor) && GameDataFunction::getPlayerHitPoint(mPlayerActor) == 1 &&
        !rs::isSceneStatusBossBattle(mPlayerActor) &&
        rs::isEnableCapMessageLifeOneKidsMode(mPlayerActor) &&
        !rs::isShowCapMsgPlayerLife1(mPlayerActor)) {
        rs::showCapMsgPlayerLife1(mPlayerActor, 100);
        rs::disableCapMessageLifeOneKidsMode(mPlayerActor);
        return true;
    }
    return false;
}

void PlayerDamageKeeper::damageForce(s32 level) {
    mIsUpdateModel = true;
    mCurrentHealth = level;

    GameDataFunction::damagePlayer(mPlayerActor);
    resetKidsModeLifeRecovery();
    tryShowKidsModeLifeOneCapMessage();
}

void PlayerDamageKeeper::dead() {
    GameDataFunction::killPlayer(mPlayerActor);
}

void PlayerDamageKeeper::reset(IPlayerModelChanger* modelChanger) {
    mIsUpdateModel = false;
    mCurrentHealth = 0;
    mPush = 0;
    if (modelChanger)
        modelChanger->showModel();
}

// NON-MATCHING: https://decomp.me/scratch/TAkbx
void PlayerDamageKeeper::activatePreventDamage() {
    mIsPreventDamage = true;
    mInvincibilityTimer = 1800;
    al::startSe(mPlayerActor, "StartInvincible");
    if (!al::isInAreaObjPlayerOneIgnoreAreaTarget(mPlayerActor->getSceneInfo()->playerHolder,
                                                  "CollectBgmPlayInvalidArea")) {
        al::BgmPlayingRequest request("Invincible");
        *(s64*)&request._24 = 0;
        request._20 = 5;
        request._2c = true;
        al::startBgm(mPlayerActor, request, false, false);
    }
    mPlayerEffect->tryEmitInvincibleEffect();
}

void PlayerDamageKeeper::update(IPlayerModelChanger* modelChanger, bool isRecover,
                                bool isPreventDamage) {
    if (isPreventDamage)
        updatePreventDamage();
    updateKidsModeLifeRecovery(isRecover);

    if (mPush > 0)
        mPush = al::converge(mPush, 0, 1);

    if (mCurrentHealth == 0 || PlayerFunction::isPlayerDeadStatus(mPlayerActor)) {
        if (mIsUpdateModel)
            modelChanger->showModel();
        mIsUpdateModel = false;
        return;
    }

    if (!rs::isModeDiverOrJungleGymRom()) {
        if (mCurrentHealth % 6 < 1)
            modelChanger->hideModel();
        else
            modelChanger->showModel();
    }
    mCurrentHealth = al::converge(mCurrentHealth, 0, 1);
}

void PlayerDamageKeeper::updatePreventDamage() {
    if (mIsPreventDamage) {
        mInvincibilityTimer = al::converge(mInvincibilityTimer, 0, 1);
        if (mInvincibilityTimer < 1) {
            mIsPreventDamage = false;
            if (!al::isInAreaObjPlayerOneIgnoreAreaTarget(
                    mPlayerActor->getSceneInfo()->playerHolder, "CollectBgmPlayInvalidArea")) {
                al::stopBgm(mPlayerActor, "Invincible", 60);
                al::startAndStopBgmInCurPosition(mPlayerActor, false);
            }
            mPlayerEffect->tryDeleteInvincibleEffect();
        }

        if (mInvincibilityTimer == 40)
            al::startSe(mPlayerActor, "EndInvincible");
        al::holdSe(mPlayerActor, "Invincible");
    }

    if (al::isDead(mPlayerActor)) {
        if (!al::isInAreaObjPlayerOneIgnoreAreaTarget(mPlayerActor->getSceneInfo()->playerHolder,
                                                      "CollectBgmPlayInvalidArea")) {
            al::stopBgm(mPlayerActor, "Invincible", 60);
        }
    }
}

void PlayerDamageKeeper::updateKidsModeLifeRecovery(bool isRecover) {
    if (!rs::isKidsMode(mPlayerActor))
        return;
    if (GameDataFunction::isPlayerHitPointMax(mPlayerActor))
        return;

    if (mHitPoint != GameDataFunction::getPlayerHitPoint(mPlayerActor)) {
        resetKidsModeLifeRecovery();
        mKidsModeTimerForAutoHeal = 0;
    }

    mKidsModeTimerForAutoHeal = isRecover ? al::converge(mKidsModeTimerForAutoHeal, 120, 1) : 0;
    s32 minFramesForHeal = 600;
    s32 recoveryCooldown = 0;

    if (isRecover) {
        recoveryCooldown = mRecoveryCooldown;
        if (mKidsModeTimerForAutoHeal >= 120)
            minFramesForHeal = 60;
    } else {
        mRecoveryCooldown = 0;
    }

    mRecoveryCooldown = al::converge(recoveryCooldown, minFramesForHeal, 1);
    if (mRecoveryCooldown >= minFramesForHeal) {
        GameDataFunction::recoveryPlayer(mPlayerActor);
        resetKidsModeLifeRecovery();
    }
}
