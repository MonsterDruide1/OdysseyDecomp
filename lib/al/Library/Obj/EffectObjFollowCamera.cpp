#include "Library/Obj/EffectObjFollowCamera.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorDrawFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/EffectObjFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_IMPL(EffectObjFollowCamera, Wait)
NERVE_IMPL(EffectObjFollowCamera, Disappear)

NERVES_MAKE_NOSTRUCT(EffectObjFollowCamera, Wait, Disappear)
}  // namespace

namespace al {
EffectObjFollowCamera::EffectObjFollowCamera(const char* name) : LiveActor(name) {}

void EffectObjFollowCamera::init(const ActorInitInfo& info) {
    using EffectObjFollowCameraFunctor =
        FunctorV0M<EffectObjFollowCamera*, void (EffectObjFollowCamera::*)()>;

    EffectObjFunction::initActorEffectObj(this, info);
    invalidateClipping(this);
    setEffectNamedMtxPtr(this, "Wait", &mBaseMtx);
    initNerve(this, &Wait, 0);

    listenStageSwitchOnOffAppear(
        this, EffectObjFollowCameraFunctor(this, &EffectObjFollowCamera::startAppear),
        EffectObjFollowCameraFunctor(this, &EffectObjFollowCamera::startDisappear)) ?
        makeActorDead() :
        makeActorAlive();

    listenStageSwitchOnKill(
        this, EffectObjFollowCameraFunctor(this, &EffectObjFollowCamera::startDisappear));
}

void EffectObjFollowCamera::startAppear() {
    if (isDead(this))
        appear();

    setNerve(this, &Wait);
}

void EffectObjFollowCamera::startDisappear() {
    setNerve(this, &Disappear);
}

void EffectObjFollowCamera::control() {
    mBaseMtx.setInverse(getViewMtx(this, 0));
}

void EffectObjFollowCamera::exeWait() {
    if (isFirstStep(this)) {
        tryEmitEffect(this, "Wait", nullptr);
        tryStartSe(this, "Wait");
    }
}

void EffectObjFollowCamera::exeDisappear() {
    if (isFirstStep(this))
        deleteEffect(this, "Wait");

    if (isStep(this, 180))
        kill();
}
}  // namespace al
