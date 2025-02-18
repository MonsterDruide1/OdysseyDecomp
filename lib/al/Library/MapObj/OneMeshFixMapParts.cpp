#include "Library/MapObj/OneMeshFixMapParts.h"

#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace al {
OneMeshFixMapParts::OneMeshFixMapParts(const char* name) : FixMapParts(name) {}

void OneMeshFixMapParts::init(const ActorInitInfo& info) {
    const char* mapPartsName = nullptr;
    getStringArg(&mapPartsName, info, "MapPartsName");
    initMapPartsActorWithArchiveName(this, info, mapPartsName, nullptr);
    trySyncStageSwitchAppearAndKill(this);
}
}  // namespace al
