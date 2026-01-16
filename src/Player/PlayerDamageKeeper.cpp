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
    : mPlayer(actor), mPlayerEffect(playerEffect) {}

void PlayerDamageKeeper::invalidate(s32 damageInvalidCount) {
    mIsUpdateModel = true;
    mDamageInvalidCount = sead::Mathi::clampMin(mDamageInvalidCount, damageInvalidCount);
}

void PlayerDamageKeeper::invalidateIncludePush(s32 damageInvalidCountAbyss) {
    invalidate(damageInvalidCountAbyss);
    mDamageInvalidCountAbyss = damageInvalidCountAbyss;
}

void PlayerDamageKeeper::damage(s32 damageInvalidCount) {
    mIsUpdateModel = true;
    mDamageInvalidCount = damageInvalidCount;
    if (mIsPreventDamage)
        return;

    GameDataFunction::damagePlayer(mPlayer);
    resetKidsModeLifeRecovery();
    tryShowKidsModeLifeOneCapMessage();
}

void PlayerDamageKeeper::resetKidsModeLifeRecovery() {
    mKidsModeHealth = GameDataFunction::getPlayerHitPoint(mPlayer);
    mKidsModeRecoveryCooldown = 0;
    if (mKidsModeStandStillCounter < 120)
        mKidsModeStandStillCounter = 0;
}

bool PlayerDamageKeeper::tryShowKidsModeLifeOneCapMessage() {
    if (rs::isKidsMode(mPlayer) && GameDataFunction::getPlayerHitPoint(mPlayer) == 1 &&
        !rs::isSceneStatusBossBattle(mPlayer) && rs::isEnableCapMessageLifeOneKidsMode(mPlayer) &&
        !rs::isShowCapMsgPlayerLife1(mPlayer)) {
        rs::showCapMsgPlayerLife1(mPlayer, 100);
        rs::disableCapMessageLifeOneKidsMode(mPlayer);
        return true;
    }
    return false;
}

void PlayerDamageKeeper::damageForce(s32 damageInvalidCount) {
    mIsUpdateModel = true;
    mDamageInvalidCount = damageInvalidCount;

    GameDataFunction::damagePlayer(mPlayer);
    resetKidsModeLifeRecovery();
    tryShowKidsModeLifeOneCapMessage();
}

void PlayerDamageKeeper::dead() {
    GameDataFunction::killPlayer(mPlayer);
}

void PlayerDamageKeeper::reset(IPlayerModelChanger* modelChanger) {
    mIsUpdateModel = false;
    mDamageInvalidCount = 0;
    mDamageInvalidCountAbyss = 0;
    if (modelChanger)
        modelChanger->showModel();
}

void PlayerDamageKeeper::activatePreventDamage() {
    mIsPreventDamage = true;
    mInvincibilityTimer = 1800;
    al::startSe(mPlayer, "StartInvincible");
    if (!al::isInAreaObjPlayerOneIgnoreAreaTarget(mPlayer->getSceneInfo()->playerHolder,
                                                  "CollectBgmPlayInvalidArea")) {
        // TODO: cleanup when more examples of `BgmPlayingRequest` exist
        al::BgmPlayingRequest request("Invincible");
        *(s64*)&request._24 = 0;
        request._20 = 5;
        request._2c = true;
        al::startBgm(mPlayer, request, false, false);
    }
    mPlayerEffect->tryEmitInvincibleEffect();
}

void PlayerDamageKeeper::update(IPlayerModelChanger* modelChanger, bool isRecover,
                                bool isPreventDamage) {
    if (isPreventDamage)
        updatePreventDamage();
    updateKidsModeLifeRecovery(isRecover);

    if (mDamageInvalidCountAbyss > 0)
        mDamageInvalidCountAbyss = al::converge(mDamageInvalidCountAbyss, 0, 1);

    if (mDamageInvalidCount == 0 || PlayerFunction::isPlayerDeadStatus(mPlayer)) {
        if (mIsUpdateModel)
            modelChanger->showModel();
        mIsUpdateModel = false;
        return;
    }

    if (!rs::isModeDiverOrJungleGymRom()) {
        if (mDamageInvalidCount % 6 < 1)
            modelChanger->hideModel();
        else
            modelChanger->showModel();
    }
    mDamageInvalidCount = al::converge(mDamageInvalidCount, 0, 1);
}

void PlayerDamageKeeper::updatePreventDamage() {
    if (mIsPreventDamage) {
        mInvincibilityTimer = al::converge(mInvincibilityTimer, 0, 1);
        if (mInvincibilityTimer < 1) {
            mIsPreventDamage = false;
            if (!al::isInAreaObjPlayerOneIgnoreAreaTarget(mPlayer->getSceneInfo()->playerHolder,
                                                          "CollectBgmPlayInvalidArea")) {
                al::stopBgm(mPlayer, "Invincible", 60);
                al::startAndStopBgmInCurPosition(mPlayer, false);
            }
            mPlayerEffect->tryDeleteInvincibleEffect();
        }

        if (mInvincibilityTimer == 40)
            al::startSe(mPlayer, "EndInvincible");
        al::holdSe(mPlayer, "Invincible");
    }

    if (al::isDead(mPlayer)) {
        if (!al::isInAreaObjPlayerOneIgnoreAreaTarget(mPlayer->getSceneInfo()->playerHolder,
                                                      "CollectBgmPlayInvalidArea")) {
            al::stopBgm(mPlayer, "Invincible", 60);
        }
    }
}

void PlayerDamageKeeper::updateKidsModeLifeRecovery(bool isRecover) {
    if (!rs::isKidsMode(mPlayer))
        return;
    if (GameDataFunction::isPlayerHitPointMax(mPlayer))
        return;

    if (mKidsModeHealth != GameDataFunction::getPlayerHitPoint(mPlayer)) {
        resetKidsModeLifeRecovery();
        mKidsModeStandStillCounter = 0;
    }

    mKidsModeStandStillCounter = isRecover ? al::converge(mKidsModeStandStillCounter, 120, 1) : 0;
    s32 minFramesForHeal = 600;
    s32 recoveryCooldown = 0;

    if (isRecover) {
        recoveryCooldown = mKidsModeRecoveryCooldown;
        if (mKidsModeStandStillCounter >= 120)
            minFramesForHeal = 60;
    } else {
        mKidsModeRecoveryCooldown = 0;
    }

    mKidsModeRecoveryCooldown = al::converge(recoveryCooldown, minFramesForHeal, 1);
    if (mKidsModeRecoveryCooldown >= minFramesForHeal) {
        GameDataFunction::recoveryPlayer(mPlayer);
        resetKidsModeLifeRecovery();
    }
}
