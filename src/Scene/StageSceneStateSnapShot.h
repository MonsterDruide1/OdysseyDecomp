#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class Scene;
}  // namespace al

class ControllerGuideSnapShotCtrl;
class InputSeparator;
class NpcEventDirector;
class SceneAudioSystemPauseController;

class StageSceneStateSnapShot : public al::HostStateBase<al::Scene> {
public:
    StageSceneStateSnapShot(const char*, al::Scene*, ControllerGuideSnapShotCtrl*,
                            SceneAudioSystemPauseController*, InputSeparator*, NpcEventDirector*);
    void appear() override;
    void kill() override;
    bool tryStart();
    void exeWait();

private:
    al::Scene* mScene;
    ControllerGuideSnapShotCtrl* mControllerGuideSnapShotCtrl;
    SceneAudioSystemPauseController* mAudioSystemPauseController;
    InputSeparator* mInputSeparator;
    NpcEventDirector* mNpcEventDirector;
};

static_assert(sizeof(StageSceneStateSnapShot) == 0x48);
