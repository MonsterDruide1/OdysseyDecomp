#include "MapObj/AppearSwitchFixMapParts.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Placement/PlacementFunction.h"

AppearSwitchFixMapParts::AppearSwitchFixMapParts(const char* actorName)
    : al::LiveActor(actorName) {}

void AppearSwitchFixMapParts::init(const al::ActorInitInfo& info) {
    const char* suffix = nullptr;

    al::tryGetStringArg(&suffix, info, "Suffix");
    al::initMapPartsActor(this, info, suffix);
    al::trySyncStageSwitchAppearAndKill(this);
    al::registActorToDemoInfo(this, info);

    if (getModelKeeper() != nullptr && !al::isExistAction(this) && !al::isViewDependentModel(this))
        mIsStatic = true;
}

void AppearSwitchFixMapParts::appear() {
    al::LiveActor::appear();
    al::tryStartAction(this, "Appear");
}

void AppearSwitchFixMapParts::movement() {
    if (!mIsStatic)
        al::LiveActor::movement();
}

void AppearSwitchFixMapParts::calcAnim() {
    if (mIsStatic)
        al::calcViewModel(this);
    else
        al::LiveActor::calcAnim();
}

bool AppearSwitchFixMapParts::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                         al::HitSensor* self) {
    if (al::isMsgAskSafetyPoint(message))
        return false;

    if (al::isMsgShowModel(message)) {
        al::showModelIfHide(this);
        return true;
    }

    if (al::isMsgHideModel(message)) {
        al::hideModelIfShow(this);
        return true;
    }

    return false;
}
