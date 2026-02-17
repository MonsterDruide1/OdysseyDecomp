#include "Item/DotCharacterRare.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(DotCharacterRare, PopUp)

NERVES_MAKE_STRUCT(DotCharacterRare, PopUp)
}  // namespace

DotCharacterRare::DotCharacterRare(const char* name) : al::LiveActor(name) {}

void DotCharacterRare::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "DotMarioCatRare", nullptr);
    al::initNerve(this, &NrvDotCharacterRare.PopUp, 0);
    makeActorAlive();
}

void DotCharacterRare::appearPopUp() {
    al::startAction(this, "DotMarioCatRareReactionSmall");
    al::setNerve(this, &NrvDotCharacterRare.PopUp);
    appear();
    al::startHitReaction(this, "飛出し出現");
}

void DotCharacterRare::exePopUp() {
    if (al::isActionEnd(this))
        kill();
}
