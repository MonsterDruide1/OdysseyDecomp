#include "Amiibo/HelpAmiiboNavigateCoinCollect.h"

#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nfp/NfpFunction.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/HelpAmiiboDirector.h"
#include "Item/CoinCollect.h"
#include "Item/CoinCollectHolder.h"
#include "ModeBalloon/TimeBalloonHintArrow.h"

HelpAmiiboNavigateCoinCollect::HelpAmiiboNavigateCoinCollect(HelpAmiiboDirector* director,
                                                             al::LiveActor* actor)
    : HelpAmiiboExecutor(director, actor, "コレクトコイン誘導お助け") {}

void HelpAmiiboNavigateCoinCollect::initAfterPlacement(const al::ActorInitInfo& initInfo) {
    HelpAmiiboExecutor::initAfterPlacement(initInfo);
    mTimeBalloonHintArrow = new TimeBalloonHintArrow();
    mTimeBalloonHintArrow->init(initInfo);
    mTimeBalloonHintArrow->makeActorDead();
}

bool HelpAmiiboNavigateCoinCollect::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    return al::isCharacterIdBaseKinopio(nfpInfo);
}

void HelpAmiiboNavigateCoinCollect::activate() {
    HelpAmiiboExecutor::activate();
    CoinCollect* coinCollect = al::getSceneObj<CoinCollectHolder>(getActor())
                                   ->tryFindAliveCoinCollect(sead::Vector3f::zero, true);

    if (coinCollect == nullptr) {
        al::startSe(getDirector(), "AmiiboKoopa");
        return;
    }

    al::startSe(getDirector(), "AmiiboMario");
    mHintedCoinCollect = coinCollect;
    mTimeBalloonHintArrow->updateRotate(al::getTrans(coinCollect));
    mTimeBalloonHintArrow->appear();
}

bool HelpAmiiboNavigateCoinCollect::execute() {
    if (mHintedCoinCollect == nullptr)
        return true;

    if (al::isDead(mHintedCoinCollect)) {
        mTimeBalloonHintArrow->kill();
        return true;
    }

    mTimeBalloonHintArrow->updateRotate(al::getTrans(mHintedCoinCollect));
    return false;
}

HelpAmiiboType HelpAmiiboNavigateCoinCollect::getType() const {
    return HelpAmiiboType::Koopa;
}
