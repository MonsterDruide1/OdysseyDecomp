#include "Library/Fluid/FlowMapParts.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Project/Fluid/FlowMapCtrl.h"

namespace al {
FlowMapParts::FlowMapParts(const char* name) : al::LiveActor(name) {}

void FlowMapParts::init(const ActorInitInfo& info) {
    const char* suffix = nullptr;
    al::tryGetStringArg(&suffix, info, "Suffix");
    al::initMapPartsActor(this, info, suffix);
    s32 interval = 60;
    f32 speed = 1.0f;
    al::tryGetArg(&interval, info, "Interval");
    al::tryGetArg(&speed, info, "Speed");
    al::trySyncStageSwitchAppearAndKill(this);
    al::registActorToDemoInfo(this, info);
    if (getModelKeeper()) {
        if (!al::isExistAction(this) && !al::isViewDependentModel(this))
            _108 = true;
    }
    mFlowMapCtrl = new FlowMapCtrl(this);
    mFlowMapCtrl->mInterval = interval;
    mFlowMapCtrl->mSpeed = speed;
}

void FlowMapParts::appear() {
    LiveActor::appear();
    al::tryStartAction(this, "Appear");
}

void FlowMapParts::movement() {
    mFlowMapCtrl->update();
    if (!_108)
        LiveActor::movement();
}

void FlowMapParts::calcAnim() {
    if (_108) {
        al::calcViewModel(this);
        return;
    }
    LiveActor::calcAnim();
}

bool FlowMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (al::isMsgAskSafetyPoint(message)) {
        if (al::isValidSwitchAppear(this))
            return false;
        return !al::isValidSwitchKill(this);
    }

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

}  // namespace al
