#include "Layout/StageSceneLayout.h"

#include <nn/ui2d/Layout.h>
#include <nn/ui2d/Pane.h>

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Layout/LayoutKeeper.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Player/PlayerUtil.h"

#include "Item/ShineChipWatcherHolder.h"
#include "Layout/CoinCounter.h"
#include "Layout/CounterLifeCtrl.h"
#include "Layout/KidsModeLayoutAccessor.h"
#include "Layout/MapMini.h"
#include "Layout/PlayGuideBgm.h"
#include "Layout/PlayGuideCamera.h"
#include "Layout/ShineChipLayoutParts.h"
#include "Layout/ShineCounter.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "System/GameDataUtil.h"
#include "Util/GamePadPlayStyleInfo.h"
#include "Util/SpecialBuildUtil.h"
#include "Util/StageInputFunction.h"
#include "eui/TextBoxEx.h"

namespace {
NERVE_IMPL(StageSceneLayout, End);
NERVE_IMPL(StageSceneLayout, Appear);
NERVE_IMPL(StageSceneLayout, EndWithoutCoin);
NERVE_IMPL(StageSceneLayout, Wait);
NERVE_IMPL(StageSceneLayout, CoinCountAnim);
NERVE_IMPL(StageSceneLayout, ShineChipComplete);
NERVE_IMPL(StageSceneLayout, ShineCountAppear);

NERVES_MAKE_NOSTRUCT(StageSceneLayout, End, Appear, EndWithoutCoin, Wait, CoinCountAnim,
                     ShineChipComplete, ShineCountAppear);
}  // namespace

StageSceneLayout::StageSceneLayout(const char* name, const al::LayoutInitInfo& initInfo,
                                   const al::PlayerHolder* playerHolder,
                                   const al::SubCameraRenderer* subCameraRenderer)
    : al::NerveStateBase(name), mPlayerHolder(playerHolder) {
    initNerve(&End, 0);

    mCoinCounter = new CoinCounter("[シーン情報]コインカウンタ", initInfo, true);
    mCoinCollectCounter = new CoinCounter("[シーン情報]コインコレクトカウンタ", initInfo, false);
    mCounterLifeCtrl = new CounterLifeCtrl(initInfo, mPlayerHolder, subCameraRenderer);
    mShineCounter = new ShineCounter("[シーン情報]シャインカウンタ", initInfo);
    mShineChipLayoutParts = new ShineChipLayoutParts(initInfo, "CounterPiece");
    mPlayGuideCamera = new PlayGuideCamera("[シーン情報]カメラ操作レイアウト", initInfo,
                                           al::getPlayerActor(mPlayerHolder, 0));
    mPlayGuideBgm = new PlayGuideBgm("[シーン情報]BGM再生レイアウト更新", initInfo);
    mPlayGuideMenu = new al::SimpleLayoutAppearWaitEnd("[シーン情報]メニューガイド",
                                                       "PlayGuideMenu", initInfo, nullptr, false);
    al::setPaneString(
        mPlayGuideMenu, "TxtGuideSh",
        al::getLayoutMessageString(mPlayGuideMenu, "PlayGuideMenu", "PlayGuideMenu_Guide"));
    mPlayGuideMenu->kill();

    mMapMini = new MapMini(initInfo, mPlayerHolder);
    startActionAll("Wait");

    mGamePadPlayStyleInfo = new GamePadPlayStyleInfo{0, 5, 5};
    rs::recordGamePadPlayStyleInfo(mGamePadPlayStyleInfo, mPlayGuideMenu);

    mLayoutActorKidsMode = new al::LayoutActor("キッズモード");
    al::initLayoutActor(mLayoutActorKidsMode, initInfo, "KidsMode");
    al::startAction(mLayoutActorKidsMode, "Wait");
    mLayoutActorKidsMode->kill();

    kill();
}

void StageSceneLayout::startActionAll(const char* actionName) {
    if (rs::isExistShineChipWatcher(mShineChipLayoutParts) &&
        !rs::isAppearedShineChipShine(mShineChipLayoutParts)) {
        al::startAction(mShineChipLayoutParts, actionName);
    }
}

void StageSceneLayout::control() {
    mPlayGuideBgm->update();
}

void StageSceneLayout::updatePlayGuideMenuText() {
    if (rs::tryUpdateGamePadPlayStyleInfo(mGamePadPlayStyleInfo, mPlayGuideMenu)) {
        nn::ui2d::Pane* rootPane = mPlayGuideMenu->getLayoutKeeper()->getLayout()->GetPane();
        eui::TextBoxEx* textBox = nn::ui2d::DynamicCast<eui::TextBoxEx*, nn::ui2d::Pane>(
            rootPane->FindPaneByName("TxtGuide", true));
        textBox->mDirtyFlags |= 4;
        textBox = nn::ui2d::DynamicCast<eui::TextBoxEx*, nn::ui2d::Pane>(
            rootPane->FindPaneByName("TxtGuideSh", true));
        textBox->mDirtyFlags |= 4;
        al::requestCaptureRecursive(mPlayGuideMenu);
    }
}

void StageSceneLayout::setDirtyFlagForPlayGuideMenu() {
    nn::ui2d::Pane* rootPane = mPlayGuideMenu->getLayoutKeeper()->getLayout()->GetPane();
    eui::TextBoxEx* textBox = nn::ui2d::DynamicCast<eui::TextBoxEx*, nn::ui2d::Pane>(
        rootPane->FindPaneByName("TxtGuide", true));
    textBox->mDirtyFlags |= 4;
    textBox = nn::ui2d::DynamicCast<eui::TextBoxEx*, nn::ui2d::Pane>(
        rootPane->FindPaneByName("TxtGuideSh", true));
    textBox->mDirtyFlags |= 4;
    al::requestCaptureRecursive(mPlayGuideMenu);
}

void StageSceneLayout::start() {
    updateCounterParts();

    al::LayoutActor* kidsModeLayout = mLayoutActorKidsMode;
    if (rs::isKidsMode(kidsModeLayout))
        al::appearLayoutIfDead(kidsModeLayout);

    mCounterLifeCtrl->appear();

    if (!rs::isModeE3Rom() && !rs::isModeE3LiveRom())
        mMapMini->appearSlideIn();

    mCoinCounter->tryStart();

    tryAppearCoinCollectCounter();

    if (rs::isExistShineChipWatcher(mShineChipLayoutParts) &&
        rs::getShineChipCount(mShineChipLayoutParts) >= 1 &&
        rs::getShineChipCount(mShineChipLayoutParts) <= 4 &&
        !rs::isAppearedShineChipShine(mShineChipLayoutParts) &&
        !rs::isCurrentShineChipWatcherTypeEmpty(mShineChipLayoutParts)) {
        al::appearLayoutIfDead(mShineChipLayoutParts);
    }

    mShineCounter->tryStart();
    mPlayGuideBgm->start();

    if (!rs::isModeE3Rom() && !rs::isModeE3LiveRom())
        al::appearLayoutIfDead(mPlayGuideMenu);

    al::NerveStateBase::appear();
    mCoinCounter->setUpdateCount(true);
    mCoinCollectCounter->setUpdateCount(true);
    al::setNerve(this, &Appear);
}

void StageSceneLayout::updateCounterParts() {
    rs::tryUpdateShineChipLayoutCount(mShineChipLayoutParts);
}

void StageSceneLayout::tryAppearCoinCollectCounter() {
    if (GameDataFunction::getCoinCollectNumMax(mCoinCollectCounter) != 0)
        mCoinCollectCounter->tryStart();
    else
        al::killLayoutIfActive(mCoinCollectCounter);
}

void StageSceneLayout::startOnlyCoin(bool isOnlyCoin) {
    if (!al::isNerve(this, &EndWithoutCoin)) {
        mIsOnlyCoin = isOnlyCoin;
        appear();
        al::setNerve(this, &EndWithoutCoin);
    }
}

void StageSceneLayout::endWithoutCoin(bool isOnlyCoin) {
    if (!al::isNerve(this, &EndWithoutCoin)) {
        mIsOnlyCoin = isOnlyCoin;
        appear();
        al::setNerve(this, &EndWithoutCoin);
    }
}

void StageSceneLayout::end() {
    al::setNerve(this, &End);
}

bool StageSceneLayout::isEnd() const {
    return isDead() || al::isNerve(this, &End);
}

bool StageSceneLayout::isWait() const {
    return !isDead() && al::isNerve(this, &Wait);
}

bool StageSceneLayout::isActive() const {
    return !isDead();
}

bool StageSceneLayout::isEndLifeDemo() const {
    return mCounterLifeCtrl->isEndLifeDemo();
}

bool StageSceneLayout::tryStartLifeDemo() {
    return mCounterLifeCtrl->tryUpdateCount(
        GameDataFunction::getPlayerHitPointMaxCurrent(mCoinCounter));
}

void StageSceneLayout::startCoinCountAnim(s32 coinNum) {
    mCoinCounter->startCountAnim(coinNum);
    al::setNerve(this, &CoinCountAnim);
}

void StageSceneLayout::startCoinCollectCountAnim(s32 coinNum) {
    mCoinCollectCounter->startCountAnim(coinNum);
    al::setNerve(this, &CoinCountAnim);
}

void StageSceneLayout::appearCoinCounterForDemo() {
    mCoinCounter->tryStart();
    mCoinCounter->setUpdateCount(false);

    tryAppearCoinCollectCounter();

    mCoinCollectCounter->setUpdateCount(false);
}

bool StageSceneLayout::isEndCoinCountAnim() const {
    if (!mCoinCounter->isWait())
        return false;

    return al::isDead(mCoinCollectCounter) || mCoinCollectCounter->isWait();
}

bool StageSceneLayout::isEndShineChipCompleteAnim() const {
    return mShineChipLayoutParts->isEndCompleteAnim();
}

void StageSceneLayout::startShineChipCompleteAnim() {
    mCoinCounter->tryEnd();
    mCoinCollectCounter->tryEnd();
    mShineCounter->tryEnd();
    mPlayGuideBgm->end();
    mCounterLifeCtrl->end();
    mPlayGuideMenu->end();
    al::setNerve(this, &ShineChipComplete);
}

void StageSceneLayout::endShineChipCompleteAnim() {
    mCoinCounter->tryStart();
    mCoinCollectCounter->tryStart();
    mShineCounter->tryStart();
    mPlayGuideBgm->end();
    mCounterLifeCtrl->appear();
    mPlayGuideMenu->appear();
    mCoinCounter->setUpdateCount(true);
    mCoinCollectCounter->setUpdateCount(true);
    al::setNerve(this, &Appear);
}

bool StageSceneLayout::tryStartDemoGetLifeMaxUpItem(bool isHackKoopa) {
    return mCounterLifeCtrl->tryStartDemoLifeUp(isHackKoopa);
}

bool StageSceneLayout::isEndDemoGetLifeMaxUpItem() const {
    return mCounterLifeCtrl->isEndLifeDemo();
}

void StageSceneLayout::killShineCount() {
    al::killLayoutIfActive(mShineCounter);
}

void StageSceneLayout::appearShineCountWait() {
    mShineCounter->tryStartWait();
}

void StageSceneLayout::startCloset() {
    al::killLayoutIfActive(mShineCounter);
}

void StageSceneLayout::endCloset() {
    mShineCounter->tryStart();
}

void StageSceneLayout::missEnd() {
    al::killLayoutIfActive(mCoinCounter);
    al::killLayoutIfActive(mCoinCollectCounter);
    mCounterLifeCtrl->kill();
    al::killLayoutIfActive(mShineCounter);
    mPlayGuideBgm->endImmediate();
    al::killLayoutIfActive(mShineChipLayoutParts);
    al::killLayoutIfActive(mPlayGuideMenu);

    if (!rs::isModeE3Rom() && !rs::isModeE3LiveRom())
        al::killLayoutIfActive(mMapMini);
}

void StageSceneLayout::appearPlayGuideCamera() {
    mPlayGuideCamera->start();
}

void StageSceneLayout::exeAppear() {
    if (mCoinCounter->isAlive()) {
        const al::IUseLayoutAction& layoutAction = *mCoinCounter;
        if (al::isActionEnd(&layoutAction))
            al::setNerve(this, &Wait);
    }
}

bool StageSceneLayout::isActionEndAll() const {
    if (mCoinCounter->isAlive()) {
        const al::IUseLayoutAction& layoutAction = *mCoinCounter;
        return al::isActionEnd(&layoutAction);
    }

    return false;
}

void StageSceneLayout::exeWait() {
    if (al::isFirstStep(this))
        startActionAll("Wait");
}

void StageSceneLayout::exeEnd() {
    if (al::isFirstStep(this)) {
        startActionAll("End");

        mPlayGuideMenu->end();
        mCounterLifeCtrl->end();

        if (!rs::isModeE3Rom() && !rs::isModeE3LiveRom())
            mMapMini->end();

        mPlayGuideBgm->end();
        mShineCounter->tryEnd();
        mCoinCounter->tryEnd();
        mCoinCollectCounter->tryEnd();
        al::killLayoutIfActive(mLayoutActorKidsMode);
    }

    if (isActionEndAll())
        al::killLayoutIfActive(mShineChipLayoutParts);
}

void StageSceneLayout::exeEndWithoutCoin() {
    if (!al::isFirstStep(this))
        return;

    appearCoinCounterForDemo();

    mShineCounter->tryEnd();

    startActionAll("End");

    mPlayGuideMenu->end();

    if (!rs::isModeE3Rom() && !rs::isModeE3LiveRom())
        mMapMini->end();

    mPlayGuideBgm->end();

    if (!mIsOnlyCoin)
        mCounterLifeCtrl->end();
}

void StageSceneLayout::exeCoinCountAnim() {}

void StageSceneLayout::exeShineChipComplete() {}

void StageSceneLayout::exeShineCountAppear() {}

void StageSceneLayout::updateLifeCounter() {
    mCounterLifeCtrl->updateNerve();
}

void StageSceneLayout::updateKidsModeLayout() {
    if (rs::isKidsModeLayoutDisable(mLayoutActorKidsMode)) {
        al::killLayoutIfActive(mLayoutActorKidsMode);
        return;
    }

    if (al::isNerve(this, &Wait)) {
        al::LayoutActor* kidsModeLayout = mLayoutActorKidsMode;
        if (rs::isKidsMode(kidsModeLayout))
            al::appearLayoutIfDead(kidsModeLayout);
    }
}

void StageSceneLayout::startShineCountAnim(bool isAddTenShines) {
    al::NerveStateBase::appear();
    mShineCounter->startCountAnim(isAddTenShines);
    al::setNerve(this, &ShineCountAppear);
}

bool StageSceneLayout::isEndShineCountAnim() const {
    return mShineCounter->isEndCountAnim();
}
