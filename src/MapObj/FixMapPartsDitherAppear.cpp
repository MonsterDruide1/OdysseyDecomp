#include "MapObj/FixMapPartsDitherAppear.h"

#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
NERVE_IMPL(FixMapPartsDitherAppear, Wait);
NERVE_IMPL(FixMapPartsDitherAppear, Appear);
NERVES_MAKE_NOSTRUCT(FixMapPartsDitherAppear, Wait, Appear);
}  // namespace

FixMapPartsDitherAppear::FixMapPartsDitherAppear(const char* actorName)
    : al::LiveActor(actorName) {}

void FixMapPartsDitherAppear::init(const al::ActorInitInfo& info) {
    using FixMapPartsDitherAppearFunctor =
        al::FunctorV0M<FixMapPartsDitherAppear*, void (FixMapPartsDitherAppear::*)()>;

    al::initMapPartsActor(this, info, nullptr);
    al::registActorToDemoInfo(this, info);
    al::initNerve(this, &Wait, 0);

    bool isListenSwitchAppear = al::listenStageSwitchOn(
        this, "SwitchAppear",
        FixMapPartsDitherAppearFunctor(this, &FixMapPartsDitherAppear::ditherAppear));
    bool isListenSwitchAppeared = al::listenStageSwitchOn(
        this, "SwitchAppeared",
        FixMapPartsDitherAppearFunctor(this, &FixMapPartsDitherAppear::waitAppear));
    al::tryGetArg(&mAnimFrame, info, "AnimFrame");

    if (isListenSwitchAppear || isListenSwitchAppeared)
        makeActorDead();
    else
        makeActorAlive();
}

void FixMapPartsDitherAppear::ditherAppear() {
    al::LiveActor::appear();
    al::setModelAlphaMask(this, 0.0f);
    al::setNerve(this, &Appear);
}

void FixMapPartsDitherAppear::waitAppear() {
    if (al::isNerve(this, &Appear))
        return;

    al::LiveActor::appear();
    al::setModelAlphaMask(this, 1.0f);
    al::setNerve(this, &Wait);
}

void FixMapPartsDitherAppear::calcAnim() {
    al::calcViewModel(this);
}

bool FixMapPartsDitherAppear::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                         al::HitSensor* self) {
    if (al::isMsgAskSafetyPoint(message) && al::isNerve(this, &Wait))
        return true;
    return false;
}

void FixMapPartsDitherAppear::exeAppear() {
    f32 rate = al::calcNerveRate(this, mAnimFrame);
    al::setModelAlphaMask(this, rate);

    if (al::isGreaterEqualStep(this, mAnimFrame))
        al::setNerve(this, &Wait);
}

void FixMapPartsDitherAppear::exeWait() {}
