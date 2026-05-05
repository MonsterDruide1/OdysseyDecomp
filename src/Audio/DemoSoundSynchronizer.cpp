#include "Audio/DemoSoundSynchronizer.h"

#include <nn/oe.h>

#include "Library/Application/ApplicationMessageReceiver.h"
#include "Library/Audio/AudioDirector.h"
#include "Library/Demo/DemoSyncedEventKeeper.h"
#include "Library/Demo/DemoSyncedSeCtrl.h"
#include "Library/Se/SeKeeper.h"

DemoSoundSynchronizer::DemoSoundSynchronizer(const al::ApplicationMessageReceiver* receiver,
                                             al::AudioDirector* audioDirector)
    : mApplicationMessageReceiver(receiver), mAudioDirector(audioDirector) {}

void DemoSoundSynchronizer::startSync() {
    mIsHandheld =
        mApplicationMessageReceiver->getCachedOperationMode() == nn::oe::OperationMode_Handheld;
    mIsSyncActive = true;
}

void DemoSoundSynchronizer::endSync() {
    mIsHandheld = false;

    if (!mIsSyncActive)
        return;

    resume(0);
    mIsSyncActive = false;
}

void DemoSoundSynchronizer::resume(s32 delay) {
    if (!mIsPaused)
        return;

    mResumeDelay = delay;
    tryResume();
}

void DemoSoundSynchronizer::update() {
    if (mResumeDelay <= -1)
        return;
    tryResume();
    mResumeDelay -= 1;
}

void DemoSoundSynchronizer::tryResume() {
    if (mResumeDelay != 0)
        return;

    mAudioDirector->pauseSystem(false, "サウンド同期ポーズ", true, 0.0, false, false, true);
    mIsPaused = false;
    mResumeDelay = -1;
}

void DemoSoundSynchronizer::trySync(s32 currentTime, al::DemoSyncedEventKeeper* syncedEventKeeper) {
    if (!mIsSyncActive)
        return;

    al::DemoSyncedSeCtrl* seCtrl = syncedEventKeeper->getDemoSyncedSeCtrl();
    if (!seCtrl)
        return;

    const char* name = seCtrl->getCurPlayingSeName();
    if (!name)
        return;

    al::SeKeeper* seKeeper = seCtrl->getSeKeeper();

    trySyncCommon(currentTime, seCtrl->getCurPlayingSeStartFrame(),
                  seKeeper->tryGetCurrentStreamSamplePos(name, nullptr));
}

void DemoSoundSynchronizer::trySyncCommon(s32 currentTime, s32 seStartFrame, s64 sample) {
    mIsHandheld =
        mApplicationMessageReceiver->getCachedOperationMode() == nn::oe::OperationMode_Handheld;

    if (!mAudioDirector->isPpausedSystem("システムポーズ") && sample >= 0) {
        s32 resumeDelay = s32((f32(sample) / 48000.0f) * 60.0f) + (seStartFrame - currentTime);
        if (resumeDelay > 4) {
            pause();
            mResumeDelay = resumeDelay;
        }
    }
}

void DemoSoundSynchronizer::trySync(s32 currentTime, s32 seStartFrame, s64 sample) {
    if (mIsSyncActive)
        trySyncCommon(currentTime, seStartFrame, sample);
}

void DemoSoundSynchronizer::pause() {
    if (!mIsPaused) {
        mAudioDirector->pauseSystem(true, "サウンド同期ポーズ", true, 0.0, false, false, true);
        mIsPaused = true;
    } else
        mResumeDelay = -1;
}
