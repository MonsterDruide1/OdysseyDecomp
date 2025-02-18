#include "Library/Obj/EffectObjCameraEmit.h"

#include "Library/Collision/PartsConnector.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Obj/EffectObjFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {
EffectObjCameraEmit::EffectObjCameraEmit(const char* name) : LiveActor(name) {}

void EffectObjCameraEmit::init(const ActorInitInfo& info) {
    using EffectObjCameraEmitFunctor =
        FunctorV0M<EffectObjCameraEmit*, void (EffectObjCameraEmit::*)()>;

    EffectObjFunction::initActorEffectObj(this, info);
    listenStageSwitchOnOffAppear(
        this, EffectObjCameraEmitFunctor(this, &EffectObjCameraEmit::appear),
        EffectObjCameraEmitFunctor(this, &EffectObjCameraEmit::switchOnKill)) ?
        makeActorDead() :
        makeActorAlive();

    listenStageSwitchOnKill(this,
                            EffectObjCameraEmitFunctor(this, &EffectObjCameraEmit::switchOnKill));

    listenStageSwitchOnOff(this, "OnKillOffAppearSwitch",
                           EffectObjCameraEmitFunctor(this, &EffectObjCameraEmit::switchOnKill),
                           EffectObjCameraEmitFunctor(this, &EffectObjCameraEmit::appear));

    mMtxConnector = tryCreateMtxConnector(this, info);
}

void EffectObjCameraEmit::switchOnKill() {
    mIsStopFollowCamera = true;
    tryDeleteEffect(this, "Wait");
}

void EffectObjCameraEmit::initAfterPlacement() {
    if (mMtxConnector == nullptr)
        return;

    attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void EffectObjCameraEmit::control() {
    if (mMtxConnector != nullptr)
        connectPoseQT(this, mMtxConnector);

    makeMtxRT(&mBaseMtx, this);

    if (!mIsStarted)
        tryEmitStart();

    if (mIsStopFollowCamera && !isEffectEmitting(this, "Wait"))
        kill();
}

bool EffectObjCameraEmit::tryEmitStart() {
    if (mIsStarted)
        return false;

    emitEffect(this, "Wait", nullptr);
    tryStartSe(this, "Wait");
    mIsStarted = true;

    return true;
}

void EffectObjCameraEmit::appear() {
    mIsStarted = false;
    mIsStopFollowCamera = false;

    LiveActor::appear();
}

void EffectObjCameraEmit::kill() {
    mIsStarted = false;
    mIsStopFollowCamera = false;

    LiveActor::kill();
}

const sead::Matrix34f* EffectObjCameraEmit::getBaseMtx() const {
    return &mBaseMtx;
}
}  // namespace al
