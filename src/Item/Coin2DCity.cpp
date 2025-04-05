#include "Item/Coin2DCity.h"

#include "Library/Controller/PadRumbleFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Scene/SceneObjUtil.h"
#include "Library/Stage/StageResourceList.h"

#include "Item/Coin2DCityDirector.h"
#include "System/GameDataFunction.h"
#include "Util/ActorDimensionKeeper.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(Coin2DCity, Wait);
NERVE_IMPL(Coin2DCity, Light);
NERVE_IMPL(Coin2DCity, Got);
NERVE_IMPL(Coin2DCity, GotWait);

NERVES_MAKE_NOSTRUCT(Coin2DCity, Wait);
NERVES_MAKE_STRUCT(Coin2DCity, Got, Light, GotWait);
}  // namespace

Coin2DCity::Coin2DCity(const char* name, Coin2DCityDirector* director)
    : al::LiveActor(name), mCityDirector(director) {}

void Coin2DCity::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "CoinDot", nullptr);
    al::tryAddDisplayOffset(this, initInfo);
    mDimensionKeeper = rs::createDimensionKeeper(this);
    rs::updateDimensionKeeper(mDimensionKeeper);

    mCityDirector->registerCoin(this);
    al::PlacementInfo placementInfo;
    if (al::isExistLinkChild(initInfo, "NextCoin", 0)) {
        al::getLinksInfo(&placementInfo, initInfo, "NextCoin");
        mNextCoin = new Coin2DCity("コイン2D都市", mCityDirector);
        al::initCreateActorWithPlacementInfo(mNextCoin, initInfo, placementInfo);
    }

    al::initNerve(this, &Wait, 0);
    al::invalidateClipping(this);
    makeActorAlive();
}

void Coin2DCity::control() {
    mSyncCounter = al::getSceneObj<al::StageSyncCounter>(this)->getCounter();
    if (mLightTime > -1 && mCityDirector->isTriggerBeat()) {
        if (mNextCoin != nullptr && mLightTime == 1)
            mNextCoin->startLight();

        if (mLightTime == mCityDirector->getLightTime())
            endLight();
        else
            mLightTime++;
    }
}

bool Coin2DCity::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                            al::HitSensor* self) {
    if (rs::isMsgPlayerItemGet2D(message) && !al::isNerve(this, &NrvCoin2DCity.Got) &&
        !al::isNerve(this, &NrvCoin2DCity.GotWait)) {
        al::startHitReaction(this, "取得");
        al::setNerve(this, &NrvCoin2DCity.Got);
        return true;
    }
    return false;
}

ActorDimensionKeeper* Coin2DCity::getActorDimensionKeeper() const {
    return mDimensionKeeper;
}

void Coin2DCity::startLight() {
    mLightTime = 0;
    if (!al::isNerve(this, &NrvCoin2DCity.Got) && !al::isNerve(this, &NrvCoin2DCity.GotWait)) {
        al::setNerve(this, &NrvCoin2DCity.Light);
        al::startVisAnim(this, "LightOn");
    }

    al::StageSyncCounter* syncCounter = al::getSceneObj<al::StageSyncCounter>(this);
    if (mSyncCounter == syncCounter->getCounter() && mCityDirector->isTriggerBeat()) {
        if (mNextCoin != nullptr && mLightTime == 1)
            mNextCoin->startLight();

        if (mLightTime == mCityDirector->getLightTime())
            endLight();
        else
            mLightTime++;
    }
}

void Coin2DCity::endLight() {
    al::startVisAnim(this, "LightOff");
    mLightTime = -1;
}

void Coin2DCity::exeWait() {
    al::startVisAnim(this, "LightOff");
}

void Coin2DCity::exeLight() {}

void Coin2DCity::exeGot() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Got");
        alPadRumbleFunction::startPadRumble(this, "コッ（微弱）", 1000.0f, 3000.0f);
        GameDataFunction::addCoin(this, 1);
    }
    if (al::isActionEnd(this))
        al::setNerve(this, &NrvCoin2DCity.GotWait);
}

void Coin2DCity::exeGotWait() {
    if (al::isFirstStep(this)) {
        al::hideModelIfShow(this);
        mCityDirector->getCoin();
    }
}
