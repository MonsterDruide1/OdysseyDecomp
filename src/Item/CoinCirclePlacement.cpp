#include "Item/CoinCirclePlacement.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Item/Coin.h"
#include "Util/ShadowUtil.h"

namespace {
NERVE_IMPL(CoinCirclePlacement, Move);

NERVES_MAKE_STRUCT(CoinCirclePlacement, Move);
}  // namespace

CoinCirclePlacement::CoinCirclePlacement(const char* name) : al::LiveActor(name) {}

void CoinCirclePlacement::init(const al::ActorInitInfo& initInfo) {
    using CoinCirclePlacementFunctor =
        al::FunctorV0M<CoinCirclePlacement*, void (CoinCirclePlacement::*)()>;

    al::initActor(this, initInfo);
    al::initNerve(this, &NrvCoinCirclePlacement.Move, 0);
    al::getArg(&mCoinNum, initInfo, "CoinNum");

    if (mCoinNum <= 0) {
        kill();
        return;
    }

    al::getArg(&mRotateVelocity, initInfo, "RotateVelocity");
    al::tryGetArg(&mCircleXWidth, initInfo, "CircleXWidth");
    al::tryGetArg(&mCircleZWidth, initInfo, "CircleZWidth");
    al::tryGetSide(&mSide, initInfo);
    al::tryGetUp(&mUp, initInfo);
    al::tryGetFront(&mFront, initInfo);

    s32 coinNum = mCoinNum;
    mCoinArray = new Coin*[mCoinNum];
    for (s32 i = 0; i < mCoinNum; i++) {
        Coin* coin = new Coin("コイン", false);
        al::initCreateActorWithPlacementInfo(coin, initInfo);
        mCoinArray[i] = coin;

        f32 coinAngle = sead::Mathf::deg2rad((360.0f / coinNum) * i);
        f32 xWidth = mCircleXWidth * sead::Mathf::cos(coinAngle) * 100.0f;
        f32 zWidth = mCircleZWidth * sead::Mathf::sin(coinAngle) * 100.0f;

        const sead::Vector3f& centerPos = al::getTrans(this);
        sead::Vector3f coinPos = centerPos + xWidth * mSide + zWidth * mFront;

        al::setTrans(mCoinArray[i], coinPos);
        al::setScale(mCoinArray[i], {1.0f, 1.0f, 1.0f});
        al::tryAddDisplayOffset(mCoinArray[i], initInfo);
        al::expandClippingRadiusByShadowLength(
            this, &_154, rs::setShadowDropLength(mCoinArray[i], initInfo, "本体"));
        mCoinArray[i]->appearCirclePlacement();
    }

    f32 clippingRadius = 0.0f;
    const sead::Vector3f& centerPosition = al::getTrans(this);
    for (s32 i = 0; i < mCoinNum; i++) {
        sead::Vector3f distanceToCenter = al::getTrans(mCoinArray[i]) - centerPosition;
        f32 coinRadius = distanceToCenter.length() + al::getClippingRadius(mCoinArray[i]);
        clippingRadius = sead::Mathf::max(coinRadius, clippingRadius);
    }
    al::setClippingInfo(this, clippingRadius, al::getTransPtr(this));

    if (!al::listenStageSwitchOnAppear(
            this, CoinCirclePlacementFunctor(this, &CoinCirclePlacement::listenAppear))) {
        makeActorAlive();
    } else {
        makeActorDead();
        for (s32 i = 0; i < mCoinNum; i++)
            mCoinArray[i]->makeActorDead();
    }
}

void CoinCirclePlacement::listenAppear() {
    for (s32 i = 0; i < mCoinNum; i++)
        mCoinArray[i]->makeActorAlive();
    makeActorAlive();
}

inline f32 modDegree(f32 value) {
    return al::modf(value + 360.0f, 360.0f) + 0.0f;
}

void CoinCirclePlacement::exeMove() {
    if (al::isNearZero(mRotateVelocity, 0.001f))
        return;

    mCurrentAngle = modDegree(mCurrentAngle + mRotateVelocity);

    s32 coinNum = mCoinNum;
    bool isNoCoinAlive = true;
    for (s32 i = 0; i < mCoinNum; i++) {
        if (al::isDead(mCoinArray[i]) || !mCoinArray[i]->isWait())
            continue;
        f32 prevY = al::getTrans(mCoinArray[i]).y;
        f32 coinAngle = sead::Mathf::deg2rad(modDegree(mCurrentAngle + (360.0f / coinNum) * i));
        f32 xWidth = mCircleXWidth * sead::Mathf::cos(coinAngle) * 100.0f;
        f32 zWidth = mCircleZWidth * sead::Mathf::sin(coinAngle) * 100.0f;

        const sead::Vector3f& centerPos = al::getTrans(this);
        // The coin's new position is inverted on the X axis from the spawn point.
        // This is likely a game bug with no adverse effects.
        sead::Vector3f newCoinPos = -xWidth * mSide + centerPos + zWidth * mFront;
        newCoinPos.y = prevY;

        al::setTrans(mCoinArray[i], newCoinPos);
        isNoCoinAlive = false;
    }

    if (isNoCoinAlive)
        kill();
}
