#include "Layout/CoinCounter.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Se/SeFunction.h"

#include "System/GameDataFunction.h"
#include "Util/StageLayoutFunction.h"

namespace al {
class IUseLayout;
}  // namespace al

namespace {
NERVE_IMPL(CoinCounter, Appear);
NERVE_IMPL(CoinCounter, Wait);
NERVE_IMPL(CoinCounter, End);
NERVE_IMPL(CoinCounter, Add);
NERVE_IMPL(CoinCounter, Sub);
NERVE_IMPL(CoinCounter, CountAnimAdd);
NERVE_IMPL(CoinCounter, CountAnimSub);

NERVES_MAKE_NOSTRUCT(CoinCounter, Wait);
NERVES_MAKE_STRUCT(CoinCounter, Appear, End, Add, Sub, CountAnimAdd, CountAnimSub);
}  // namespace

CoinCounter::CoinCounter(const char* name, const al::LayoutInitInfo& initInfo, bool isCoin)
    : al::LayoutActor(name), mIsCoin(isCoin) {
    if (mIsCoin) {
        al::initLayoutActor(this, initInfo, "CounterCoin", nullptr);
        mNumDigits = 4;
        mPanelName = "Coin";
    } else {
        al::initLayoutActor(this, initInfo, "CounterCollectCoin", nullptr);
        mNumDigits = 3;
        mPanelName = "CollectCoin";
        al::setPaneString(this, "TxtIcon", rs::getWorldCoinCollectPictureFont(this), 0);
        al::setPaneString(this, "TxtIconSh", rs::getWorldCoinCollectPictureFont(this), 0);
    }

    initNerve(&NrvCoinCounter.Appear, 0);
    kill();
    updatePanel(mCoinNum, mNumDigits);
}

void CoinCounter::kill() {
    al::setNerve(this, &NrvCoinCounter.Appear);
    al::LayoutActor::kill();
}

void CoinCounter::updatePanel(u32 coinCount, s32 numDigits) {
    if (numDigits == 4) {
        al::IUseLayout* layout = static_cast<al::LayoutActor*>(this);
        al::setPaneStringFormat(layout, "TxtCounter", "%04d", coinCount);
        al::setPaneStringFormat(layout, "TxtCounterSh", "%04d", coinCount);
        al::requestCaptureRecursive(this);
    }
    if (numDigits == 3) {
        al::IUseLayout* layout = static_cast<al::LayoutActor*>(this);
        al::setPaneStringFormat(layout, "TxtCounter", "%03d", coinCount);
        al::setPaneStringFormat(layout, "TxtCounterSh", "%03d", coinCount);
        al::requestCaptureRecursive(this);
    }
}

bool CoinCounter::isWait() const {
    return al::isNerve(this, &Wait);
}

void CoinCounter::tryStart() {
    if (al::isNerve(this, &NrvCoinCounter.Appear)) {
        al::startAction(this, "Appear", nullptr);
        updateCountImmidiate();
        al::LayoutActor::appear();
        al::setNerve(this, &NrvCoinCounter.End);
    }
}

void CoinCounter::updateCountImmidiate() {
    s32 newCoinNum = getCountFromData();
    s32 newTotalCoins = getCountTotalFromData();

    mTotalCoins = newTotalCoins;
    mCoinNum = newCoinNum;
    updatePanel(mCoinNum, mNumDigits);
}

void CoinCounter::tryEnd() {
    if (!al::isNerve(this, &NrvCoinCounter.Appear))
        al::setNerve(this, &NrvCoinCounter.Appear);
}

void CoinCounter::startCountAnim(s32 coinNum) {
    s32 prevCoinCount = mCoinNum;
    mCoinNum = coinNum;
    mPrevCoinCount = prevCoinCount;
    mTotalCoins = getCountTotalFromData();

    if (coinNum < prevCoinCount)
        al::setNerve(this, &NrvCoinCounter.Sub);
    else
        al::setNerve(this, &NrvCoinCounter.Add);
}

bool CoinCounter::tryUpdateCount() {
    s32 newCoinNum = getCountFromData();
    s32 newTotalCoins = getCountTotalFromData();

    if (mTotalCoins == newTotalCoins) {
        if (newCoinNum >= mCoinNum)
            return false;
    }

    al::Nerve* nerve;
    if (mTotalCoins < newTotalCoins)
        nerve = &NrvCoinCounter.CountAnimAdd;
    else
        nerve = &NrvCoinCounter.CountAnimSub;

    mTotalCoins = newTotalCoins;
    mCoinNum = newCoinNum;
    al::setNerve(this, nerve);
    return true;
}

s32 CoinCounter::getCountFromData() const {
    if (mIsCoin)
        return GameDataFunction::getCoinNum(this);

    return GameDataFunction::getCoinCollectNum(this);
}

s32 CoinCounter::getCountTotalFromData() const {
    if (mIsCoin)
        return GameDataFunction::getTotalCoinNum(this);

    return GameDataFunction::getCoinCollectGotNum(this);
}

void CoinCounter::exeAppear() {
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
}

void CoinCounter::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", nullptr);
    if (mIsUpdateCount)
        tryUpdateCount();
}

void CoinCounter::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);
    if (al::isActionEnd(this, nullptr))
        kill();
}

void CoinCounter::exeAdd() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Add", mPanelName);
        updatePanel(mCoinNum, mNumDigits);
    }
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
}

void CoinCounter::exeSub() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Add", mPanelName);
        updatePanel(mCoinNum, mNumDigits);
    }
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
}

void CoinCounter::exeCountAnimAdd() {
    if (al::isFirstStep(this))
        mAnimationCount = mPrevCoinCount;

    if (al::isGreaterEqualStep(this, 60)) {
        updatePanel(mCoinNum, mNumDigits);
        al::setNerve(this, &Wait);
        return;
    }

    s32 animationCount = al::calcNerveValue(this, 60, mPrevCoinCount, mCoinNum);
    updatePanel(animationCount, mNumDigits);

    if (mAnimationCount != animationCount) {
        al::startSe(this, mIsCoin ? "CoinCount" : "CoinCollectCount");
        mAnimationCount = animationCount;
    }

    al::holdSe(this, "CoinCount_loop");
}

void CoinCounter::exeCountAnimSub() {
    if (al::isFirstStep(this))
        mAnimationCount = mPrevCoinCount;

    if (al::isGreaterEqualStep(this, 60)) {
        updatePanel(mCoinNum, mNumDigits);
        al::setNerve(this, &Wait);
        return;
    }

    s32 animationCount = al::calcNerveValue(this, 60, mPrevCoinCount, mCoinNum);
    updatePanel(animationCount, mNumDigits);

    if (mAnimationCount != animationCount) {
        al::startSe(this, mIsCoin ? "CoinCount" : "CoinCollectCount");
        mAnimationCount = animationCount;
    }

    al::holdSe(this, "CoinCount_loop");
}
