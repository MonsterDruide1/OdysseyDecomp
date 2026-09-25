#include "Player/HackCap.h"

#include "Player/CapTargetInfo.h"
#include "Player/PlayerCapFunction.h"
#include "Util/DemoUtil.h"
#include "Util/ScenePlayerCameraFunction.h"

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

void HackCap::recordHack() {
    if (!mCapTargetInfo1->isSetHackNameToCamera())
        return;
    PlayerCameraFunction::setCameraHackName(this, CapFunction::getHackObjInfo(this));
}
