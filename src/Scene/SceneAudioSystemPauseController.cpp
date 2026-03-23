#include "Scene/SceneAudioSystemPauseController.h"

#include "Library/Audio/AudioDirector.h"
#include "Library/Audio/System/AudioKeeperFunction.h"

SceneAudioSystemPauseController::SceneAudioSystemPauseController(al::AudioDirector* director)
    : mAudioDirector(director) {}

void SceneAudioSystemPauseController::update() {
    if (mResumeDelay > -1) {
        tryResume();
        mResumeDelay -= 1;
    }
}

void SceneAudioSystemPauseController::tryResume() {
    if (mResumeDelay != 0)
        return;

    if (mIsPausedBgm) {
        alAudioSystemFunction::pauseBgmDirector(mAudioDirector, false, 0);
        mIsPausedBgm = false;
    } else
        alAudioSystemFunction::pauseAudioDirector(mAudioDirector, false, 0);
    mIsPaused = false;
    mResumeDelay = -1;
}

void SceneAudioSystemPauseController::pause(bool pauseBgm) {
    if (mIsPaused) {
        mResumeDelay = -1;
        return;
    }

    mIsPausedBgm = pauseBgm;

    if (pauseBgm)
        alAudioSystemFunction::pauseBgmDirector(mAudioDirector, true, 0);
    else
        alAudioSystemFunction::pauseAudioDirector(mAudioDirector, true, 0);

    mIsPaused = true;
}

void SceneAudioSystemPauseController::resume(s32 resumeDelay) {
    if (!mIsPaused)
        return;

    mResumeDelay = resumeDelay;

    if (resumeDelay == 0)
        tryResume();
}
