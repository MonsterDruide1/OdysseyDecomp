#include "Amiibo/HelpAmiiboFallCoin.h"

#include <math/seadVector.h>

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Nfp/NfpFunction.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/HelpAmiiboDirector.h"
#include "Item/Coin.h"

namespace {
NERVE_IMPL(HelpAmiiboFallCoin, Wait)
NERVE_IMPL(HelpAmiiboFallCoin, Fall)

NERVES_MAKE_NOSTRUCT(HelpAmiiboFallCoin, Wait, Fall)
}  // namespace

HelpAmiiboFallCoin::HelpAmiiboFallCoin(HelpAmiiboDirector* director, al::LiveActor* actor)
    : HelpAmiiboExecutor(director, actor, "コレクトコインお助け") {}

void HelpAmiiboFallCoin::initAfterPlacement(const al::ActorInitInfo& initInfo) {
    HelpAmiiboExecutor::initAfterPlacement(initInfo);
    mNerveKeeper = new al::NerveKeeper(this, &Wait, 0);

    mCoinBuffer.allocBuffer(3, nullptr);
    for (s32 i = 0; i < 3; i++) {
        Coin* coin = new Coin("コイン[amiibo]", false);
        al::initCreateActorNoPlacementInfo(coin, initInfo);
        coin->makeActorDead();
        mCoinBuffer.pushBack(coin);
    }
}

bool HelpAmiiboFallCoin::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    return al::isCharacterIdBaseKinopio(nfpInfo);
}

bool HelpAmiiboFallCoin::isEnableUse() {
    return true;
}

void HelpAmiiboFallCoin::activate() {
    HelpAmiiboExecutor::activate();
    al::setNerve(this, &Fall);
    al::startSe(getDirector(), "AmiiboPeach");
}

bool HelpAmiiboFallCoin::execute() {
    getNerveKeeper()->update();
    for (s32 i = 0; i < 3; i++)
        if (!al::isDead(mCoinBuffer[i]))
            return false;
    return true;
}

al::NerveKeeper* HelpAmiiboFallCoin::getNerveKeeper() const {
    return mNerveKeeper;
}

void HelpAmiiboFallCoin::exeWait() {}

void HelpAmiiboFallCoin::exeFall() {
    if (al::isFirstStep(this)) {
        for (s32 i = 0; i < 3; i++) {
            Coin* coin = mCoinBuffer[i];
            sead::Vector3f trans = al::getTrans(getActor());
            al::resetPosition(coin, trans);
            coin->appearPopUp();
        }
    }

    al::setNerve(this, &Wait);
}
