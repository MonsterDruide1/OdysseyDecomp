#include "Library/Obj/EffectObjFollowCameraLimit.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/Effect/EffectKeeper.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorDrawFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/EffectObjFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Se/SeFunction.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_IMPL(EffectObjFollowCameraLimit, Wait)
NERVE_IMPL(EffectObjFollowCameraLimit, Disappear)

NERVES_MAKE_NOSTRUCT(EffectObjFollowCameraLimit, Wait, Disappear)
}  // namespace

namespace al {
EffectObjFollowCameraLimit::EffectObjFollowCameraLimit(const char* name) : LiveActor(name) {}

void EffectObjFollowCameraLimit::init(const ActorInitInfo& info) {
    using EffectObjFollowCameraLimitFunctor =
        FunctorV0M<EffectObjFollowCameraLimit*, void (EffectObjFollowCameraLimit::*)()>;

    EffectObjFunction::initActorEffectObj(this, info);
    invalidateClipping(this);
    setEffectNamedMtxPtr(this, "Wait", &mBaseMtx);
    initNerve(this, &Wait, 0);

    listenStageSwitchOnOffAppear(
        this, EffectObjFollowCameraLimitFunctor(this, &EffectObjFollowCameraLimit::startAppear),
        EffectObjFollowCameraLimitFunctor(this, &EffectObjFollowCameraLimit::startDisappear)) ?
        makeActorDead() :
        makeActorAlive();

    tryGetArg(&mLimitBottom, info, "LimitBottom");
    tryGetArg(&mLimitTop, info, "LimitTop");

    listenStageSwitchOnOffAppear(
        this, EffectObjFollowCameraLimitFunctor(this, &EffectObjFollowCameraLimit::startAppear),
        EffectObjFollowCameraLimitFunctor(this, &EffectObjFollowCameraLimit::startDisappear));

    listenStageSwitchOnOff(
        this, "OnKillOffAppearSwitch",
        EffectObjFollowCameraLimitFunctor(this, &EffectObjFollowCameraLimit::kill),
        EffectObjFollowCameraLimitFunctor(this, &EffectObjFollowCameraLimit::startAppear));

    listenStageSwitchOnKill(
        this, EffectObjFollowCameraLimitFunctor(this, &EffectObjFollowCameraLimit::startDisappear));
}

void EffectObjFollowCameraLimit::startAppear() {
    if (isDead(this))
        appear();

    setNerve(this, &Wait);
}

void EffectObjFollowCameraLimit::startDisappear() {
    setNerve(this, &Disappear);
}

void EffectObjFollowCameraLimit::control() {
    sead::Vector3f front;
    sead::Vector3f pos;

    calcCameraFront(&front, this, 0);
    pos.set(getCameraPos(this, 0));

    pos.y = sead::Mathf::max(pos.y, mLimitBottom);

    if (!isNearZero(mLimitTop + 1, 0.001f))
        pos.y = sead::Mathf::min(pos.y, mLimitTop);

    if (!isParallelDirection(sead::Vector3f::ey, front, 0.01f))
        makeMtxUpFrontPos(&mBaseMtx, sead::Vector3f::ey, front, pos);
}

void EffectObjFollowCameraLimit::kill() {
    LiveActor::kill();
}

void EffectObjFollowCameraLimit::appear() {
    LiveActor::appear();
}

void EffectObjFollowCameraLimit::exeWait() {
    if (isFirstStep(this)) {
        tryEmitEffect(this, "Wait", nullptr);
        tryStartSe(this, "Wait");
    }
}

void EffectObjFollowCameraLimit::exeDisappear() {
    if (isFirstStep(this))
        deleteEffect(this, "Wait");

    if (isStep(this, 180))
        kill();
}
}  // namespace al
