#include "Scene/StageSceneStatePauseMenu.h"

#include <nn/oe.h>

#include "Library/Base/StringUtil.h"
#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Controller/KeyRepeatCtrl.h"
#include "Library/Draw/GraphicsFunction.h"
#include "Library/Effect/EffectSystem.h"
#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSceneInfo.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorKit.h"
#include "Library/LiveActor/SubActorKeeper.h"
#include "Library/Math/MathUtil.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/PartsModel.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"
#include "Library/Play/Layout/WindowConfirm.h"
#include "Library/Play/Layout/WipeSimple.h"
#include "Library/Scene/SceneUtil.h"
#include "Library/Screen/ScreenFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/System/GameSystemInfo.h"
#include "Project/Scene/SceneInitInfo.h"

#include "Layout/FooterParts.h"
#include "Layout/MenuSelectParts.h"
#include "Layout/StageSceneLayout.h"
#include "Scene/SceneAudioSystemPauseController.h"
#include "Scene/StageScene.h"
#include "Scene/StageSceneStateEndSeparatePlay.h"
#include "Scene/StageSceneStateOption.h"
#include "Scene/StageSceneStateStartSeparatePlay.h"
#include "Sequence/GameSequenceInfo.h"
#include "System/GameDataHolder.h"
#include "System/SaveDataAccessFunction.h"
#include "Util/HelpFunction.h"
#include "Util/InputInterruptTutorialUtil.h"
#include "Util/SpecialBuildUtil.h"
#include "Util/StageInputFunction.h"

namespace {
NERVE_IMPL(StageSceneStatePauseMenu, Appear);
NERVE_IMPL(StageSceneStatePauseMenu, StartSeparatePlay);
NERVE_IMPL(StageSceneStatePauseMenu, EndSeparatePlay);
NERVE_IMPL(StageSceneStatePauseMenu, Option);
NERVE_IMPL_(StageSceneStatePauseMenu, OptionFromHelp, Option);
NERVE_IMPL(StageSceneStatePauseMenu, Wait);
NERVE_IMPL(StageSceneStatePauseMenu, End);
NERVE_IMPL(StageSceneStatePauseMenu, WaitDraw);
NERVE_IMPL(StageSceneStatePauseMenu, Save);
NERVE_IMPL(StageSceneStatePauseMenu, ConfirmNewGame);
NERVE_IMPL(StageSceneStatePauseMenu, NotExistEmptyFile);
NERVE_IMPL(StageSceneStatePauseMenu, FadeBeforeHelp);
NERVE_IMPL(StageSceneStatePauseMenu, StartHelp);
NERVE_IMPL_(StageSceneStatePauseMenu, StartHelpFromOption, StartHelp);

NERVES_MAKE_NOSTRUCT(StageSceneStatePauseMenu, StartHelp);
NERVES_MAKE_STRUCT(StageSceneStatePauseMenu, Appear, StartSeparatePlay, EndSeparatePlay, Option,
                   OptionFromHelp, Wait, End, WaitDraw, Save, ConfirmNewGame, NotExistEmptyFile,
                   FadeBeforeHelp, StartHelpFromOption);
}  // namespace

StageSceneStatePauseMenu::StageSceneStatePauseMenu(
    const char* name, al::Scene* host, al::SimpleLayoutAppearWaitEnd* menuLayout,
    GameDataHolder* gameDataHolder, const al::SceneInitInfo& sceneInitInfo,
    const al::ActorInitInfo& actorInitInfo, const al::LayoutInitInfo& layoutInitInfo,
    al::WindowConfirm* windowConfirm, StageSceneLayout* stageSceneLayout, bool isTitle,
    SceneAudioSystemPauseController* sceneAudioSystemPauseController)
    : al::HostStateBase<al::Scene>(name, host), mMenuLayout(menuLayout),
      mGameDataHolderAccessor(gameDataHolder), mWindowConfirm(windowConfirm),
      mStageSceneLayout(stageSceneLayout),
      mSceneAudioSystemPauseController(sceneAudioSystemPauseController) {
    mMarioActor = new al::LiveActor("メニュー用マリオモデル");

    al::initActorWithArchiveName(mMarioActor, actorInitInfo, "MarioHigh", "PauseMenu");
    mMarioActor->makeActorDead();

    al::PartsModel* capManEyes = new al::PartsModel("メニュー用キャップ目");
    al::LiveActor* marioHat = al::getSubActor(mMarioActor, "頭");
    capManEyes->initPartsFixFile(marioHat, actorInitInfo, "CapManHeroHighEyes", nullptr, nullptr);
    capManEyes->makeActorDead();
    al::onSyncAppearSubActor(marioHat, capManEyes);
    al::startAction(capManEyes, "Wait");

    mSelectParts = new MenuSelectParts("[ポーズメニュー]選択肢", mMenuLayout, mMarioActor,
                                       layoutInitInfo, cMenuItemAmount + 1);
    mMenuGuide = new al::SimpleLayoutAppearWaitEnd("[ポーズメニュー]フッター", "MenuGuide",
                                                   layoutInitInfo, nullptr, false);
    mMenuRight = new al::SimpleLayoutAppearWaitEnd("[ポーズメニュー]権利", "MenuRight",
                                                   layoutInitInfo, nullptr, false);

    nn::oe::DisplayVersion displayVersion;
    nn::oe::GetDisplayVersion(&displayVersion);
    if (al::isEqualString(displayVersion.name, "1.0.0"))
        al::hidePane(mMenuRight, "TxtVersion");
    else
        al::setPaneStringFormat(mMenuRight, "TxtVersion", displayVersion.name);

    mFooterParts =
        new FooterParts(mMenuGuide, layoutInitInfo,
                        al::getSystemMessageString(mMenuGuide, "Footer", "MenuMessage_Footer"),
                        "TxtGuide", "ParFooter");
    mMenuGuide->kill();
    mMenuWipe = new al::WipeSimple("メニュー黒フェード", "FadeBlack", layoutInitInfo, nullptr);
    mMenuWipe->kill();
    mHelpWipe = new al::WipeSimple("ヘルプ黒フェード", "FadeBlack", layoutInitInfo, "BeforeWindow");
    mHelpWipe->kill();

    mPauseCameraCtrl = al::initAndCreatePauseCameraCtrl(getHost(), 10.0f);
    mKeyRepeatCtrl = new al::KeyRepeatCtrl();
    mKeyRepeatCtrl->init(30, 5);

    initNerve(&NrvStageSceneStatePauseMenu.Appear, 4);

    al::GamePadSystem* gamePadSystem = sceneInitInfo.gameSysInfo->gamePadSystem;

    mStateStartSeparatePlay = new StageSceneStateStartSeparatePlay(
        "おすそ分け開始", this, layoutInitInfo, mMenuWipe, gamePadSystem, mFooterParts);
    mStateEndSeparatePlay = new StageSceneStateEndSeparatePlay(
        "おすそ分け終了", this, layoutInitInfo, mMenuWipe, gamePadSystem);
    mStateOption = new StageSceneStateOption("オプション画面", getHost(), layoutInitInfo,
                                             mFooterParts, mGameDataHolderAccessor, isTitle);

    al::initNerveState(this, mStateStartSeparatePlay,
                       &NrvStageSceneStatePauseMenu.StartSeparatePlay, "おすそ分けプレイ開始");
    al::initNerveState(this, mStateEndSeparatePlay, &NrvStageSceneStatePauseMenu.EndSeparatePlay,
                       "おすそ分けプレイ終了");
    al::initNerveState(this, mStateOption, &NrvStageSceneStatePauseMenu.Option, "オプション画面");
    al::addNerveState(this, mStateOption, &NrvStageSceneStatePauseMenu.OptionFromHelp,
                      "オプション画面[ヘルプから遷移]");

    rs::registerGraphicsPresetPause(getHost());
    mHtmlViewer = sceneInitInfo.gameSysInfo->htmlViewer;
}

void StageSceneStatePauseMenu::appear() {
    mStartType = StartType::Title;
    setDead(false);
    if (rs::isModeE3LiveRom()) {
        if (rs::isSeparatePlay(getHost()))
            al::setNerve(this, &NrvStageSceneStatePauseMenu.EndSeparatePlay);
        else
            al::setNerve(this, &NrvStageSceneStatePauseMenu.StartSeparatePlay);
        return;
    }
    al::setNerve(this, &NrvStageSceneStatePauseMenu.Appear);
}

void StageSceneStatePauseMenu::kill() {
    rs::updateGyroText(getHost());

    setDead(true);
    killPauseMenu();
    killMarioModel();  // redundant as killPauseMenu() already does this

    if (!isNeedKillHost() && mSceneAudioSystemPauseController) {
        mSceneAudioSystemPauseController->resume(1);
        al::endPausePadRumble(getHost());
    }

    alGraphicsFunction::validateGpuStressAnalyzer(getHost());
}

void StageSceneStatePauseMenu::killPauseMenu() {
    mMenuLayout->kill();
    mMenuRight->kill();
    mMenuGuide->kill();

    al::endCameraPause(mPauseCameraCtrl);
    killMarioModel();
}

void StageSceneStatePauseMenu::killMarioModel() {
    if (mIsShowMarioModel)
        al::setNearClipDistance(getHost(), mPrevNearClipDistance, 0);

    if (al::isAlive(mMarioActor))
        mMarioActor->kill();
}

bool StageSceneStatePauseMenu::isNeedKillHost() const {
    return isLoadData() || isNewGame() || isChangeLanguage() || isModeSelectEnd();
}

void StageSceneStatePauseMenu::startNormal() {
    mStartType = StartType::Normal;
    al::pausePadRumble(getHost());
    mSceneAudioSystemPauseController->pause(false);
    alGraphicsFunction::invalidateGpuStressAnalyzer(getHost());
}

void StageSceneStatePauseMenu::startAfterTitle() {
    mStartType = StartType::AfterTitle;
}

void StageSceneStatePauseMenu::killAllOptionLayout() {
    mStateOption->killAllLayouts();
}

bool StageSceneStatePauseMenu::isEndToCancel() const {
    return !mSelectParts->isDecideSetting();
}

bool StageSceneStatePauseMenu::isEndToHelp() const {
    return false;
}

bool StageSceneStatePauseMenu::isLoadData() const {
    return mStateOption->isLoadData();
}

s32 StageSceneStatePauseMenu::getSelectedFileId() const {
    return mStateOption->getSelectedFileId();
}

bool StageSceneStatePauseMenu::isChangeLanguage() const {
    return mStateOption->isChangeLanguage();
}

const char* StageSceneStatePauseMenu::getLanguage() const {
    return mStateOption->getLanguage();
}

bool StageSceneStatePauseMenu::isNewGame() const {
    return mIsNewGame;
}

bool StageSceneStatePauseMenu::isModeSelectEnd() const {
    return mStateOption->isModeSelectEnd();
}

bool StageSceneStatePauseMenu::checkNeedKillByHostAndEnd() {
    if (!isNeedKillHost())
        return false;

    getHost()->getLiveActorKit()->getEffectSystem()->set_69(false);
    mSceneAudioSystemPauseController->resume(0);
    alSeFunction::stopAllSe(getHost()->getAudioDirector(), 0);
    al::stopAllBgm(getHost(), 0);
    return true;
}

void StageSceneStatePauseMenu::startActionMario(const char* actionName) {
    mSelectParts->startActionMario(mMarioActor, actionName);
}

al::LiveActor* StageSceneStatePauseMenu::getMarioActor() const {
    return mMarioActor;
}

bool StageSceneStatePauseMenu::isDrawLayout() const {
    return !al::isNerve(this, &NrvStageSceneStatePauseMenu.Wait) &&
           !al::isNerve(this, &NrvStageSceneStatePauseMenu.Appear);
}

bool StageSceneStatePauseMenu::isDrawLayoutMain() const {
    return !al::isNerve(this, &NrvStageSceneStatePauseMenu.End) &&
           !al::isNerve(this, &NrvStageSceneStatePauseMenu.WaitDraw);
}

bool StageSceneStatePauseMenu::isDrawViewRenderer() const {
    if (mStateStartSeparatePlay->isDrawViewRenderer() ||
        mStateEndSeparatePlay->isDrawViewRenderer() ||
        al::isNerve(this, &NrvStageSceneStatePauseMenu.WaitDraw) ||
        al::isNerve(this, &NrvStageSceneStatePauseMenu.End))
        return true;

    if (al::isNerve(this, &NrvStageSceneStatePauseMenu.Appear))
        return al::isDead(mMarioActor);

    return false;
}

bool StageSceneStatePauseMenu::isDrawChromakey() const {
    if (al::isNerve(this, &NrvStageSceneStatePauseMenu.Appear) ||
        al::isNerve(this, &NrvStageSceneStatePauseMenu.Wait) ||
        al::isNerve(this, &NrvStageSceneStatePauseMenu.StartSeparatePlay) ||
        al::isNerve(this, &NrvStageSceneStatePauseMenu.EndSeparatePlay))
        return true;

    return !isDrawViewRenderer();
}

void StageSceneStatePauseMenu::exeAppear() {
    if (al::isFirstStep(this))
        startPauseCamera();

    if (al::isStep(this, 1)) {
        if (mStartType == StartType::Normal)
            setNormal();
        if (mStartType == StartType::AfterTitle) {
            mMenuRight->appear();
            mMenuGuide->appear();
            al::startAction(mMenuLayout, "SelectTitle", "Select");
        }
    }

    if (al::isStep(this, 2))
        appearMarioModel();

    updatePlayerPose();
    al::updateKitListPrev(getHost());
    if (al::isGreaterEqualStep(this, 2) || !mIsNormal) {
        rs::requestGraphicsPresetAndCubeMapPause(getHost());
        alGraphicsFunction::requestUpdateMaterialInfo(getHost());
    }

    al::updateKitList(getHost(), "デモ");
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    if (al::isGreaterEqualStep(this, 3) && mMenuLayout->isWait())
        al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
}

void StageSceneStatePauseMenu::setNormal() {
    mIsNormal = true;

    if (mMenuLayout->isAlive())
        mSelectParts->appearWait();
    else {
        mMenuLayout->appear();
        mMenuRight->appear();
        mMenuGuide->appear();
    }

    mSelectParts->appear(cMenuItemAmount);
    mSelectParts->setSelectMessage(
        MenuSelectParts::Selection::Continue,
        al::getSystemMessageString(mMenuLayout, "MenuMessage", "Continue"));

    if (rs::isSeparatePlay(getHost()))
        mSelectParts->setSelectMessage(
            MenuSelectParts::Selection::SeparatePlay,
            al::getSystemMessageString(mMenuLayout, "MenuMessage", "EndSeparatePlay"));
    else
        mSelectParts->setSelectMessage(
            MenuSelectParts::Selection::SeparatePlay,
            al::getSystemMessageString(mMenuLayout, "MenuMessage", "StartSeparatePlay"));

    al::startAction(mMenuLayout, "SelectPause", "Select");
}

void StageSceneStatePauseMenu::appearMarioModel() {
    if (!mIsShowMarioModel)
        mPrevNearClipDistance = alCameraFunction::getNearClipDistance(getHost(), 0);

    al::setNearClipDistance(getHost(), 400.0f, 0);
    updatePlayerPose();
    mMarioActor->appear();
    mIsShowMarioModel = true;
}

void StageSceneStatePauseMenu::updatePlayerPose() {
    sead::Vector3f worldPos = sead::Vector3f::zero;
    al::calcWorldPosFromLayoutPos(&worldPos, getHost(), {310.0f, -960.0f}, 500.0f);
    al::setTrans(mMarioActor, worldPos);

    sead::Quatf quat = sead::Quatf::unit;
    sead::Vector3f cameraFront = sead::Vector3f::zero;
    al::calcCameraFront(&cameraFront, getHost(), 0);
    al::makeQuatFrontUp(&quat, -cameraFront, al::getCameraUp(getHost(), 0));
    al::rotateQuatYDirDegree(&quat, quat, 360.0f);
    al::updatePoseQuat(mMarioActor, quat);
}

void StageSceneStatePauseMenu::exeWait() {
    if (mIsNormal) {
        if (rs::isSeparatePlay(getHost()))
            mSelectParts->setSelectMessage(
                MenuSelectParts::Selection::SeparatePlay,
                al::getSystemMessageString(mMenuLayout, "MenuMessage", "EndSeparatePlay"));
        else
            mSelectParts->setSelectMessage(
                MenuSelectParts::Selection::SeparatePlay,
                al::getSystemMessageString(mMenuLayout, "MenuMessage", "StartSeparatePlay"));
    }

    al::updateKitListPrev(getHost());
    al::updateKitList(getHost(), "カメラ");
    rs::requestGraphicsPresetAndCubeMapPause(getHost());
    mSelectParts->update();
    updatePlayerPose();
    al::updateKitList(getHost(), "デモ");
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");

    if (mSelectParts->isDecideSave()) {
        al::setNerve(this, &NrvStageSceneStatePauseMenu.Save);
    }

    else if (mSelectParts->isDecideContinue()) {
        killMarioModel();
        al::endCameraPause(mPauseCameraCtrl);
        al::setNerve(this, &NrvStageSceneStatePauseMenu.WaitDraw);
    }

    else if (mSelectParts->isDecideSeparatePlay()) {
        if (rs::isSeparatePlay(getHost()) && !mIsNormal) {
            al::setNerve(this, &NrvStageSceneStatePauseMenu.WaitDraw);
            return;
        }
        if (rs::isSeparatePlay(getHost()))
            al::setNerve(this, &NrvStageSceneStatePauseMenu.EndSeparatePlay);
        else
            al::setNerve(this, &NrvStageSceneStatePauseMenu.StartSeparatePlay);

    }

    else if (mSelectParts->isDecideNewGame()) {
        if (mGameDataHolderAccessor->tryFindEmptyFileId() < 0)
            al::setNerve(this, &NrvStageSceneStatePauseMenu.NotExistEmptyFile);
        else
            al::setNerve(this, &NrvStageSceneStatePauseMenu.ConfirmNewGame);
    }

    else if (mSelectParts->isDecideHelp()) {
        al::setNerve(this, &NrvStageSceneStatePauseMenu.FadeBeforeHelp);
    }

    else if (!isEndToCancel()) {
        al::setNerve(this, &NrvStageSceneStatePauseMenu.Option);
    }

    else
        al::updateKitListPost(getHost());
}

void StageSceneStatePauseMenu::changeNerveAndReturn(const al::Nerve* nerve) {
    al::setNerve(this, nerve);
}

void StageSceneStatePauseMenu::exeFadeBeforeHelp() {
    if (al::isFirstStep(this))
        mHelpWipe->startClose(-1);

    al::updateKitListPrev(getHost());
    al::updateKitList(getHost(), "カメラ");
    rs::requestGraphicsPresetAndCubeMapPause(getHost());
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    if (mHelpWipe->isCloseEnd())
        al::setNerve(this, &StartHelp);
}

void StageSceneStatePauseMenu::exeStartHelp() {
    al::updateKitListPrev(getHost());
    al::updateKitList(getHost(), "カメラ");
    rs::requestGraphicsPresetAndCubeMapPause(getHost());
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    if (al::isFirstStep(this)) {
        bool isStartHelpFromOption =
            al::isNerve(this, &NrvStageSceneStatePauseMenu.StartHelpFromOption);
        bool isSceneStatusInvalidSave = rs::isSceneStatusInvalidSave(getHost());

        al::StringTmp<256> helpUrl;
        HelpFunction::callHelp(mHtmlViewer, mGameDataHolderAccessor, isStartHelpFromOption,
                               isSceneStatusInvalidSave, false, &helpUrl);

        if (al::isEqualString("http://localhost/ChangeMode", helpUrl.cstr())) {
            mStateOption->set_51(true);
            al::setNerve(this, &NrvStageSceneStatePauseMenu.OptionFromHelp);
            return;
        }
        mHelpWipe->startOpen(-1);
    }

    if (mHelpWipe->isOpenEnd()) {
        mSelectParts->appearWait();
        al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
    }
}

void StageSceneStatePauseMenu::exeWaitDraw() {
    al::updateKitListPrev(getHost());
    al::updateKitList(getHost(), "カメラ");
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());
    if (al::isFirstStep(this))
        alGraphicsFunction::requestUpdateMaterialInfo(getHost());
    if (al::isGreaterEqualStep(this, 2))
        al::setNerve(this, &NrvStageSceneStatePauseMenu.End);
}

void StageSceneStatePauseMenu::exeEnd() {
    al::updateKitListPrev(getHost());
    al::updateKitList(getHost(), "カメラ");
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());
    if (al::isFirstStep(this)) {
        mMenuLayout->end();
        mMenuRight->end();
        mMenuGuide->end();
    }
    if (al::isDead(mMenuLayout))
        kill();
}

void StageSceneStatePauseMenu::exeStartSeparatePlay() {
    if (al::isFirstStep(this)) {
        mFooterParts->tryChangeTextFade(
            al::getSystemMessageString(mFooterParts, "Footer", "MenuMessage_Footer_SeparatePlay"));
    }
    al::updateKitListPrev(getHost());
    al::updateKitList(getHost(), "カメラ");
    if (mStateStartSeparatePlay->isNeedRequestGraphicsPreset())
        rs::requestGraphicsPresetAndCubeMapPause(getHost());
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    if (al::updateNerveState(this)) {
        if (mStageSceneLayout)
            mStageSceneLayout->setDirtyFlagForPlayGuideMenu();
        if (mStateStartSeparatePlay->isCancel() && !rs::isModeE3LiveRom()) {
            mSelectParts->appearWait();
            al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
            return;
        }
        kill();
    }
}

void StageSceneStatePauseMenu::exeEndSeparatePlay() {
    al::updateKitListPrev(getHost());
    al::updateKitList(getHost(), "カメラ");
    if (mStateEndSeparatePlay->isNeedRequestGraphicsPreset())
        rs::requestGraphicsPresetAndCubeMapPause(getHost());
    al::updateKitList(getHost(), "シャドウマスク");
    al::updateKitList(getHost(), "グラフィックス要求者");
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    if (al::updateNerveState(this)) {
        if (mStageSceneLayout)
            mStageSceneLayout->setDirtyFlagForPlayGuideMenu();

        if (mStateEndSeparatePlay->isCancel() && !rs::isModeE3LiveRom()) {
            mSelectParts->appearWait();
            al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
            return;
        }
        kill();
    }
}

void StageSceneStatePauseMenu::exeOption() {
    al::updateKitListPrev(getHost());
    rs::requestGraphicsPresetAndCubeMapPause(getHost());
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    if (al::updateNerveState(this)) {
        if (isChangeLanguage() || isLoadData()) {
            kill();
            return;
        }

        mSelectParts->appearWait();
        mFooterParts->tryChangeTextFade(
            al::getSystemMessageString(mMenuGuide, "Footer", "MenuMessage_Footer"));

        if (al::isNerve(this, &NrvStageSceneStatePauseMenu.OptionFromHelp))
            al::setNerve(this, &NrvStageSceneStatePauseMenu.StartHelpFromOption);
        else
            al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
    }
}

void StageSceneStatePauseMenu::exeSave() {
    al::updateKitListPrev(getHost());
    rs::requestGraphicsPresetAndCubeMapPause(getHost());
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    if (al::isFirstStep(this))
        SaveDataAccessFunction::startSaveDataWriteWithWindow(mGameDataHolderAccessor);

    if (SaveDataAccessFunction::updateSaveDataAccess(mGameDataHolderAccessor, false)) {
        mSelectParts->appearWait();
        al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
    }
}

void StageSceneStatePauseMenu::exeConfirmNewGame() {
    if (al::isFirstStep(this)) {
        mWindowConfirm->setListNum(2);

        mWindowConfirm->setTxtMessage(
            al::getSystemMessageString(mWindowConfirm, "ConfirmMessage", "NewGame"));
        mWindowConfirm->setTxtList(
            0, al::getSystemMessageString(mWindowConfirm, "ConfirmMessage", "NewGame_Yes"));
        mWindowConfirm->setTxtList(
            1, al::getSystemMessageString(mWindowConfirm, "ConfirmMessage", "NewGame_No"));

        mWindowConfirm->appear();
        mKeyRepeatCtrl->reset();
    }

    al::updateKitListPrev(getHost());
    rs::requestGraphicsPresetAndCubeMapPause(getHost());
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    mKeyRepeatCtrl->update(rs::isHoldUiUp(getHost()), rs::isHoldUiDown(getHost()));

    if (mKeyRepeatCtrl->isUp())
        mWindowConfirm->tryUp();

    if (mKeyRepeatCtrl->isDown())
        mWindowConfirm->tryDown();

    al::WindowConfirm::SelectionType selectionType = mWindowConfirm->getPrevSelectionType();

    if (rs::isTriggerUiDecide(getHost())) {
        if (selectionType == al::WindowConfirm::SelectionType::HardKey)
            mWindowConfirm->tryDecideWithoutEnd();
        if (selectionType == al::WindowConfirm::SelectionType::List00)
            mWindowConfirm->tryCancel();
    }

    if (rs::isTriggerUiCancel(getHost()))
        mWindowConfirm->tryCancel();

    if (selectionType == al::WindowConfirm::SelectionType::HardKey &&
        mWindowConfirm->isNerveEnd()) {
        mWindowConfirm->tryDecide();
        s32 emptyFileId = mGameDataHolderAccessor->tryFindEmptyFileId();
        mGameDataHolderAccessor->requestSetPlayingFileId(emptyFileId);
        mGameDataHolderAccessor->setRequireSave();
        mIsNewGame = true;
    }

    if (selectionType == al::WindowConfirm::SelectionType::List00) {
        if (al::isDead(mWindowConfirm)) {
            mSelectParts->appearWait();
            al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
        }
    }
}

void StageSceneStatePauseMenu::exeNotExistEmptyFile() {
    if (al::isFirstStep(this)) {
        mWindowConfirm->setListNum(1);
        mWindowConfirm->setTxtMessage(
            al::getSystemMessageString(mWindowConfirm, "ConfirmMessage", "NotExistEmptyFile"));
        mWindowConfirm->appear();
    }

    al::updateKitListPrev(getHost());
    rs::requestGraphicsPresetAndCubeMapPause(getHost());
    al::updateKitList(getHost(), "２Ｄ（ポーズ無視）");
    al::updateKitListPost(getHost());

    if (rs::isTriggerUiDecide(getHost()) || rs::isTriggerUiCancel(getHost()))
        mWindowConfirm->tryDecide();

    if (al::isDead(mWindowConfirm)) {
        mSelectParts->appearWait();
        al::setNerve(this, &NrvStageSceneStatePauseMenu.Wait);
    }
}

void StageSceneStatePauseMenu::startPauseCamera() {
    al::startCameraPause(mPauseCameraCtrl);
}

void StageSceneStatePauseMenu::setAfterTitle() {
    mIsNormal = false;

    mSelectParts->appear(cMenuItemAmount);
    mSelectParts->setMainMenu(true);

    mSelectParts->setSelectMessage(
        MenuSelectParts::Selection::Continue,
        al::getSystemMessageString(mMenuLayout, "MenuMessage", "StartGame"));
    mSelectParts->setSelectMessage(
        MenuSelectParts::Selection::SeparatePlay,
        al::getSystemMessageString(mMenuLayout, "MenuMessage", "StartGameSeparatePlay"));
    mSelectParts->setSelectMessage(
        MenuSelectParts::Selection::NewGame,
        al::getSystemMessageString(mMenuLayout, "MenuMessage", "NewGame"));
}
