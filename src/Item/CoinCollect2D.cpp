#include "Item/CoinCollect2D.h"

#include "Library/Collision/PartsConnector.h"
#include "Library/Controller/PadRumbleFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Scene/SceneObjUtil.h"

#include "Item/CoinCollectEmpty2D.h"
#include "Item/CoinCollectHintState.h"
#include "Item/CoinCollectHolder.h"
#include "Item/CoinCollectWatcher.h"
#include "MapObj/CapMessageShowInfo.h"
#include "System/GameDataFunction.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/ActorDimensionUtil.h"
#include "Util/ItemUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CoinCollect2D, Wait);
NERVE_IMPL(CoinCollect2D, WaitHint);
NERVE_IMPL(CoinCollect2D, Got);

NERVES_MAKE_STRUCT(CoinCollect2D, Wait, WaitHint, Got);
}  // namespace

CoinCollect2D::CoinCollect2D(const char* name) : al::LiveActor(name) {}

void CoinCollect2D::init(const al::ActorInitInfo& initInfo) {
    al::initActorSceneInfo(this, initInfo);
    rs::createCoinCollectWatcher(this);
    rs::createCoinCollectHolder(this);
    CoinCollectHolder* holder = al::getSceneObj<CoinCollectHolder>(this);
    holder->registerCoinCollect2D(this);

    if (!GameDataFunction::isGotCoinCollect(this, initInfo)) {
        al::initActorWithArchiveName(this, initInfo, rs::getStageCoinCollect2DArchiveName(this),
                                     nullptr);
        makeActorAlive();
        CoinCollectWatcher* watcher = al::getSceneObj<CoinCollectWatcher>(this);
        watcher->registerCoin(false);
        al::initNerve(this, &NrvCoinCollect2D.Wait, 1);

        mHintState = new CoinCollectHintState(this);
        al::initNerveState(this, mHintState, &NrvCoinCollect2D.WaitHint, "ヒント");

        al::tryAddDisplayOffset(this, initInfo);
        mMtxConnector = al::tryCreateMtxConnector(this, initInfo);
        mDimensionKeeper = rs::createDimensionKeeper(this);
        rs::updateDimensionKeeper(mDimensionKeeper);
        rs::snap2DParallelizeFront(this, this, 500.0f);
        al::startAction(this, "Wait");
        mPlacementId = al::createPlacementId(initInfo);

    } else {
        makeActorDead();
        const char* archiveName = rs::getStageCoinCollect2DEmptyArchiveName(this);
        CoinCollectWatcher* watcher = al::getSceneObj<CoinCollectWatcher>(this);
        watcher->registerCoin(true);

        CoinCollectEmpty2D* coinCollectEmpty2d =
            new CoinCollectEmpty2D("コレクトコイン空2D", archiveName);
        al::initCreateActorWithPlacementInfo(coinCollectEmpty2d, initInfo);
    }
}

void CoinCollect2D::initAfterPlacement() {
    if (mMtxConnector != nullptr)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

bool CoinCollect2D::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                               al::HitSensor* self) {
    if (rs::isMsgItemGet2D(message) && (al::isNerve(this, &NrvCoinCollect2D.Wait) ||
                                        al::isNerve(this, &NrvCoinCollect2D.WaitHint))) {
        al::invalidateClipping(this);
        al::setNerve(this, &NrvCoinCollect2D.Got);
        return true;
    }

    if (al::isMsgPlayerDisregard(message))
        return true;

    return false;
}

void CoinCollect2D::endClipped() {
    rs::syncCoin2DAnimFrame(this, "Wait");
    al::LiveActor::endClipped();
}

ActorDimensionKeeper* CoinCollect2D::getActorDimensionKeeper() const {
    return mDimensionKeeper;
}

void CoinCollect2D::appearHintEffect() {
    al::invalidateClipping(this);
    al::setNerve(this, &NrvCoinCollect2D.WaitHint);
    al::startHitReaction(this, "発光");
}

void CoinCollect2D::deleteHintEffect() {
    mHintState->deleteHintEffect();
}

void CoinCollect2D::reappearHintEffect() {
    mHintState->appearHintEffect();
}

bool CoinCollect2D::isEnableHint() const {
    if (al::isDead(this))
        return true;

    return al::isNerve(this, &NrvCoinCollect2D.Wait);
}

void CoinCollect2D::exeWait() {
    if (al::isFirstStep(this))
        al::validateClipping(this);

    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);
}

void CoinCollect2D::exeWaitHint() {
    if (mMtxConnector != nullptr)
        al::connectPoseQT(this, mMtxConnector);

    if (al::updateNerveState(this))
        al::setNerve(this, &NrvCoinCollect2D.Wait);
}

void CoinCollect2D::exeGot() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Got");
        CoinCollectWatcher* watcher = al::getSceneObj<CoinCollectWatcher>(this);
        watcher->countup(this);

        GameDataFunction::addCoinCollect(this, mPlacementId);
        rs::tryShowCapMsgCollectCoinGetFirst(this);
        alPadRumbleFunction::startPadRumble(this, "コッ（弱）", 1000.0f, 3000.0f);
    }

    if (al::isActionEnd(this))
        kill();
}
