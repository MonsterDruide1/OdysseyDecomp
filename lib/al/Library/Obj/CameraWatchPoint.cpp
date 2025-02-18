#include "Library/Obj/CameraWatchPoint.h"

#include "Library/Camera/ActorCameraSubTarget.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Project/Camera/CameraSubTargetTurnParam.h"

namespace al {
CameraWatchPoint::CameraWatchPoint(const char* name) : LiveActor(name) {}

void CameraWatchPoint::init(const ActorInitInfo& info) {
    initActorSceneInfo(this, info);
    initActorPoseTRSV(this);
    initActorSRT(this, info);
    initActorClipping(this, info);
    invalidateClipping(this);
    initExecutorWatchObj(this, info);
    initStageSwitch(this, info);

    mCameraSubTarget = createActorCameraSubTarget(this, nullptr);

    CameraSubTargetTurnParam* params = new CameraSubTargetTurnParam();
    params->init(info);

    initCameraSubTargetTurnParam(mCameraSubTarget, params);
    trySyncStageSwitchAppearAndKill(this);
}

void CameraWatchPoint::makeActorAlive() {
    LiveActor::makeActorAlive();

    setCameraPlacementSubTarget(this, mCameraSubTarget);
}

void CameraWatchPoint::kill() {
    LiveActor::kill();

    resetCameraPlacementSubTarget(this, mCameraSubTarget);
}
}  // namespace al
