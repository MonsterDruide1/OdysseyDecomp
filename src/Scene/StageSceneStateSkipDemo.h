#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Scene;
class WindowConfirm;
class WipeHolder;
class DemoSyncedEventKeeper;
}  // namespace al

class PlayGuideSkip;
class SceneAudioSystemPauseController;
class StageSceneStateWorldMap;

class StageSceneStateSkipDemo : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateSkipDemo(const char*, al::Scene*, al::WindowConfirm*, al::WipeHolder*,
                            PlayGuideSkip*, SceneAudioSystemPauseController*,
                            al::DemoSyncedEventKeeper*);
    void setWorldMapState(StageSceneStateWorldMap*);
    void tryAppearSkipDemoLayout();
    bool tryStartSkipDemo();
    bool isDisableSkipByWorldMap() const;
    bool isDemoCancelStageScene() const;
    bool tryEndForNoSkip();
    bool isCancelSkip() const;
    bool isConfirmingOfSkip() const;
    void appear() override;
    void kill() override;
    void exeConfirmSkip();
    void exeWaitConfirmClose();
    void exeSkip();
    void exeWaitSkipEnd();
};
