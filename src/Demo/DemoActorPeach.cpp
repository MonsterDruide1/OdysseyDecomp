#include "Demo/DemoActorPeach.h"

#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Placement/PlacementFunction.h"

DemoActorPeach::DemoActorPeach(const char* name) : al::DemoActor(name) {}

void DemoActorPeach::init(const al::ActorInitInfo& initInfo) {
    al::initActorWithArchiveName(this, initInfo, "Peach", "Demo");
    al::tryGetArg(&mIsUseCapManHeroineEyes, initInfo, "IsUseCapManHeroineEyes");

    if (!mIsUseCapManHeroineEyes) {
        al::LiveActor* subActor = al::tryGetSubActor(this, "[デモ用]ティアラの目");
        if (subActor)
            subActor->kill();
    }

    makeActorDead();
}

void DemoActorPeach::makeActorAlive() {
    al::invalidateClipping(this);
    al::startVisAnim(this, "TiaraOn");
    al::LiveActor::makeActorAlive();

    if (!mIsUseCapManHeroineEyes) {
        al::LiveActor* subActor = al::tryGetSubActor(this, "[デモ用]ティアラの目");
        if (subActor)
            subActor->kill();
    }
}
