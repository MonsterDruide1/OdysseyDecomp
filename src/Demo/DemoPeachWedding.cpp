#include "Demo/DemoPeachWedding.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Placement/PlacementFunction.h"

DemoPeachWedding::DemoPeachWedding(const char* name) : al::DemoActor(name) {}

void DemoPeachWedding::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "PeachWedding", "Demo");
    al::tryGetArg(&mIsUseCapManHeroineEyes, info, "IsUseCapManHeroineEyes");
    if (!mIsUseCapManHeroineEyes) {
        if (al::LiveActor* subActor = al::getSubActor(this, "ティアラの目"))
            subActor->kill();
    }
    makeActorDead();
}

void DemoPeachWedding::makeActorAlive() {
    al::LiveActor::makeActorAlive();
    if (mIsUseCapManHeroineEyes) {
        if (!al::isValidOcclusionQuery(this))
            return al::invalidateOcclusionQuery(al::getSubActor(this, "ティアラの目"));
    } else {
        if (al::LiveActor* subActor = al::getSubActor(this, "ティアラの目"))
            return subActor->kill();
    }
}
