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
FlowMapParts::FlowMapParts(const char* name) : LiveActor(name) {}

void FlowMapParts::init(const ActorInitInfo& info) {
    const char* suffix = nullptr;
    tryGetStringArg(&suffix, info, "Suffix");
    initMapPartsActor(this, info, suffix);

    s32 interval = 60;
    f32 speed = 1.0f;
    tryGetArg(&interval, info, "Interval");
    tryGetArg(&speed, info, "Speed");

    trySyncStageSwitchAppearAndKill(this);
    registActorToDemoInfo(this, info);
    if (getModelKeeper() && !isExistAction(this) && !isViewDependentModel(this))
        mIsStatic = true;

    mFlowMapCtrl = new FlowMapCtrl(this);
    mFlowMapCtrl->init(interval, speed);
}

void FlowMapParts::appear() {
    LiveActor::appear();
    tryStartAction(this, "Appear");
}

void FlowMapParts::movement() {
    mFlowMapCtrl->update();
    if (mIsStatic)
        return;
    LiveActor::movement();
}

void FlowMapParts::calcAnim() {
    if (mIsStatic) {
        calcViewModel(this);
        return;
    }
    LiveActor::calcAnim();
}

bool FlowMapParts::receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) {
    if (isMsgAskSafetyPoint(message)) {
        if (isValidSwitchAppear(this))
            return false;
        if (isValidSwitchKill(this))
            return false;
        return true;
    }

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
