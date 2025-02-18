#include "Item/CoinStack.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Item/CoinStackGroup.h"
#include "System/GameDataFunction.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(CoinStack, Wait);
NERVE_IMPL(CoinStack, Float);
NERVE_IMPL(CoinStack, Fall);
NERVE_IMPL(CoinStack, Land);
NERVE_IMPL(CoinStack, Collected);

NERVES_MAKE_NOSTRUCT(CoinStack, Land, Collected);
NERVES_MAKE_STRUCT(CoinStack, Wait, Fall, Float);
}  // namespace

CoinStack::CoinStack(const char* name) : al::LiveActor(name) {}

CoinStack::~CoinStack() = default;

void CoinStack::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "CoinStack", nullptr);
    al::initNerve(this, &NrvCoinStack.Wait, 0);
    al::setClippingNearDistance(this, -1.0f);
    makeActorDead();
}

bool CoinStack::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                           al::HitSensor* self) {
    if (rs::isMsgItemGetAll(message)) {
        al::invalidateClipping(this);
        al::setNerve(this, &Collected);
        return true;
    }

    return false;
}

CoinStack* CoinStack::getBelow() {
    return mStackBelow;
}

CoinStack* CoinStack::getAbove() {
    return mStackAbove;
}

void CoinStack::makeStackAppear() {
    CoinStack* stack = this;
    do {
        stack->makeActorAlive();
        stack = stack->getAbove();
    } while (stack != nullptr);
}

void CoinStack::makeStackDisappear() {
    CoinStack* stack = this;
    do {
        stack->makeActorDead();
        stack = stack->getAbove();
    } while (stack != nullptr);
}

void CoinStack::changeAlpha(f32 alphaMask) {
    CoinStack* stack = this;
    do {
        al::stopDitherAnimAutoCtrl(stack);
        al::setModelAlphaMask(stack, alphaMask);
        stack = stack->getAbove();
    } while (stack != nullptr);
}

f32 CoinStack::getFallSpeed() {
    if (!al::isNerve(this, &NrvCoinStack.Fall))
        mFallSpeed = 0.0f;
    else
        mFallSpeed = sead::Mathf::min(mFallSpeed + 0.5, 20.0f);

    return mFallSpeed;
}

void CoinStack::setAbove(CoinStack* stack) {
    mStackAbove = stack;
}

void CoinStack::setBelow(CoinStack* stack) {
    mStackBelow = stack;
}

void CoinStack::signalFall(u32 delay, f32 radius) {
    mLandHeight -= *mExternalFallDistance;
    f32 fallDistance = *mExternalFallDistance;
    mClippingRadius = radius;
    mClippingPos.y += fallDistance * -0.5f;

    if (mStackAbove != nullptr)
        mStackAbove->signalFall(delay + 1, radius);

    if (!al::isNerve(this, &NrvCoinStack.Float) && !al::isNerve(this, &NrvCoinStack.Fall)) {
        mFloatDuration = delay * 5;
        al::setNerve(this, &NrvCoinStack.Float);
    }
}

void CoinStack::postInit(CoinStackGroup* coinStackGroup, const sead::Vector3f& transY,
                         CoinStack* below, const sead::Vector3f& clippingPos, f32 clippingRadius,
                         const f32* fallDistance) {
    mExternalFallDistance = fallDistance;
    mCoinStackGroup = coinStackGroup;
    mStackBelow = below;
    mClippingPos = clippingPos;
    mClippingRadius = clippingRadius;
    al::setClippingInfo(this, clippingRadius, &mClippingPos);
    al::setTrans(this, transY);
    mLandHeight = transY.y;
    mTransY = transY.y;
    if (mStackBelow != nullptr)
        mStackBelow->setAbove(this);
}

void CoinStack::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void CoinStack::exeFloat() {
    if (al::isLessStep(this, mFloatDuration))
        return;

    al::setNerve(this, &NrvCoinStack.Fall);
}

void CoinStack::exeFall() {
    if (al::isFirstStep(this))
        al::startAction(this, "Fall");

    if (mTransY < mLandHeight) {
        al::setTransY(this, mLandHeight);
        al::setNerve(this, &Land);
        return;
    }

    if (mStackBelow != nullptr && mTransY - mStackBelow->getTransY() < *mExternalFallDistance)
        al::setNerve(this, &Land);
    else
        al::setTransY(this, mTransY);
}

void CoinStack::exeLand() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Land");
        mFallSpeed = 0.0;
    }

    if (al::isActionEnd(this)) {
        if (mStackAbove == nullptr)
            mCoinStackGroup->validateClipping();

        al::setClippingInfo(this, mClippingRadius, &mClippingPos);

        al::Nerve* nerve;
        if (mTransY > mLandHeight)
            nerve = &NrvCoinStack.Fall;
        else
            nerve = &NrvCoinStack.Wait;

        al::setNerve(this, nerve);
    }
}

void CoinStack::exeCollected() {
    al::startHitReaction(this, "取得");
    GameDataFunction::addCoin(this, 5);
    mClippingRadius = mCoinStackGroup->setStackAsCollected(this);

    if (mStackBelow != nullptr)
        mStackBelow->setAbove(mStackAbove);

    if (mStackAbove != nullptr) {
        mStackAbove->setBelow(mStackBelow);
        mStackAbove->signalFall(0, mClippingRadius);
    }

    kill();
}
