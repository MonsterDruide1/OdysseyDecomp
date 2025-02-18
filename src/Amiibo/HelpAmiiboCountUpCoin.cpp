#include "Amiibo/HelpAmiiboCountUpCoin.h"

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Nfp/NfpFunction.h"

#include "Amiibo/HelpAmiiboFunction.h"
#include "Item/Coin.h"
#include "Item/Coin2D.h"
#include "Item/LifeUpItem.h"
#include "Item/LifeUpItem2D.h"
#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(HelpAmiiboCountUpCoin, End);
NERVE_IMPL(HelpAmiiboCountUpCoin, Delay);
NERVE_IMPL(HelpAmiiboCountUpCoin, AppearCoin);

NERVES_MAKE_NOSTRUCT(HelpAmiiboCountUpCoin, AppearCoin);
NERVES_MAKE_STRUCT(HelpAmiiboCountUpCoin, End, Delay);
}  // namespace

HelpAmiiboCountUpCoin::HelpAmiiboCountUpCoin(HelpAmiiboDirector* director, al::LiveActor* actor)
    : HelpAmiiboExecutor(director, actor, "ランダムアイテムお助け") {
    mNerveKeeper = new al::NerveKeeper(this, &NrvHelpAmiiboCountUpCoin.End, 0);
}

void HelpAmiiboCountUpCoin::initAfterPlacement(const al::ActorInitInfo& initInfo) {
    HelpAmiiboExecutor::initAfterPlacement(initInfo);
    mLifeUpItem = new LifeUpItem("ライフアップアイテム[amiibo]");
    al::initCreateActorNoPlacementInfo(mLifeUpItem, initInfo);
    mLifeUpItem->setBool145(true);
    mLifeUpItem->makeActorDead();

    mLifeUpItem2D = new LifeUpItem2D("ライフアップアイテム2D[amiibo]");
    al::initCreateActorNoPlacementInfo(mLifeUpItem2D, initInfo);
    mLifeUpItem2D->makeActorDead();
    mLifeUpItem2D->setBool130(true);
    mCoinBuffer.allocBuffer(3, nullptr);
    mCoin2DBuffer.allocBuffer(3, nullptr);

    for (u32 i = 0; i < 3; i++) {
        Coin* coin = new Coin("コイン[amiibo]", false);
        al::initCreateActorNoPlacementInfo(coin, initInfo);
        coin->makeActorDead();

        Coin2D* coin2D = new Coin2D("コイン2D[amiibo]");
        al::initCreateActorNoPlacementInfo(coin2D, initInfo);
        coin2D->makeActorDead();

        mCoinBuffer.pushBack(coin);
        mCoin2DBuffer.pushBack(coin2D);
    }
}

bool HelpAmiiboCountUpCoin::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    if (!al::isCharacterIdBaseKoopa(nfpInfo) && !al::isCharacterIdBasePeach(nfpInfo) &&
        !al::isCharacterIdBaseMario(nfpInfo))
        return true;

    return false;
}

bool HelpAmiiboCountUpCoin::isEnableUse() {
    return !HelpAmiiboFunction::calcIsHitArrowItemAppearPos(getActor());
}

bool HelpAmiiboCountUpCoin::execute() {
    getNerveKeeper()->update();
    return al::isNerve(this, &NrvHelpAmiiboCountUpCoin.End);
}

void HelpAmiiboCountUpCoin::activate() {
    HelpAmiiboExecutor::activate();

    mNumberOfCoins = al::getRandom(4);
    if (mNumberOfCoins != 0) {
        mCoinsSpawned = 0;
        al::setNerve(this, &NrvHelpAmiiboCountUpCoin.Delay);
        return;
    }

    al::LiveActor* actor = getActor();
    sead::Vector3f offset = sead::Vector3f::zero;
    HelpAmiiboFunction::calcLifeUpItemAppearOffset(&offset, actor);

    if (rs::isPlayer2D(getActor())) {
        al::resetPosition(mLifeUpItem2D, rs::getPlayerPos(actor) + offset);
        mLifeUpItem2D->appearAmiiboTouch();
    } else {
        if (!rs::isPlayerHack(actor)) {
            al::resetPosition(mLifeUpItem, rs::getPlayerPos(actor) + offset);
        } else {
            sead::Vector3f basePos = sead::Vector3f::zero;
            rs::calcPlayerAmiiboPeachAppearBasePos(&basePos, actor);
            al::resetPosition(mLifeUpItem);
        }
        mLifeUpItem->appearAmiiboTouch();
    }

    al::setNerve(this, &NrvHelpAmiiboCountUpCoin.End);
}

HelpAmiiboType HelpAmiiboCountUpCoin::getType() const {
    return HelpAmiiboType::CountUpCoin;
}

al::NerveKeeper* HelpAmiiboCountUpCoin::getNerveKeeper() const {
    return mNerveKeeper;
}

void HelpAmiiboCountUpCoin::updateItemQT(al::LiveActor* actor) {
    sead::Quatf pose = sead::Quatf::unit;
    sead::Vector3f position = sead::Vector3f::zero;

    HelpAmiiboFunction::calcCoinAppearQT(&pose, &position, actor);

    al::updatePoseQuat(actor, pose);
    al::resetPosition(actor, position);
}

void HelpAmiiboCountUpCoin::exeEnd() {}

void HelpAmiiboCountUpCoin::exeDelay() {
    if (al::isGreaterEqualStep(this, 60))
        al::setNerve(this, &AppearCoin);
}

void HelpAmiiboCountUpCoin::exeAppearCoin() {
    if (al::getNerveStep(this) % 10 != 0)
        return;

    if (rs::isPlayer2D(getActor())) {
        Coin2D* coin2D = mCoin2DBuffer[mCoinsSpawned];
        updateItemQT(coin2D);
        coin2D->appearCountUp();

    } else {
        Coin* coin = mCoinBuffer[mCoinsSpawned];
        updateItemQT(coin);
        coin->appearCountUp();
    }

    s32 mLastCoinIndex = (mNumberOfCoins == 1) ? 0 :
                         (mNumberOfCoins == 2) ? 1 :
                         (mNumberOfCoins == 3) ? 2 :
                                                 -1;

    if ((s32)mCoinsSpawned == mLastCoinIndex) {
        al::setNerve(this, &NrvHelpAmiiboCountUpCoin.End);
        return;
    }

    mCoinsSpawned++;
}
