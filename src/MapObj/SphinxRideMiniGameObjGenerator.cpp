#include "MapObj/SphinxRideMiniGameObjGenerator.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
NERVE_IMPL(SphinxRideMiniGameObjGenerator, Wait)

NERVES_MAKE_NOSTRUCT(SphinxRideMiniGameObjGenerator, Wait)
}  // namespace

SphinxRideMiniGameObjGenerator::SphinxRideMiniGameObjGenerator(const char* name, f32 unused1,
                                                               f32 unused2)
    : al::LiveActor("乗れるスフィンクスのミニゲームオブジェジェネレータ") {}

void SphinxRideMiniGameObjGenerator::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    al::initNerve(this, &Wait, 0);
    al::tryGetArg(&mIsAppearRandom, info, "isAppearRandom");

    s32 linkChildNum = al::calcLinkChildNum(info, "CreateObj");

    if (linkChildNum <= 0) {
        makeActorDead();
        return;
    }

    mActorArray.allocBuffer(linkChildNum, nullptr, 8);
    mTransArray = new sead::Vector3f[linkChildNum];

    for (s32 i = 0; i != linkChildNum; i++) {
        al::LiveActor* linksActor = al::createLinksActorFromFactory(info, "CreateObj", i);
        mActorArray.pushBack(linksActor);
        mTransArray[i] = al::getTrans(linksActor);
    }

    makeActorDeadAllActor();
    makeActorAlive();
}

void SphinxRideMiniGameObjGenerator::makeActorDeadAllActor() {
    for (s32 i = 0; i < mActorArray.size(); i++)
        mActorArray[i]->makeActorDead();
}

void SphinxRideMiniGameObjGenerator::exeWait() {
    if (al::isStep(this, 60))
        appearAllActor();
}

void SphinxRideMiniGameObjGenerator::appearAllActor() {
    for (s32 i = 0; i < mActorArray.size(); i++) {
        al::LiveActor* actor = mActorArray[i];
        actor->appear();
        if (!mIsAppearRandom)
            al::setTrans(actor, mTransArray[i]);
    }
}
