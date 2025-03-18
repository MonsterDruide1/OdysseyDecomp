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
    void* filler[2];
};
