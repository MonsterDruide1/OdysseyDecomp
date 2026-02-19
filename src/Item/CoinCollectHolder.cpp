#include "Item/CoinCollectHolder.h"

#include "Library/Area/AreaObjUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Item/CoinCollect.h"
#include "Item/CoinCollect2D.h"
#include "MapObj/CoinCollectHintObj.h"
#include "Util/PlayerUtil.h"

CoinCollectHolder::CoinCollectHolder() {
    mCollect.allocBuffer(100, nullptr);
    mCollect2D.allocBuffer(100, nullptr);
    mCollectHintObj.allocBuffer(10, nullptr);
}

void CoinCollectHolder::registerCoinCollect(CoinCollect* coin) {
    mCollect.pushBack(coin);
}

void CoinCollectHolder::registerCoinCollect2D(CoinCollect2D* coin) {
    mCollect2D.pushBack(coin);
}

void CoinCollectHolder::registerHintObj(CoinCollectHintObj* hintObj) {
    mCollectHintObj.pushBack(hintObj);
}

CoinCollect* CoinCollectHolder::tryFindAliveCoinCollect(const sead::Vector3f& playerPos,
                                                        bool checkAreaObj) const {
    s32 size = mCollect.size();
    if (size <= 0)
        return nullptr;

    al::AreaObj* playerArea = al::tryFindAreaObj(mCollect[0], "InvalidateStageMapArea", playerPos);

    CoinCollect* selectedCoin = nullptr;
    f32 minDistance = sead::Mathf::maxNumber();

    for (s32 i = 0; i < size; i++) {
        CoinCollect* coin = mCollect[i];

        if (!al::isAlive(coin))
            continue;

        if (checkAreaObj && playerArea) {
            al::AreaObj* coinArea =
                al::tryFindAreaObj(coin, "InvalidateStageMapArea", al::getTrans(coin));
            if (!playerArea)
                continue;
            if (!coinArea || coinArea != playerArea)
                continue;
        }

        f32 distance = (playerPos - al::getTrans(coin)).length();
        if (minDistance > distance) {
            selectedCoin = coin;
            minDistance = distance;
        }
    }
    return selectedCoin;
}

CoinCollect* CoinCollectHolder::tryFindAliveCoinCollect(const sead::Vector3f& playerPos,
                                                        f32 minRange, f32 maxRange,
                                                        bool checkAreaObj) const {
    s32 size = mCollect.size();
    if (size <= 0)
        return nullptr;

    al::AreaObj* playerArea = al::tryFindAreaObj(mCollect[0], "InvalidateStageMapArea", playerPos);

    CoinCollect* selectedCoin = nullptr;
    f32 minDistance = sead::Mathf::maxNumber();

    for (s32 i = 0; i < size; i++) {
        CoinCollect* coin = mCollect[i];

        if (!al::isAlive(coin))
            continue;

        if (checkAreaObj && playerArea) {
            al::AreaObj* coinArea =
                al::tryFindAreaObj(coin, "InvalidateStageMapArea", al::getTrans(coin));
            if (coinArea && coinArea != playerArea)
                continue;
        }

        f32 distance = (playerPos - al::getTrans(coin)).length();
        if (distance > minRange && distance < maxRange && minDistance > distance) {
            selectedCoin = coin;
            minDistance = distance;
        }
    }
    return selectedCoin;
}

CoinCollect* CoinCollectHolder::tryFindDeadButHintEnableCoinCollect() const {
    s32 size = mCollect.size();
    if (size == 0)
        return nullptr;

    const sead::Vector3f& playerPos = rs::getPlayerPos(mCollect[0]);
    al::AreaObj* playerArea = al::tryFindAreaObj(mCollect[0], "InvalidateStageMapArea", playerPos);

    for (s32 i = 0; i < size; i++) {
        if (!al::isAlive(mCollect[i]) &&
            al::tryFindAreaObj(mCollect[i], "InvalidateStageMapArea",
                               mCollect[i]->getTransForHint()) == playerArea &&
            mCollect[i]->isHintEnabled()) {
            return mCollect[i];
        }
    }
    return nullptr;
}

CoinCollect2D* CoinCollectHolder::tryFindAliveCoinCollect2D(const sead::Vector3f& playerPos,
                                                            bool checkAreaObj) const {
    s32 size = mCollect2D.size();
    if (size <= 0)
        return nullptr;

    al::AreaObj* playerArea =
        al::tryFindAreaObj(mCollect2D[0], "InvalidateStageMapArea", playerPos);

    CoinCollect2D* selectedCoin = nullptr;
    f32 minDistance = sead::Mathf::maxNumber();

    for (s32 i = 0; i < size; i++) {
        CoinCollect2D* coin = mCollect2D[i];

        if (!al::isAlive(coin))
            continue;

        if (checkAreaObj && playerArea) {
            al::AreaObj* coinArea =
                al::tryFindAreaObj(coin, "InvalidateStageMapArea", al::getTrans(coin));
            if (!playerArea)
                continue;
            if (!coinArea || coinArea != playerArea)
                continue;
        }

        f32 distance = (playerPos - al::getTrans(coin)).length();
        if (minDistance > distance) {
            selectedCoin = coin;
            minDistance = distance;
        }
    }
    return selectedCoin;
}

CoinCollect2D* CoinCollectHolder::tryFindAliveCoinCollect2D(const sead::Vector3f& playerPos,
                                                            f32 minRange, f32 maxRange,
                                                            bool checkAreaObj) const {
    s32 size = mCollect2D.size();
    if (size <= 0)
        return nullptr;

    al::AreaObj* playerArea =
        al::tryFindAreaObj(mCollect2D[0], "InvalidateStageMapArea", playerPos);

    CoinCollect2D* selectedCoin = nullptr;
    f32 minDistance = sead::Mathf::maxNumber();

    for (s32 i = 0; i < size; i++) {
        CoinCollect2D* coin = mCollect2D[i];

        if (!al::isAlive(coin))
            continue;

        if (checkAreaObj && playerArea) {
            al::AreaObj* coinArea =
                al::tryFindAreaObj(coin, "InvalidateStageMapArea", al::getTrans(coin));
            if (coinArea && coinArea != playerArea)
                continue;
        }

        f32 distance = (playerPos - al::getTrans(coin)).length();
        if (distance > minRange && distance < maxRange && minDistance > distance) {
            selectedCoin = coin;
            minDistance = distance;
        }
    }
    return selectedCoin;
}

bool CoinCollectHolder::tryFindExStageHintObjTrans(sead::Vector3f* outTrans,
                                                   const char* stageName) {
    s32 size = mCollectHintObj.size();
    for (s32 i = 0; i < size; i++) {
        if (al::isEqualString(mCollectHintObj[i]->getStageName(), stageName)) {
            outTrans->set(mCollectHintObj[i]->getTrans());
            return true;
        }
    }
    return false;
}

const char* CoinCollectHolder::getSceneObjName() const {
    return "コレクトコイン保持者";
}

namespace rs {
CoinCollectHolder* createCoinCollectHolder(const al::IUseSceneObjHolder* objHolder) {
    return al::createSceneObj<CoinCollectHolder>(objHolder);
}
}  // namespace rs
