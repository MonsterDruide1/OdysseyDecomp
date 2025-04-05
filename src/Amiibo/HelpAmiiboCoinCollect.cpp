#include "Amiibo/HelpAmiiboCoinCollect.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Area/IUseAreaObj.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nfp/NfpFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Se/SeFunction.h"

#include "Amiibo/HelpAmiiboDirector.h"
#include "Item/CoinCollect.h"
#include "Item/CoinCollect2D.h"
#include "Item/CoinCollectDummy.h"
#include "Item/CoinCollectHolder.h"
#include "System/GameDataFunction.h"
#include "Util/PlayerUtil.h"

HelpAmiiboCoinCollect::HelpAmiiboCoinCollect(HelpAmiiboDirector* director,
                                             al::LiveActor* amiiboActor)
    : HelpAmiiboExecutor(director, amiiboActor, "コレクトコインお助け") {}

void HelpAmiiboCoinCollect::initAfterPlacement(const al::ActorInitInfo& actorInitInfo) {
    HelpAmiiboExecutor::initAfterPlacement(actorInitInfo);

    mCoinCollectDummy = new CoinCollectDummy("コレクトコインお助け");
    al::initCreateActorNoPlacementInfo(mCoinCollectDummy, actorInitInfo);
}

bool HelpAmiiboCoinCollect::isTriggerTouch(const al::NfpInfo& nfpInfo) const {
    return al::isCharacterIdBaseKoopa(nfpInfo);
}

bool HelpAmiiboCoinCollect::isEnableUse() {
    CoinCollectHolder* coinHolder = al::tryGetSceneObj<CoinCollectHolder>(getActor());
    if (coinHolder == nullptr)
        return false;

    if (mCoinCollect2D != nullptr && al::isAlive(mCoinCollect2D))
        return false;

    if (mCoinCollect != nullptr && al::isAlive(mCoinCollect))
        return false;

    const sead::Vector3f& playerPos = rs::getPlayerPos(getActor());
    if (coinHolder->tryFindAliveCoinCollect(playerPos, true) != nullptr)
        return true;
    if (coinHolder->tryFindAliveCoinCollect2D(playerPos, true) != nullptr)
        return true;
    if (coinHolder->tryFindDeadButHintEnableCoinCollect() != nullptr)
        return true;

    if (al::tryFindAreaObj(mCoinCollectDummy, "InvalidateStageMapArea", playerPos) == nullptr) {
        sead::Vector3f stagePos = sead::Vector3f::zero;
        const char* stageName = nullptr;
        if (GameDataFunction::tryFindExistCoinCollectStagePosExcludeHomeStageInCurrentWorld(
                &stagePos, &stageName, getActor())) {
            if (GameDataFunction::isMainStage(mCoinCollectDummy) ||
                coinHolder->tryFindExStageHintObjTrans(&stagePos, stageName))
                return true;
        }
    }

    return false;
}

bool HelpAmiiboCoinCollect::execute() {
    const sead::Vector3f& playerPos = rs::getPlayerPos(getActor());

    al::AreaObj* areaObj =
        al::tryFindAreaObj(mCoinCollectDummy, "InvalidateStageMapArea", playerPos);
    if (areaObj != mStartInvalidateStageMapArea) {
        deleteHintEffect();
        return true;
    }

    if (mCoinCollect != nullptr) {
        if (mCoinCollect->isEnableHint()) {
            mCoinCollect = nullptr;
            return true;
        }
        return false;
    }

    if (mCoinCollect2D != nullptr) {
        if (mCoinCollect2D->isEnableHint()) {
            mCoinCollect2D = nullptr;
            return true;
        }
        return false;
    }

    return !al::isAlive(mCoinCollectDummy);
}

void HelpAmiiboCoinCollect::activate() {
    HelpAmiiboExecutor::activate();

    al::LiveActor* actor = getActor();
    const sead::Vector3f& playerPos = rs::getPlayerPos(actor);
    CoinCollectHolder* coinHolder = al::tryGetSceneObj<CoinCollectHolder>(actor);
    mStartInvalidateStageMapArea =
        al::tryFindAreaObj(mCoinCollectDummy, "InvalidateStageMapArea", playerPos);

    CoinCollect* aliveCoinCollect =
        coinHolder->tryFindAliveCoinCollect(playerPos, 5000.0f, 10000.0f, true);
    if (aliveCoinCollect != nullptr) {
        al::startSe(getDirector(), "AmiiboKoopa");
        if (isUseDummyModel(aliveCoinCollect)) {
            sead::Vector3f dummyEffectEmitPos = sead::Vector3f::zero;
            getDummyEffectEmitPos(&dummyEffectEmitPos, aliveCoinCollect);
            mCoinCollectDummy->appearHint(dummyEffectEmitPos);
            return;
        }
        aliveCoinCollect->appearHelpAmiiboEffect();
        mCoinCollect = aliveCoinCollect;
        return;
    }

    CoinCollect2D* aliveCoinCollect2D =
        coinHolder->tryFindAliveCoinCollect2D(playerPos, 5000.0f, 10000.0f, true);
    if (aliveCoinCollect2D != nullptr) {
        al::startSe(getDirector(), "AmiiboKoopa");
        if (isUseDummyModel(aliveCoinCollect2D)) {
            sead::Vector3f dummyEffectEmitPos = sead::Vector3f::zero;
            getDummyEffectEmitPos(&dummyEffectEmitPos, aliveCoinCollect2D);
            mCoinCollectDummy->appearHint(dummyEffectEmitPos);
            return;
        }
        aliveCoinCollect2D->appearHintEffect();
        mCoinCollect2D = aliveCoinCollect2D;
        return;
    }

    CoinCollect* coinCollect = coinHolder->tryFindAliveCoinCollect(playerPos, true);
    if (coinCollect != nullptr) {
        al::startSe(getDirector(), "AmiiboKoopa");
        if (isUseDummyModel(coinCollect)) {
            sead::Vector3f dummyEffectEmitPos = sead::Vector3f::zero;
            getDummyEffectEmitPos(&dummyEffectEmitPos, coinCollect);
            mCoinCollectDummy->appearHint(dummyEffectEmitPos);
            return;
        }
        coinCollect->appearHelpAmiiboEffect();
        mCoinCollect = coinCollect;
        return;
    }

    CoinCollect2D* coinCollect2D = coinHolder->tryFindAliveCoinCollect2D(playerPos, true);
    if (coinCollect2D != nullptr) {
        al::startSe(getDirector(), "AmiiboKoopa");
        if (isUseDummyModel(coinCollect2D)) {
            sead::Vector3f dummyEffectEmitPos = sead::Vector3f::zero;
            getDummyEffectEmitPos(&dummyEffectEmitPos, coinCollect2D);
            mCoinCollectDummy->appearHint(dummyEffectEmitPos);
            return;
        }
        coinCollect2D->appearHintEffect();
        mCoinCollect2D = coinCollect2D;
        return;
    }

    CoinCollect* deadCoinCollect = coinHolder->tryFindDeadButHintEnableCoinCollect();
    if (deadCoinCollect != nullptr) {
        al::startSe(getDirector(), "AmiiboKoopa");
        if (isUseDummyModel(deadCoinCollect)) {
            sead::Vector3f dummyEffectEmitPos = sead::Vector3f::zero;
            getDummyEffectEmitPos(&dummyEffectEmitPos, deadCoinCollect);
            mCoinCollectDummy->appearHint(dummyEffectEmitPos);
            return;
        }
        deadCoinCollect->appearHelpAmiiboEffect();
        mCoinCollect = deadCoinCollect;
        return;
    }

    if (al::tryFindAreaObj(mCoinCollectDummy, "InvalidateStageMapArea", playerPos) == nullptr) {
        sead::Vector3f stagePos = sead::Vector3f::zero;
        const char* stageName = nullptr;
        if (GameDataFunction::tryFindExistCoinCollectStagePosExcludeHomeStageInCurrentWorld(
                &stagePos, &stageName, actor)) {
            if (GameDataFunction::isMainStage(actor) ||
                coinHolder->tryFindExStageHintObjTrans(&stagePos, stageName)) {
                mCoinCollectDummy->appearHint(stagePos);
                al::startSe(getDirector(), "AmiiboKoopa");
            }
        }
    }
}

HelpAmiiboType HelpAmiiboCoinCollect::getType() const {
    return HelpAmiiboType::Koopa;
}

void HelpAmiiboCoinCollect::deleteHintEffect() {
    if (mCoinCollect != nullptr) {
        mCoinCollect->deleteHelpAmiiboEffect();
        mCoinCollect = nullptr;
        return;
    }

    if (mCoinCollect2D != nullptr) {
        mCoinCollect2D->deleteHintEffect();
        mCoinCollect2D = nullptr;
        return;
    }

    if (al::isAlive(mCoinCollectDummy))
        mCoinCollectDummy->kill();
}

void HelpAmiiboCoinCollect::appearEffect() {
    if (mCoinCollect != nullptr) {
        mCoinCollect->reappearHelpAmiiboEffect();
        return;
    }

    if (mCoinCollect2D != nullptr) {
        mCoinCollect2D->reappearHintEffect();
        return;
    }

    if (al::isAlive(mCoinCollectDummy))
        mCoinCollectDummy->reappearHint();
}

void HelpAmiiboCoinCollect::killEffect() {
    if (mCoinCollect != nullptr) {
        mCoinCollect->deleteHelpAmiiboEffect();
        return;
    }

    if (mCoinCollect2D != nullptr)
        mCoinCollect2D->deleteHintEffect();

    if (al::isAlive(mCoinCollectDummy))
        mCoinCollectDummy->deleteHint();
}

bool HelpAmiiboCoinCollect::isUseDummyModel(al::LiveActor* actor) {
    const sead::Vector3f& playerPos = rs::getPlayerPos(mCoinCollectDummy);
    al::AreaObj* dummyAreaObj =
        al::tryFindAreaObj(mCoinCollectDummy, "InvalidateStageMapArea", playerPos);
    al::AreaObj* actorAreaObj =
        al::tryFindAreaObj(actor, "InvalidateStageMapArea", al::getTrans(actor));

    return dummyAreaObj != actorAreaObj;
}

void HelpAmiiboCoinCollect::getDummyEffectEmitPos(sead::Vector3f* position, al::LiveActor* actor) {
    al::AreaObj* areaObj = al::tryFindAreaObj(actor, "InvalidateStageMapArea", al::getTrans(actor));
    if (areaObj != nullptr)
        al::tryGetLinksTrans(position, areaObj->getPlacementInfo(), "PlayerPoint");
}
