#include "MapObj/FixMapPartsAppearKillAsync.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Placement/PlacementFunction.h"

FixMapPartsAppearKillAsync::FixMapPartsAppearKillAsync(const char* actorName)
    : al::LiveActor(actorName) {}

void FixMapPartsAppearKillAsync::init(const al::ActorInitInfo& info) {
    const char* suffix = nullptr;

    al::tryGetStringArg(&suffix, info, "Suffix");
    al::initMapPartsActor(this, info, suffix);
    al::tryListenStageSwitchKill(this);
    al::tryListenStageSwitchAppear(this);
}

void FixMapPartsAppearKillAsync::appear() {
    al::LiveActor::appear();
    al::tryStartAction(this, "Appear");
}

void FixMapPartsAppearKillAsync::kill() {
    al::LiveActor::kill();
    al::tryStartAction(this, "Kill");
}

void FixMapPartsAppearKillAsync::movement() {
    if (!mIsCalcViewModel)
        al::LiveActor::movement();
}

void FixMapPartsAppearKillAsync::calcAnim() {
    if (mIsCalcViewModel)
        al::calcViewModel(this);
    else
        al::LiveActor::calcAnim();
}

bool FixMapPartsAppearKillAsync::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                            al::HitSensor* self) {
    if (al::isMsgAskSafetyPoint(message))
        return true;

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
