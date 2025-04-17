#include "MapObj/VolleyballBase.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(VolleyballBase, Wait);
NERVE_IMPL(VolleyballBase, Reaction);

NERVES_MAKE_NOSTRUCT(VolleyballBase, Wait, Reaction);
}  // namespace

VolleyballBase::VolleyballBase(const char* name) : al::LiveActor(name) {}

void VolleyballBase::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "VolleyballBase", nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

void VolleyballBase::startReaction() {
    al::setNerve(this, &Reaction);
}

void VolleyballBase::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void VolleyballBase::exeReaction() {
    if (al::isFirstStep(this))
        al::startAction(this, "Reaction");

    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}
