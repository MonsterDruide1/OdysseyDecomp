#include "Item/CoinCollectWatcher.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Scene/IUseSceneObjHolder.h"
#include "Library/Se/SeFunction.h"

#include "Layout/CoinCollectLayout.h"
#include "System/GameDataFunction.h"

CoinCollectWatcher::CoinCollectWatcher() : al::ISceneObj() {}

const char* CoinCollectWatcher::getSceneObjName() const {
    return "コインコレクト監視者";
}

void CoinCollectWatcher::initAfterPlacementSceneObj(const al::ActorInitInfo& initInfo) {
    mCoinCollectLayout = new CoinCollectLayout(*al::getLayoutInitInfo(initInfo));
}

void CoinCollectWatcher::registerCoin(bool isCountUpCoin) {
    if (isCountUpCoin) {
        mCoinUpCount++;
        mRegisteredCoins++;
        return;
    }
    mRegisteredCoins++;
}

void CoinCollectWatcher::countup(const al::LiveActor* actor) {
    mCoinUpCount++;
    const s32 coinNumber = GameDataFunction::getCoinCollectGotNum(GameDataHolderAccessor(actor));
    const s32 countNumberMax =
        GameDataFunction::getCoinCollectNumMax(GameDataHolderAccessor(actor));

    mCoinCollectLayout->appearCounter(countNumberMax, coinNumber + 1, al::getPlayerActor(actor, 0));
    if (countNumberMax == coinNumber + 1)
        al::startSe(mCoinCollectLayout, "CountMax");
}

namespace rs {
CoinCollectWatcher* createCoinCollectWatcher(const al::IUseSceneObjHolder* objHolder) {
    return (CoinCollectWatcher*)al::createSceneObj(objHolder, 8);
}
}  // namespace rs
