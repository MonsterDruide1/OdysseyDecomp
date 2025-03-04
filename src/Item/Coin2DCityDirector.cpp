#include "Item/Coin2DCityDirector.h"

#include "Library/Bgm/BgmBeatCounter.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"

#include "Item/Coin2DCity.h"

namespace {
NERVE_IMPL(Coin2DCityDirector, Wait);

NERVES_MAKE_STRUCT(Coin2DCityDirector, Wait);
}  // namespace

Coin2DCityDirector::Coin2DCityDirector(const char* name) : al::LiveActor(name) {}

void Coin2DCityDirector::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "Coin2DCityDirector", nullptr);
    if (!al::isExistLinkChild(initInfo, "FirstCoin", 0)) {
        kill();
        return;
    }

    al::tryGetArg(&mNextCoinLightTime, initInfo, "NextCoinLightTime");
    al::tryGetArg(&mLightTime, initInfo, "LightTime");
    al::tryGetArg(&mLightInterval, initInfo, "LightInterval");
    al::tryGetArg(&mDelayTime, initInfo, "DelayTime");
    if (mNextCoinLightTime <= -1 || mLightTime <= -1 || mLightInterval <= -1 || mDelayTime <= -1) {
        kill();
        return;
    }

    mCoinHolder.allocBuffer(100, nullptr);

    al::PlacementInfo placementInfo;
    al::getLinksInfo(&placementInfo, initInfo, "FirstCoin");
    s32 linkNestNum = al::calcLinkNestNum(placementInfo, "NextCoin");

    Coin2DCity* coin = new Coin2DCity("コイン2D都市", this);
    al::initCreateActorWithPlacementInfo(coin, initInfo, placementInfo);
    al::initSubActorKeeperNoFile(this, initInfo, linkNestNum + 1);

    mBgmBeatCounter = new al::BgmBeatCounter(this, -0.28f);
    al::initNerve(this, &NrvCoin2DCityDirector.Wait, 0);
    makeActorAlive();
}

void Coin2DCityDirector::initAfterPlacement() {
    sead::Vector3f minPos = sead::Vector3f::zero;
    sead::Vector3f maxPos = sead::Vector3f::zero;

    s32 size = mCoinHolder.size();
    for (s32 i = 0; i < size; i++) {
        al::registerSubActorSyncClipping(this, mCoinHolder[i]);
        const sead::Vector3f& trans = al::getTrans(mCoinHolder[i]);

        if (i == 0) {
            minPos = trans;
            maxPos = trans;
            continue;
        }

        minPos.x = sead::Mathf::min(minPos.x, trans.x);
        minPos.y = sead::Mathf::min(minPos.y, trans.y);
        minPos.z = sead::Mathf::min(minPos.z, trans.z);

        maxPos.x = sead::Mathf::max(maxPos.x, trans.x);
        maxPos.y = sead::Mathf::max(maxPos.y, trans.y);
        maxPos.z = sead::Mathf::max(maxPos.z, trans.z);
    }

    mClippingPos = (maxPos + minPos) * 0.5f;
    f32 clippingSize = (maxPos - minPos).length() * 0.5f + 500.0f;
    al::setClippingInfo(this, clippingSize, &mClippingPos);
}

void Coin2DCityDirector::control() {
    return mBgmBeatCounter->update();
}

void Coin2DCityDirector::registerCoin(Coin2DCity* coin) {
    mCoinHolder.pushBack(coin);
}

void Coin2DCityDirector::getCoin() {
    if (++mCoinsCollected < mCoinHolder.size())
        return;

    s32 size = mCoinHolder.size();
    for (s32 i = 0; i < size; i++)
        mCoinHolder[i]->kill();

    kill();
}

bool Coin2DCityDirector::isTriggerBeat() const {
    return mBgmBeatCounter->isOnBeat();
}

void Coin2DCityDirector::exeWait() {
    if (al::isEnableRhythmAnim(this, nullptr)) {
        if (mBgmBeatCounter->isTriggerBeat(mLightInterval, mDelayTime))
            mCoinHolder.front()->startLight();
        return;
    }

    if (al::isFirstStep(this))
        return;

    if (al::getNerveStep(this) % mLightInterval == 0)
        mCoinHolder.front()->startLight();
}
