#include "Library/MapObj/SwitchDitherMapParts.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(SwitchDitherMapParts, Wait)

NERVE_ACTIONS_MAKE_STRUCT(SwitchDitherMapParts, Wait)
}  // namespace

namespace al {
SwitchDitherMapParts::SwitchDitherMapParts(const char* name) : LiveActor(name) {}

void SwitchDitherMapParts::init(const ActorInitInfo& info) {
    using SwitchDitherMapPartsFunctor =
        FunctorV0M<SwitchDitherMapParts*, void (SwitchDitherMapParts::*)()>;

    initNerveAction(this, "Wait", &NrvSwitchDitherMapParts.collector, 0);
    initMapPartsActor(this, info, nullptr);

    bool isListenStartOnOff = listenStageSwitchOnOff(
        this, "SwitchStart", SwitchDitherMapPartsFunctor(this, &SwitchDitherMapParts::ditherOn),
        SwitchDitherMapPartsFunctor(this, &SwitchDitherMapParts::ditherOff));
    if (isListenStartOnOff)
        invalidateDitherAnim(this);

    makeActorAlive();
}

void SwitchDitherMapParts::ditherOn() {
    validateDitherAnim(this);
}

void SwitchDitherMapParts::ditherOff() {
    invalidateDitherAnim(this);
}

void SwitchDitherMapParts::exeWait() {}
}  // namespace al
