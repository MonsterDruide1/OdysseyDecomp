#include "MapObj/FastenerObjSpace.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(FastenerObjSpace, Wait);
NERVE_IMPL(FastenerObjSpace, Disappear);
NERVES_MAKE_NOSTRUCT(FastenerObjSpace, Wait, Disappear);
}  // namespace

FastenerObjSpace::FastenerObjSpace(const char* name) : al::LiveActor(name) {}

void FastenerObjSpace::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "FastenerSpace", nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

void FastenerObjSpace::appear() {
    al::LiveActor::appear();
    al::setModelAlphaMask(this, 1.0f);
    al::setNerve(this, &Wait);
}

void FastenerObjSpace::disappear() {
    al::invalidateClipping(this);
    al::setNerve(this, &Disappear);
}

void FastenerObjSpace::exeWait() {
    if (al::isFirstStep(this)) {
    }
}

void FastenerObjSpace::exeDisappear() {
    f32 rate = al::calcNerveRate(this, 60);
    al::setModelAlphaMask(this, 1.0f - rate);
    if (al::isGreaterStep(this, 60))
        kill();
}
