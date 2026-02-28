#pragma once

#include <basis/seadTypes.h>

namespace al {
class AudioDirector;
}

class SceneAudioSystemPauseController {
public:
    SceneAudioSystemPauseController(al::AudioDirector*);
    void update();
    void tryResume();
    void pause(bool);
    void resume(s32);

private:
    al::AudioDirector* mAudioDirector;
    s32 mResumeDelay = -1;
    bool mIsPausedBgm = false;
    bool mIsPaused = false;
};

static_assert(sizeof(SceneAudioSystemPauseController) == 0x10);
