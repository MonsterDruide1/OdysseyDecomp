#include "Layout/CoinCollectLayout.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Screen/ScreenFunction.h"

namespace {
NERVE_IMPL(CoinCollectLayout, Start);
NERVE_IMPL(CoinCollectLayout, Wait);
NERVE_IMPL(CoinCollectLayout, End);

NERVES_MAKE_NOSTRUCT(CoinCollectLayout, Start, Wait, End);
}  // namespace

CoinCollectLayout::CoinCollectLayout(const al::LayoutInitInfo& info)
    : al::LayoutActor("コレクトコインレイアウト") {
    al::initLayoutActor(this, info, "PopUpCollectCoin");
    initNerve(&Start);
}

void CoinCollectLayout::appearCounter(s32 maxCoins, s32 currentCoins,
                                      const al::LiveActor* coinActor) {
    mCoinActor = coinActor;
    al::setPaneStringFormat(this, "TxtCollectCoin", "%d/%d", currentCoins, maxCoins);
    al::setNerve(this, &Start);

    // regswap when using updatePos() directly because coinActor is used instead of mCoinActor
    sead::Vector2f layoutPos = sead::Vector2f::zero;
    sead::Vector3f coinPos = al::getTrans(coinActor);
    al::calcLayoutPosFromWorldPos(&layoutPos, coinActor, coinPos);
    al::setLocalTrans(this, layoutPos);

    appear();
}

void CoinCollectLayout::exeStart() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear");
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
    updatePos();
}

void CoinCollectLayout::updatePos() {
    sead::Vector2f layoutPos = sead::Vector2f::zero;
    sead::Vector3f coinPos = al::getTrans(mCoinActor);
    al::calcLayoutPosFromWorldPos(&layoutPos, mCoinActor, coinPos);
    al::setLocalTrans(this, layoutPos);
}

void CoinCollectLayout::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
    if (al::isGreaterEqualStep(this, 120)) {
        al::setNerve(this, &End);
        return;
    }
    updatePos();
}

void CoinCollectLayout::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End");
    if (al::isActionEnd(this)) {
        kill();
        return;
    }
    updatePos();
}
