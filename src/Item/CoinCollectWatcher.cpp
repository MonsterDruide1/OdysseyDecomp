#include "Item/CoinCollectWatcher.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Scene/IUseSceneObjHolder.h"
#include "Library/Se/SeFunction.h"

#include "Layout/CoinCollectLayout.h"
#include "System/GameDataFunction.h"

CoinCollectWatcher::CoinCollectWatcher() : al::ISceneObj() {}

CoinCollectWatcher::~CoinCollectWatcher() = default;

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
    GameDataHolderAccessor gameData(actor);
    s32 coinNumber = GameDataFunction::getCoinCollectGotNum(gameData);
    GameDataHolderAccessor gameData2(actor);
    s32 counNumberMax = GameDataFunction::getCoinCollectNumMax(gameData2);

    mCoinCollectLayout->appearCounter(counNumberMax, coinNumber + 1, al::getPlayerActor(actor, 0));
    if (counNumberMax == coinNumber + 1)
        al::startSe(mCoinCollectLayout, "CountMax");
}

namespace rs {
CoinCollectWatcher* createCoinCollectWatcher(const al::IUseSceneObjHolder* objHolder) {
    return (CoinCollectWatcher*)al::createSceneObj(objHolder, 8);
}
}  // namespace rs
