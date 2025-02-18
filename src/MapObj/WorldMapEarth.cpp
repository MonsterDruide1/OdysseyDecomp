#include "MapObj/WorldMapEarth.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

WorldMapEarth::WorldMapEarth(const char* name) : al::LiveActor(name) {}

void WorldMapEarth::init(const al::ActorInitInfo& initInfo) {
    al::initMapPartsActor(this, initInfo, nullptr);
    makeActorAlive();
}

void WorldMapEarth::control() {
    al::setRotateY(this, al::modf(al::getRotate(this).y + 0.05f + 360.0f, 360.0f) + 0.0f);
}
