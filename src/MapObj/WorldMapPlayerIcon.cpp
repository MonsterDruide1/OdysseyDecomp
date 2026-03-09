#include "MapObj/WorldMapPlayerIcon.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/LiveActorFunction.h"

#include "MapObj/ShineTowerRocket.h"

static void movementRecursive(al::LiveActor* actor) {
    actor->movement();
    if (al::isExistSubActorKeeper(actor))
        for (s32 i = 0; i < al::getSubActorNum(actor); i++)
            movementRecursive(al::getSubActor(actor, i));
}

static void calcAnimRecursive(al::LiveActor* actor) {
    actor->calcAnim();
    if (al::isExistSubActorKeeper(actor))
        for (s32 i = 0; i < al::getSubActorNum(actor); i++)
            calcAnimRecursive(al::getSubActor(actor, i));
}

static const sead::Vector3f sFloatOffset = {0.0f, 20.0f, 0.0f};

WorldMapPlayerIcon* WorldMapPlayerIcon::create(const char* name, const al::ActorInitInfo& initInfo,
                                               const sead::Matrix34f* worldMtx) {
    WorldMapPlayerIcon* icon = new WorldMapPlayerIcon(name);
    al::initActorSceneInfo(icon, initInfo);
    const char* arcName = rs::getHomeArchiveName(icon);
    initParts(icon, arcName, initInfo, worldMtx, sead::Matrix34f::ident, "WorldMap");
    al::startAction(icon, "WaitWorldMap");
    al::startAction(al::getSubActor(icon, u8"旗"), "After");
    rs::setupHomeMeter(icon);
    rs::setupHomeSticker(icon);
    return icon;
}

WorldMapPlayerIcon::WorldMapPlayerIcon(const char* name)
    : WorldMapPartsFloat(name, sFloatOffset, 360, 10.0f) {}

void WorldMapPlayerIcon::movement() {
    al::LiveActor::movement();
    for (s32 i = 0; i < al::getSubActorNum(this); i++)
        movementRecursive(al::getSubActor(this, i));
}

void WorldMapPlayerIcon::calcAnim() {
    al::LiveActor::calcAnim();
    for (s32 i = 0; i < al::getSubActorNum(this); i++)
        calcAnimRecursive(al::getSubActor(this, i));
}
