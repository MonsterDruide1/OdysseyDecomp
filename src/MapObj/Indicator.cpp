#include "MapObj/Indicator.h"

#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
NERVE_IMPL(Indicator, Wait);

NERVES_MAKE_NOSTRUCT(Indicator, Wait);
}  // namespace

Indicator::Indicator(const char* name) : al::LiveActor(name) {}

void Indicator::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    al::invalidateClipping(this);
    al::tryGetArg(&mIgnoreLevel, info, "IgnoreLevel");
    makeActorAlive();
}

void Indicator::update() {
    if (mTargetLightLevel == mLightLevel)
        return;

    f32 lightLevel = mLightLevelStep + mLightLevelFloat;
    mLightLevelFloat = lightLevel;
    mLightLevel = lightLevel;
    al::setMtsAnimFrame(this, mLightLevel - mIgnoreLevel);
}

void Indicator::setLightLevel(s32 lightLevel) {
    al::setMtsAnimFrame(this, lightLevel - mIgnoreLevel);
    mLightLevel = lightLevel;
    // BUG: does not update `mLightLevelFloat`
    // NOTE: also does not update `mTargetLightLevel` (might be intentional)
}

void Indicator::setLightLevel(s32 lightLevel, s32 duration) {
    mTargetLightLevel = lightLevel;
    mLightLevelStep = lightLevel - mLightLevel;
    if (duration >= 1)
        mLightLevelStep /= duration;
    mLightLevelFloat = mLightLevel;
    al::setMtsAnimFrame(this, mLightLevel - mIgnoreLevel);
}

void Indicator::tryDecreaseLightLevel(s32 minLightLevel) {
    if (mLightLevel <= minLightLevel)
        return;

    mLightLevel--;
    // BUG: does not update `mLightLevelFloat`
    al::setMtsAnimFrame(this, mLightLevel - mIgnoreLevel);
}

void Indicator::exeWait() {
    if (al::isFirstStep(this))
        al::startMtsAnimAndSetFrameAndStop(this, "Light", 0.0f);
}
