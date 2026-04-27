#include "MapObj/WaterfallWorldWaterfall.h"

#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/EffectObjFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
NERVE_IMPL(WaterfallWorldWaterfall, WaitMax)
NERVE_IMPL(WaterfallWorldWaterfall, ChangeMaxToMin)
NERVE_IMPL(WaterfallWorldWaterfall, ChangeMinToMax)
NERVE_IMPL(WaterfallWorldWaterfall, WaitMin)

NERVES_MAKE_NOSTRUCT(WaterfallWorldWaterfall, WaitMax, ChangeMaxToMin, ChangeMinToMax, WaitMin)
}  // namespace

WaterfallWorldWaterfall::WaterfallWorldWaterfall(const char* name) : al::LiveActor(name) {}

void WaterfallWorldWaterfall::init(const al::ActorInitInfo& info) {
    using WaterfallWorldWaterfallFunctor =
        al::FunctorV0M<WaterfallWorldWaterfall*, void (WaterfallWorldWaterfall::*)()>;

    al::EffectObjFunction::initActorEffectObj(this, info);
    al::initNerve(this, &WaitMax, 0);

    al::listenStageSwitchOnOff(
        this, "OnChangeAlphaSwitch",
        WaterfallWorldWaterfallFunctor(this, &WaterfallWorldWaterfall::changeToMin),
        WaterfallWorldWaterfallFunctor(this, &WaterfallWorldWaterfall::changeToMax));

    al::makeMtxRT(&mBaseMtx, this);
    makeActorAlive();
}

void WaterfallWorldWaterfall::changeToMin() {
    al::setNerve(this, &ChangeMaxToMin);
}

void WaterfallWorldWaterfall::changeToMax() {
    al::setNerve(this, &ChangeMinToMax);
}

void WaterfallWorldWaterfall::exeWaitMax() {
    if (!al::isFirstStep(this))
        return;

    al::emitEffect(this, "Wait", nullptr);
    al::setEffectParticleAlpha(this, "Wait", 1.0f);
}

void WaterfallWorldWaterfall::exeChangeMaxToMin() {
    al::setEffectParticleAlpha(this, "Wait",
                               al::lerpValue(1.0f, 0.1f, al::calcNerveRate(this, 10)));

    if (al::isGreaterEqualStep(this, 10))
        al::setNerve(this, &WaitMin);
}

void WaterfallWorldWaterfall::exeWaitMin() {
    if (!al::isFirstStep(this))
        return;

    al::setEffectParticleAlpha(this, "Wait", 0.1f);
}

void WaterfallWorldWaterfall::exeChangeMinToMax() {
    al::setEffectParticleAlpha(this, "Wait",
                               al::lerpValue(0.1f, 1.0f, al::calcNerveRate(this, 10)));

    if (al::isGreaterEqualStep(this, 10))
        al::setNerve(this, &WaitMax);
}
