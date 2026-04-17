#include "MapObj/MotorcyclePlayerAnimator.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/PlayerPuppetFunction.h"

namespace {
NERVE_IMPL(MotorcyclePlayerAnimator, None)
NERVE_IMPL(MotorcyclePlayerAnimator, BindKeepDemo)
NERVE_IMPL(MotorcyclePlayerAnimator, RideStartL)
NERVE_IMPL(MotorcyclePlayerAnimator, RideStartR)
NERVE_IMPL(MotorcyclePlayerAnimator, RideOn)
NERVE_IMPL(MotorcyclePlayerAnimator, Wait)
NERVE_IMPL(MotorcyclePlayerAnimator, RideRunStart)
NERVE_IMPL(MotorcyclePlayerAnimator, RideClash)
NERVE_IMPL(MotorcyclePlayerAnimator, RideJump)
NERVE_IMPL(MotorcyclePlayerAnimator, RideRun)
NERVE_IMPL(MotorcyclePlayerAnimator, RideLand)

NERVES_MAKE_NOSTRUCT(MotorcyclePlayerAnimator, None, BindKeepDemo, RideStartL, RideStartR, RideOn,
                     Wait, RideClash)
NERVES_MAKE_STRUCT(MotorcyclePlayerAnimator, RideRunStart, RideJump, RideRun, RideLand)
}  // namespace

MotorcyclePlayerAnimator::MotorcyclePlayerAnimator()
    : al::NerveExecutor("バイクに乗っているときのプレイヤーアニメ") {
    initNerve(&None, 0);
}

void MotorcyclePlayerAnimator::update(f32 weight1, f32 weight2, f32 weight3) {
    mBlendWeight1 = weight1;
    mBlendWeight2 = weight2;
    mBlendWeight3 = weight3;
    updateNerve();
}

void MotorcyclePlayerAnimator::startBind(IUsePlayerPuppet* playerPuppet) {
    mPlayerPuppet = playerPuppet;
}

void MotorcyclePlayerAnimator::startBindKeepDemo() {
    al::setNerve(this, &BindKeepDemo);
}

void MotorcyclePlayerAnimator::endBind() {
    rs::startPuppetAction(mPlayerPuppet, "Jump");
    mPlayerPuppet = nullptr;
    al::setNerve(this, &None);
}

void MotorcyclePlayerAnimator::startBindRideStartL() {
    rs::startPuppetAction(mPlayerPuppet, "MotorcycleRideStartL");
    al::setNerve(this, &RideStartL);
}

void MotorcyclePlayerAnimator::startBindRideStartR() {
    rs::startPuppetAction(mPlayerPuppet, "MotorcycleRideStartR");
    al::setNerve(this, &RideStartR);
}

void MotorcyclePlayerAnimator::startBindRideOn() {
    rs::startPuppetAction(mPlayerPuppet, "MotorcycleRideOn");
    al::setNerve(this, &RideOn);
}

void MotorcyclePlayerAnimator::startBindWait() {
    al::setNerve(this, &Wait);
}

void MotorcyclePlayerAnimator::startBindRideRunStart() {
    al::setNerve(this, &NrvMotorcyclePlayerAnimator.RideRunStart);
}

void MotorcyclePlayerAnimator::startBindRideClash() {
    al::setNerve(this, &RideClash);
}

void MotorcyclePlayerAnimator::startBindRideJump() {
    al::setNerve(this, &NrvMotorcyclePlayerAnimator.RideJump);
}

bool MotorcyclePlayerAnimator::tryStartBindRideRunIfNotPlaying() {
    const al::Nerve* nerve = &NrvMotorcyclePlayerAnimator.RideRunStart;
    if (al::isNerve(this, nerve))
        return false;

    nerve = &NrvMotorcyclePlayerAnimator.RideRun;
    if (al::isNerve(this, nerve))
        return false;

    al::setNerve(this, nerve);
    return true;
}

bool MotorcyclePlayerAnimator::tryStartBindRideLandIfJump() {
    if (mPlayerPuppet == nullptr || !al::isNerve(this, &NrvMotorcyclePlayerAnimator.RideJump))
        return false;

    al::setNerve(this, &NrvMotorcyclePlayerAnimator.RideLand);
    return true;
}

void MotorcyclePlayerAnimator::exeNone() {}

void MotorcyclePlayerAnimator::exeRideStartL() {}

void MotorcyclePlayerAnimator::exeRideStartR() {}

void MotorcyclePlayerAnimator::exeRideOn() {}

void MotorcyclePlayerAnimator::exeWait() {
    if (al::isFirstStep(this))
        rs::startPuppetAction(mPlayerPuppet, "MotorcycleWait");
}

void MotorcyclePlayerAnimator::exeRideRunStart() {
    if (al::isFirstStep(this))
        rs::startPuppetAction(mPlayerPuppet, "MotorcycleRideRunStart");

    rs::setPuppetAnimBlendWeight(mPlayerPuppet, mBlendWeight1, mBlendWeight2, mBlendWeight3, 0.0f,
                                 0.0f, 0.0f);

    if (rs::isPuppetActionEnd(mPlayerPuppet))
        al::setNerve(this, &NrvMotorcyclePlayerAnimator.RideRun);
}

void MotorcyclePlayerAnimator::exeRideRun() {
    if (al::isFirstStep(this))
        rs::startPuppetAction(mPlayerPuppet, "MotorcycleRide");

    rs::setPuppetAnimBlendWeight(mPlayerPuppet, mBlendWeight1, mBlendWeight2, mBlendWeight3, 0.0f,
                                 0.0f, 0.0f);
}

void MotorcyclePlayerAnimator::exeRideClash() {
    if (al::isFirstStep(this))
        rs::startPuppetAction(mPlayerPuppet, "MotorcycleRideClash");

    rs::setPuppetAnimBlendWeight(mPlayerPuppet, mBlendWeight1, mBlendWeight2, mBlendWeight3, 0.0f,
                                 0.0f, 0.0f);

    if (rs::isPuppetActionEnd(mPlayerPuppet))
        al::setNerve(this, &NrvMotorcyclePlayerAnimator.RideRun);
}

void MotorcyclePlayerAnimator::exeRideJump() {
    if (al::isFirstStep(this))
        rs::startPuppetAction(mPlayerPuppet, "MotorcycleRideJump");

    rs::setPuppetAnimBlendWeight(mPlayerPuppet, mBlendWeight1, mBlendWeight2, mBlendWeight3, 0.0f,
                                 0.0f, 0.0f);
}

void MotorcyclePlayerAnimator::exeRideLand() {
    if (al::isFirstStep(this))
        rs::startPuppetAction(mPlayerPuppet, "MotorcycleRideLand");

    rs::setPuppetAnimBlendWeight(mPlayerPuppet, mBlendWeight1, mBlendWeight2, mBlendWeight3, 0.0f,
                                 0.0f, 0.0f);

    if (rs::isPuppetActionEnd(mPlayerPuppet))
        al::setNerve(this, &NrvMotorcyclePlayerAnimator.RideRun);
}

void MotorcyclePlayerAnimator::exeBindKeepDemo() {}

MotorcyclePlayerAnimator::~MotorcyclePlayerAnimator() {}
