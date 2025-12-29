#include "Item/CoinLead.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"

#include "Item/Coin.h"

namespace {
NERVE_IMPL(CoinLead, Wait)
NERVE_IMPL(CoinLead, FirstCoinGotDelay)
NERVE_IMPL(CoinLead, Move)
NERVE_IMPL(CoinLead, WaitAllGet)

NERVES_MAKE_NOSTRUCT(CoinLead, Wait, FirstCoinGotDelay, Move, WaitAllGet)
}  // namespace

CoinLead::CoinLead(const char* name) : al::LiveActor(name) {}

void CoinLead::init(const al::ActorInitInfo& info) {
    using CoinLeadFunctor =
        al::RailPlacementCallFunctorClassMember<CoinLead*, void (CoinLead::*)(const sead::Vector3f&,
                                                                              s32, f32)>;

    al::initActor(this, info);

    if (!al::isExistRail(this)) {
        kill();
        return;
    }

    al::getArg(&mPlaceInterval, info, "PlaceInterval");
    if (mPlaceInterval < 0.0f || al::getRailPointNum(this) <= 1) {
        kill();
        return;
    }

    f32 speed = 0.0f;
    al::getArg(&speed, info, "Speed");
    if (speed < 0.0f) {
        kill();
        return;
    }

    bool isPlaceRailPoint = false;
    al::getArg(&isPlaceRailPoint, info, "IsPlaceRailPoint");
    s32 divideNum = al::calcRailDivideNum(this, mPlaceInterval, isPlaceRailPoint);
    mCoinArray = new Coin*[divideNum];
    mCoinCoords = new f32[divideNum];
    mCoinCount = divideNum;

    al::tryGetDisplayOffset(&mDisplayOffset, info);

    for (s32 i = 0; i < divideNum; i++) {
        Coin* coin = new Coin("コイン", false);
        al::initCreateActorWithPlacementInfo(coin, info);
        mCoinArray[i] = coin;
        mCoinCoords[i] = 0.0f;
    }

    mCoinLead = new Coin("1つめコイン", false);
    al::initCreateActorNoPlacementInfo(mCoinLead, info);
    al::resetPosition(mCoinLead, al::getTrans(this) + mDisplayOffset);

    al::placementRailDivide(this, mPlaceInterval, isPlaceRailPoint,
                            CoinLeadFunctor(this, &CoinLead::place));

    f32 distance;
    al::calcRailClippingInfo(&mClippingInfo, &distance, this, 100.0f, 100.0f);
    al::setClippingInfo(this, distance, &mClippingInfo);

    f32 shadowLength = 1500.0f;
    al::tryGetArg(&shadowLength, info, "ShadowLength");
    mCoinLead->setShadowDropLength(shadowLength);
    for (s32 i = 0; i < mCoinCount; i++)
        mCoinArray[i]->setShadowDropLength(shadowLength);

    al::tryGetArg(&mIsPlaySuccessSe, info, "IsPlaySuccessSe");
    mSpeed = speed;

    mRailTotalLength = al::getRailTotalLength(this);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

void CoinLead::place(const sead::Vector3f& pos, s32 index, f32 coord) {
    al::setTrans(mCoinArray[index], pos + mDisplayOffset);
    mCoinArray[index]->makeActorDead();
    mCoinCoords[index] = coord;
}

void CoinLead::exeWait() {
    if (mCoinLead->isGotOrRotate()) {
        al::setNerve(this, &FirstCoinGotDelay);
        mDistance = 0.0f;
        mCoinsDisplayed = 0;
        al::invalidateClipping(this);
    }
}

void CoinLead::exeFirstCoinGotDelay() {
    if (isGreaterEqualStep(this, 15))
        al::setNerve(this, &Move);
}

void CoinLead::exeMove() {
    mDistance += mSpeed;

    if (mCoinsDisplayed >= mCoinCount)
        return;

    // TODO: Clean this loop
    s32 count = mCoinsDisplayed;
    for (s32 i = mCoinsDisplayed; mCoinCoords[count] < mDistance; i++) {
        mCoinArray[count]->appearLimitTime(600);
        if (i == mCoinCount - 1) {
            al::setNerve(this, &WaitAllGet);
            return;
        }
        count = i + 1;
        if (count >= mCoinCount)
            return;
    }
    mCoinsDisplayed = count;
}

void CoinLead::exeWaitAllGet() {
    for (s32 i = 0; i < mCoinCount; i++) {
        bool isGot = mCoinArray[i]->isGot();

        if (al::isDead(mCoinArray[i]) && !isGot) {
            kill();
            return;
        }

        if (!isGot)
            return;
    }

    if (mIsPlaySuccessSe)
        al::startHitReaction(this, "全取得");
    kill();
}
