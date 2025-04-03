#include "Library/Stage/StageSwitchWatcher.h"

#include "Library/Stage/StageSwitchListener.h"
#include "Project/Stage/StageSwitchAccesser.h"

namespace al {

StageSwitchWatcher::StageSwitchWatcher(StageSwitchListener* switchListener,
                                       StageSwitchAccesser* switchAccesser)
    : mSwitchListener(switchListener), mSwitchAccesser(switchAccesser) {}

void StageSwitchWatcher::update() {
    bool isOn = mSwitchAccesser->isOnSwitch();
    if (isOn != mIsSwitchOn) {
        if (isOn)
            mSwitchListener->listenOn();
        else
            mSwitchListener->listenOff();
    }
    mIsSwitchOn = isOn;
}

}  // namespace al
