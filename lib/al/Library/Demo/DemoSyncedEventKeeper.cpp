#include "Library/Demo/DemoSyncedEventKeeper.h"

#include "Library/Audio/AudioDirector.h"
#include "Library/Audio/AudioEventController.h"
#include "Library/Demo/DemoSyncedBgmCtrl.h"
#include "Library/Demo/DemoSyncedSeCtrl.h"
#include "Library/Se/SeKeeper.h"

namespace al {

DemoSyncedEventKeeper::DemoSyncedEventKeeper(AudioDirector* audioDirector)
    : mAudioDirector(audioDirector) {
    mDemoSyncedBgmCtrl = DemoSyncedBgmCtrl::tryCreate(mAudioDirector);
    mDemoSyncedSeCtrl = DemoSyncedSeCtrl::tryCreate(mAudioDirector);
}

void DemoSyncedEventKeeper::updateOnlyWhenDemoRun(s32 param) {
    if (mDemoSyncedBgmCtrl)
        mDemoSyncedBgmCtrl->update(param);
    if (mDemoSyncedSeCtrl)
        mDemoSyncedSeCtrl->update(param);
}

void DemoSyncedEventKeeper::finalize() {
    if (mDemoSyncedBgmCtrl)
        mDemoSyncedBgmCtrl->finalize();
    if (mDemoSyncedSeCtrl)
        mDemoSyncedSeCtrl->finalize();
    mDemoName = nullptr;
}

void DemoSyncedEventKeeper::startDemo(const char* demoName) {
    endDemo(false, false);

    alAudioEventControllerFunction::deactivateAudioEventCtrl(mAudioDirector);

    mDemoName = demoName;
    if (mDemoSyncedBgmCtrl)
        mDemoSyncedBgmCtrl->startDemo(demoName);
    if (mDemoSyncedSeCtrl)
        mDemoSyncedSeCtrl->startDemo(mDemoName);
}

// TODO: parameter names
void DemoSyncedEventKeeper::endDemo(bool param_1, bool param_2) {
    if (!mDemoName)
        return;

    if (mDemoSyncedBgmCtrl)
        mDemoSyncedBgmCtrl->endDemo(mDemoName, param_2);
    if (mDemoSyncedSeCtrl)
        mDemoSyncedSeCtrl->endDemo(mDemoName, param_2);

    if (!param_1)
        alAudioEventControllerFunction::activateAudioEventCtrl(mAudioDirector);

    mDemoName = nullptr;
}
}  // namespace al

void alDemoSyncedEventFunction::tryCreateAndSetSeKeeper(al::DemoSyncedEventKeeper* keeper,
                                                        al::AudioDirector* audioDirector,
                                                        const char* seName) {
    if (!seName)
        return;

    al::SeKeeper* seKeeper =
        al::SeKeeper::create(audioDirector->getAudioSystemInfo(), audioDirector->getSeDirector(),
                             seName, nullptr, nullptr, nullptr, nullptr);

    if (al::DemoSyncedSeCtrl* seCtrl = keeper->getDemoSyncedSeCtrl())
        seCtrl->setSeKeeper(seKeeper);
}
