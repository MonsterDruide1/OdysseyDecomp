#include "Library/MapObj/FixMapParts.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

namespace al {
FixMapParts::FixMapParts(const char* name) : LiveActor(name) {}

void FixMapParts::init(const ActorInitInfo& info) {
    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "Suffix");
    initMapPartsActor(this, info, suffix);
    trySyncStageSwitchAppearAndKill(this);
    registActorToDemoInfo(this, info);

    if (getModelKeeper() && !isExistAction(this) && !isViewDependentModel(this))
        mIsStatic = true;
}

void FixMapParts::appear() {
    LiveActor::appear();

    if (isExistModel(this))
        tryStartAction(this, "Appear");
}

void FixMapParts::movement() {
    if (!mIsStatic)
        LiveActor::movement();
}

void FixMapParts::calcAnim() {
    if (!mIsStatic)
        LiveActor::calcAnim();
    else
        calcViewModel(this);
}

bool FixMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgAskSafetyPoint(message))
        return !isValidSwitchAppear(this) && !isValidSwitchKill(this);

    if (isMsgShowModel(message)) {
        showModelIfHide(this);
        return true;
    }

    if (isMsgHideModel(message)) {
        hideModelIfShow(this);
        return true;
    }

    return false;
}
}  // namespace al
