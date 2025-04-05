#pragma once

#include "Library/Scene/Scene.h"

namespace al {
class ChromakeyDrawer;
class SimpleLayoutAppearWaitEnd;
class WindowConfirm;
class WipeSimple;
}  // namespace al
class StageSceneStatePauseMenu;
class BootLayout;

class TitleMenuScene : public al::Scene {
public:
    TitleMenuScene();
    ~TitleMenuScene() override;

    void init(const al::SceneInitInfo& info) override;
    void appear() override;
    void control() override;
    void drawMain() const override;
    bool isEnableKill() const;
    bool isChangeLanguage() const;
    const char* getLanguage() const;
    bool isNewGame() const;
    void startLoadDirect(bool);
    void setScenario();
    void setLoadPercent(f32 percent);
    bool isCancelLoadWorldResource() const;
    void startLoadWorldResource();
    void exeAppear();
    void exeMenu();
    void exeWipe();
    void exeLoadAppear();
    void exeLoadWait();

private:
    StageSceneStatePauseMenu* mStatePauseMenu = nullptr;
    al::ChromakeyDrawer* mChromakeyDrawer = nullptr;
    al::SimpleLayoutAppearWaitEnd* mLayoutMenu = nullptr;
    al::SimpleLayoutAppearWaitEnd* mLayoutContinueLoading = nullptr;
    al::SimpleLayoutAppearWaitEnd* mLayoutParBG = nullptr;
    al::WindowConfirm* mWindowConfirm = nullptr;
    al::WipeSimple* mFadeBlack = nullptr;
    BootLayout* mBootLayout = nullptr;
    f32 mLoadPercent = 0.0f;
    f32 mLoadPercentBuffered = 0.0f;  // unused
    bool mIsCancelLoadResource = false;
    s32 mLoadCompleteCounter = 0;
};
