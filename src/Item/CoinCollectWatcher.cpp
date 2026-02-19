#include "Item/CoinCollectWatcher.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Library/Scene/IUseSceneObjHolder.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Se/SeFunction.h"

#include "Layout/CoinCollectLayout.h"
#include "System/GameDataFunction.h"

CoinCollectWatcher::CoinCollectWatcher() : al::ISceneObj() {}

void CoinCollectWatcher::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    mCoinCollectLayout = new CoinCollectLayout(al::getLayoutInitInfo(info));
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
    s32 coinNumber = GameDataFunction::getCoinCollectGotNum(actor);
    s32 countNumberMax = GameDataFunction::getCoinCollectNumMax(actor);

    mCoinCollectLayout->appearCounter(countNumberMax, coinNumber + 1, al::getPlayerActor(actor, 0));
    if (countNumberMax == coinNumber + 1)
        al::startSe(mCoinCollectLayout, "CountMax");
}

namespace rs {
CoinCollectWatcher* createCoinCollectWatcher(const al::IUseSceneObjHolder* objHolder) {
    return al::createSceneObj<CoinCollectWatcher>(objHolder);
}
}  // namespace rs
