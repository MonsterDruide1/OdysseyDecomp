#pragma once

#include "Library/Nerve/NerveStateBase.h"

#include "System/GameDataHolderAccessor.h"

namespace al {
struct ActorInitInfo;
class HtmlViewer;
class KeyRepeatCtrl;
class LayoutInitInfo;
class PauseCameraCtrl;
class Scene;
struct SceneInitInfo;
class SimpleLayoutAppearWaitEnd;
class WindowConfirm;
class WipeSimple;
}  // namespace al
class FooterParts;
class GameDataHolder;
class MenuSelectParts;
class SceneAudioSystemPauseController;
class StageSceneLayout;
class StageSceneStateEndSeparatePlay;
class StageSceneStateOption;
class StageSceneStateStartSeparatePlay;

class StageSceneStatePauseMenu : public al::HostStateBase<al::Scene> {
public:
    static const s32 cMenuItemAmount = 5;
    enum class StartType { Title, Normal, AfterTitle };

    StageSceneStatePauseMenu(const char* name, al::Scene* host,
                             al::SimpleLayoutAppearWaitEnd* menuLayout,
                             GameDataHolder* gameDataHolder, const al::SceneInitInfo& sceneInitInfo,
                             const al::ActorInitInfo& actorInitInfo,
                             const al::LayoutInitInfo& layoutInitInfo,
                             al::WindowConfirm* windowConfirm, StageSceneLayout* stageSceneLayout,
                             bool isTitle,
                             SceneAudioSystemPauseController* sceneAudioSystemPauseController);

    void appear() override;
    void kill() override;

    void killPauseMenu();
    void killMarioModel();
    bool isNeedKillHost() const;
    void startNormal();
    void startAfterTitle();
    void killAllOptionLayout();
    bool isEndToCancel() const;
    bool isEndToHelp() const;
    bool isLoadData() const;
    s32 getSelectedFileId() const;
    bool isChangeLanguage() const;
    const char* getLanguage() const;
    bool isNewGame() const;
    bool isModeSelectEnd() const;
    bool checkNeedKillByHostAndEnd();

    void startActionMario(const char*);
    al::LiveActor* getMarioActor() const;

    bool isDrawLayout() const;
    bool isDrawLayoutMain() const;
    bool isDrawViewRenderer() const;
    bool isDrawChromakey() const;

    void exeAppear();
    void setNormal();
    void appearMarioModel();
    void updatePlayerPose();
    void exeWait();
    void changeNerveAndReturn(const al::Nerve* nerve);
    void exeFadeBeforeHelp();
    void exeStartHelp();
    void exeWaitDraw();
    void exeEnd();
    void exeStartSeparatePlay();
    void exeEndSeparatePlay();
    void exeOption();
    void exeSave();
    void exeConfirmNewGame();
    void exeNotExistEmptyFile();

    void startPauseCamera();
    void setAfterTitle();

private:
    al::SimpleLayoutAppearWaitEnd* mMenuLayout = nullptr;
    al::SimpleLayoutAppearWaitEnd* mMenuGuide = nullptr;
    al::SimpleLayoutAppearWaitEnd* mMenuRight = nullptr;
    FooterParts* mFooterParts;
    MenuSelectParts* mSelectParts = nullptr;
    al::WipeSimple* mMenuWipe = nullptr;
    al::WipeSimple* mHelpWipe = nullptr;
    StartType mStartType = StartType::Title;
    StageSceneStateStartSeparatePlay* mStateStartSeparatePlay = nullptr;
    StageSceneStateEndSeparatePlay* mStateEndSeparatePlay = nullptr;
    StageSceneStateOption* mStateOption = nullptr;
    al::LiveActor* mMarioActor = nullptr;
    char field_0x80[40];
    GameDataHolderAccessor mGameDataHolderAccessor;
    al::PauseCameraCtrl* mPauseCameraCtrl = nullptr;
    al::WindowConfirm* mWindowConfirm = nullptr;
    bool mIsNewGame = false;
    al::KeyRepeatCtrl* mKeyRepeatCtrl = nullptr;
    StageSceneLayout* mStageSceneLayout = nullptr;
    bool mIsNormal = true;
    bool mIsShowMarioModel = false;
    f32 mPrevNearClipDistance = 25.0f;
    SceneAudioSystemPauseController* mSceneAudioSystemPauseController = nullptr;
    al::HtmlViewer* mHtmlViewer = nullptr;
};
