#include "Item/CoinCollectDummy.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Item/CoinCollectHintState.h"
#include "Util/ItemUtil.h"

namespace {
NERVE_IMPL(CoinCollectDummy, Hint);

struct {
    NERVE_MAKE(CoinCollectDummy, Hint);
} NrvCoinCollectDummy;

}  // namespace

CoinCollectDummy::CoinCollectDummy(const char* name) : al::LiveActor(name) {}

void CoinCollectDummy::init(const al::ActorInitInfo& initInfo) {
    al::initActorSceneInfo(this, initInfo);
    al::initActorWithArchiveName(this, initInfo, rs::getStageCoinCollectArchiveName(this), nullptr);
    al::initNerve(this, &NrvCoinCollectDummy.Hint, 1);
    mHintState = new CoinCollectHintState(this);
    al::initNerveState(this, mHintState, &NrvCoinCollectDummy.Hint, "ヒント");
    makeActorDead();
}

void CoinCollectDummy::appear() {
    al::LiveActor::appear();
    al::setNerve(this, &NrvCoinCollectDummy.Hint);
}

void CoinCollectDummy::appearHint(const sead::Vector3f& position) {
    appear();
    al::resetPosition(this, position);
    al::hideModelIfShow(this);
    al::invalidateClipping(this);
    al::startHitReaction(this, "発光");
}

void CoinCollectDummy::reappearHint() {
    mHintState->appearHintEffect();
}

void CoinCollectDummy::deleteHint() {
    mHintState->deleteHintEffect();
}

void CoinCollectDummy::exeHint() {
    if (al::updateNerveState(this))
        makeActorDead();
}
