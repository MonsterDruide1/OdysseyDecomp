#include "Item/CoinRail.h"

#include "Library/Base/Macros.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"

#include "Item/Coin.h"

namespace {
NERVE_IMPL(CoinRail, Move)
NERVE_IMPL(CoinRail, CloseMove)

NERVES_MAKE_STRUCT(CoinRail, CloseMove, Move)
}  // namespace

CoinRail::CoinRail(const char* name) : al::LiveActor(name) {}

ALWAYS_INLINE void addStaticCoinToRail(CoinRail* rail, const al::ActorInitInfo& initInfo,
                                       Coin** coins, f32* railPos, s32 coinNum, bool isLoop) {
    f32 posOnRail = 0.0f;
    f32 railDist = al::getRailTotalLength(rail) / (coinNum - (isLoop ? 0 : 1));
    for (s32 i = 0; i < coinNum; i++) {
        sead::Vector3f pos;
        al::calcRailPosAtCoord(&pos, rail, posOnRail);
        Coin* coin = new Coin("コイン", false);
        al::initCreateActorWithPlacementInfo(coin, initInfo);
        coins[i] = coin;
        coin->makeActorDead();
        al::setTrans(coins[i], pos);
        railPos[i] = posOnRail;
        al::tryAddDisplayOffset(coins[i], initInfo);
        posOnRail += railDist;
    }
}

ALWAYS_INLINE void addCoinToRail(CoinRail* rail, const al::ActorInitInfo& initInfo, Coin** coins,
                                 f32* railPos, s32 coinNum) {
    f32 posOnRail = 0.0f;
    for (s32 i = 0; i < coinNum; i++) {
        sead::Vector3f pos = sead::Vector3f::zero;
        al::calcRailPosAtCoord(&pos, rail, posOnRail);
        Coin* coin = new Coin("コイン", false);
        al::initCreateActorWithPlacementInfo(coin, initInfo);
        coins[i] = coin;
        coin->makeActorDead();
        railPos[i] = posOnRail;
        al::setTrans(coins[i], pos);
        posOnRail += 150.0f;
        al::tryAddDisplayOffset(coins[i], initInfo);
    }
    al::getRailTotalLength(rail);
}

void CoinRail::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);

    if (!al::isExistRail(this)) {
        kill();
        return;
    }

    al::getArg(&mCoinNum, info, "CoinNum");
    if (mCoinNum <= 1) {
        kill();
        return;
    }

    bool isLoop = al::isLoopRail(this);
    if (al::getRailPointNum(this) <= 1) {
        kill();
        return;
    }

    al::tryGetArg(&mMoveVelocity, info, "MoveVelocity");
    if (mMoveVelocity < 0.0f) {
        kill();
        return;
    }

    al::tryGetDisplayOffset(&mDisplayOffset, info);

    mCoins = new Coin*[mCoinNum];
    mRailPos = new f32[mCoinNum];

    if (al::isNearZero(mMoveVelocity))
        addStaticCoinToRail(this, info, mCoins, mRailPos, mCoinNum, isLoop);
    else
        addCoinToRail(this, info, mCoins, mRailPos, mCoinNum);

    f32 shadowLength = 1500.0f;
    al::tryGetArg(&shadowLength, info, "ShadowLength");
    for (s32 i = 0; i < mCoinNum; i++)
        mCoins[i]->setShadowDropLength(shadowLength);

    mLastCoinIndex = mCoinNum - 1;
    mFirstCoinIndex = 0;

    f32 distance = 0.0f;
    al::calcRailClippingInfo(&mClippingInfo, &distance, this, 100.0f, 100.0f);
    al::setClippingInfo(this, distance, &mClippingInfo);
    al::initSubActorKeeperNoFile(this, info, mCoinNum);

    for (s32 i = 0; i < mCoinNum; i++) {
        al::invalidateClipping(mCoins[i]);
        al::registerSubActorSyncClipping(this, mCoins[i]);
    }

    if (isLoop)
        al::initNerve(this, &NrvCoinRail.CloseMove, 0);
    else
        al::initNerve(this, &NrvCoinRail.Move, 0);

    al::LiveActor::makeActorDead();
    if (!al::trySyncStageSwitchAppear(this)) {
        al::LiveActor::appear();
        for (s32 i = 0; i < mCoinNum; i++)
            mCoins[i]->appearCoinRail();
    }
    al::invalidateHitSensors(this);
}

void CoinRail::appear() {
    al::LiveActor::appear();
    al::startHitReaction(this, "出現");
    for (s32 i = 0; i < mCoinNum; i++)
        mCoins[i]->appearCoinRail();
}

void CoinRail::kill() {
    al::LiveActor::kill();
    for (s32 i = 0; i < mCoinNum; i++)
        mCoins[i]->kill();
}

void CoinRail::makeActorDead() {
    al::LiveActor::makeActorDead();
    for (s32 i = 0; i < mCoinNum; i++)
        mCoins[i]->makeActorDead();
}

bool CoinRail::receiveMsg(const al::SensorMsg* message, al::HitSensor* other, al::HitSensor* self) {
    if (al::isMsgShowModel(message)) {
        for (s32 i = 0; i < mCoinNum; i++)
            if (!mCoins[i]->isGot())
                al::showModelIfHide(mCoins[i]);
        return true;
    }

    if (al::isMsgHideModel(message)) {
        for (s32 i = 0; i < mCoinNum; i++)
            al::hideModelIfShow(mCoins[i]);
        return true;
    }

    return false;
}

bool CoinRail::isGot() const {
    for (s32 i = 0; i < mCoinNum; i++)
        if (!mCoins[i]->isGot())
            return false;
    return true;
}

void CoinRail::exeMove() {
    if (al::isNearZero(mMoveVelocity))
        return;

    if (mMoveVelocity > 0.0f) {
        f32 railLength = al::getRailTotalLength(this);
        mRailPos[mLastCoinIndex] += mMoveVelocity;
        f32 lastCoinPos = mRailPos[mLastCoinIndex];
        if (lastCoinPos > railLength) {
            lastCoinPos = railLength;
            mMoveVelocity = -mMoveVelocity;
        }

        sead::Vector3f pos = sead::Vector3f::zero;
        for (s32 i = mLastCoinIndex; i >= mFirstCoinIndex; i--) {
            al::calcRailPosAtCoord(&pos, this, lastCoinPos);
            mRailPos[i] = lastCoinPos;
            lastCoinPos -= 150.0f;
            al::setTrans(mCoins[i], pos + mDisplayOffset);
        }
    } else {
        mRailPos[mFirstCoinIndex] += mMoveVelocity;
        f32 firstCoinPos = mRailPos[mFirstCoinIndex];
        if (firstCoinPos < 0.0f) {
            firstCoinPos = 0.0f;
            mMoveVelocity = -mMoveVelocity;
        }

        sead::Vector3f pos = sead::Vector3f::zero;
        for (s32 i = mFirstCoinIndex; i <= mLastCoinIndex; i++) {
            al::calcRailPosAtCoord(&pos, this, firstCoinPos);
            mRailPos[i] = firstCoinPos;
            firstCoinPos += 150.0f;
            al::setTrans(mCoins[i], pos + mDisplayOffset);
        }
    }

    for (s32 i = mFirstCoinIndex; i <= mLastCoinIndex; i++) {
        if (!mCoins[i]->isGot()) {
            mFirstCoinIndex = i;
            break;
        }
    }

    for (s32 i = mLastCoinIndex; i >= mFirstCoinIndex; i--) {
        if (!mCoins[i]->isGot()) {
            mLastCoinIndex = i;
            break;
        }
    }
}

void CoinRail::exeCloseMove() {
    sead::Vector3f postion = sead::Vector3f::zero;
    for (s32 i = 0; i < mCoinNum; i++) {
        if (!mCoins[i]->isGot()) {
            mRailPos[i] += mMoveVelocity;
            al::calcRailPosAtCoord(&postion, this, mRailPos[i]);
            al::setTrans(mCoins[i], postion);
        }
    }
}
