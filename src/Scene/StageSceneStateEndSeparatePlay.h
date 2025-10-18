#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class GamePadSystem;
class LayoutInitInfo;
class Scene;
class WipeSimple;
}  // namespace al
class StageSceneStatePauseMenu;

class StageSceneStateEndSeparatePlay : public al::HostStateBase<StageSceneStatePauseMenu> {
public:
    StageSceneStateEndSeparatePlay(const char* name, StageSceneStatePauseMenu* host,
                                   const al::LayoutInitInfo& info, al::WipeSimple* wipeSimple,
                                   al::GamePadSystem* gamePadSystem);

    void appear() override;

    bool isNeedRequestGraphicsPreset() const;
    void exeFadeOut();
    void exeApplet();
    al::Scene* getScene();
    void exeFadeIn();
    void exeWaitDraw();
    bool isDrawViewRenderer() const;

    bool isCancel() const { return mIsCancel; }

private:
    al::GamePadSystem* mGamePadSystem = nullptr;
    al::WipeSimple* mWipeSimple = nullptr;
    bool mIsCancel = false;
};
