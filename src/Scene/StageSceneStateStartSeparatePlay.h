#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class GamePadSystem;
class LayoutInitInfo;
class Scene;
class SimpleLayoutAppearWaitEnd;
class WipeSimple;
}  // namespace al
class FooterParts;
class StageSceneStatePauseMenu;

class StageSceneStateStartSeparatePlay : public al::HostStateBase<StageSceneStatePauseMenu> {
public:
    StageSceneStateStartSeparatePlay(const char* name, StageSceneStatePauseMenu* host,
                                     const al::LayoutInitInfo& info, al::WipeSimple* wipeSimple,
                                     al::GamePadSystem* gamePadSystem, FooterParts* footerParts);

    void appear() override;

    void startTreeHouse();
    bool isNeedRequestGraphicsPreset() const;
    bool isDrawViewRenderer() const;
    void exeAppear();
    void exeWait();
    al::Scene* getScene();
    void exeBack();
    void exeFadeOut();
    void exeApplet();
    void exeFadeIn();
    void exeWaitDraw();

    bool isCancel() const { return mIsCancel; }

private:
    al::GamePadSystem* mGamePadSystem = nullptr;
    al::SimpleLayoutAppearWaitEnd* mControllerGuideMulti = nullptr;
    al::WipeSimple* mWipeSimple = nullptr;
    FooterParts* mFooterParts = nullptr;
    u16 field_40 = 0;
    bool mIsCancel = false;
};
