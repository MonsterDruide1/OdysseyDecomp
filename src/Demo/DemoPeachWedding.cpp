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
        al::LiveActor* subActor = al::getSubActor(this, "ティアラの目");
        if (subActor != nullptr)
            subActor->kill();
    }
    makeActorDead();
}

void DemoPeachWedding::makeActorAlive() {
    al::LiveActor::makeActorAlive();
    if (mIsUseCapManHeroineEyes) {
        if (!al::isValidOcclusionQuery(this)) {
            al::LiveActor* subActor = al::getSubActor(this, "ティアラの目");
            return al::invalidateOcclusionQuery(subActor);
        }
    } else {
        al::LiveActor* subActor = al::getSubActor(this, "ティアラの目");
        if (subActor != nullptr)
            return subActor->kill();
    }
}
