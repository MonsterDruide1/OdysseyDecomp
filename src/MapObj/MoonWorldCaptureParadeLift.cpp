#include "MapObj/MoonWorldCaptureParadeLift.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
NERVE_IMPL(MoonWorldCaptureParadeLift, Wait)
NERVE_IMPL(MoonWorldCaptureParadeLift, MoveSign)
NERVE_IMPL(MoonWorldCaptureParadeLift, Move)
NERVE_IMPL(MoonWorldCaptureParadeLift, Delay)

NERVES_MAKE_NOSTRUCT(MoonWorldCaptureParadeLift, Move, Delay)
NERVES_MAKE_STRUCT(MoonWorldCaptureParadeLift, Wait, MoveSign)
}  // namespace

MoonWorldCaptureParadeLift::MoonWorldCaptureParadeLift(const char* name) : al::LiveActor(name) {}

void MoonWorldCaptureParadeLift::init(const al::ActorInitInfo& info) {
    using MoonWorldCaptureParadeLiftFunctor =
        al::FunctorV0M<MoonWorldCaptureParadeLift*, void (MoonWorldCaptureParadeLift::*)()>;

    al::initActor(this, info);
    al::initNerve(this, &NrvMoonWorldCaptureParadeLift.Wait, 0);

    if (al::isExistRail(this)) {
        f32 clippingRadius = al::getClippingRadius(this);
        al::setSyncRailToNearestPos(this);
        al::setRailClippingInfo(&mRailClippingInfo, this, 100.0f, clippingRadius);
    }

    al::getArg(&mSpeed, info, "Speed");
    al::getArg(&mDelay, info, "Delay");

    al::listenStageSwitchOnStart(
        this, MoonWorldCaptureParadeLiftFunctor(this, &MoonWorldCaptureParadeLift::start));

    makeActorAlive();
}

void MoonWorldCaptureParadeLift::start() {
    if (al::isNerve(this, &NrvMoonWorldCaptureParadeLift.Wait))
        al::setNerve(this, &NrvMoonWorldCaptureParadeLift.MoveSign);
}

void MoonWorldCaptureParadeLift::exeWait() {}

void MoonWorldCaptureParadeLift::exeMoveSign() {
    if (al::isFirstStep(this))
        al::startAction(this, "MoveSign");

    if (al::isActionEnd(this))
        al::setNerve(this, &Move);
}

void MoonWorldCaptureParadeLift::exeMove() {
    if (al::isFirstStep(this))
        al::startAction(this, "Move");

    f32 rate = al::calcRailTotalRate(this);
    f32 length = al::getRailTotalLength(this);

    if ((1.0f - rate) < (mSpeed / length) + 0.00001f)
        al::startHitReaction(this, "消滅");

    if (al::moveSyncRailLoop(this, mSpeed)) {
        al::startHitReaction(this, "出現");

        if (mDelay >= 1) {
            al::startMtsAnim(this, "Wait");
            al::setNerve(this, &Delay);
        }
    }
}

void MoonWorldCaptureParadeLift::exeDelay() {
    if (al::isGreaterEqualStep(this, mDelay - 1))
        al::setNerve(this, &Move);
}
