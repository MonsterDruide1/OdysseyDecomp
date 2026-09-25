#include "Player/HackCap.h"

#include "Util/DemoUtil.h"

void HackCap::syncHackDamageVisibility(bool isVisible) {
    mIsHackDamageVisible = isVisible;
}

void HackCap::addHackStartDemo() {
    rs::addDemoActor(this, true);
    rs::addDemoActor(mEquipmentHat, true);
}

void HackCap::addLockOnKeepDemo() {
    rs::addDemoActor(this, true);
    rs::addDemoActor(mEquipmentHat, true);
}
