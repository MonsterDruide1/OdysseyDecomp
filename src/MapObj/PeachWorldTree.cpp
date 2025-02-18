#include "MapObj/PeachWorldTree.h"

#include "Library/Camera/CameraUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

PeachWorldTree::PeachWorldTree(const char* name) : al::LiveActor(name) {}

void PeachWorldTree::init(const al::ActorInitInfo& info) {
    al::initActor(this, info);
    makeActorAlive();
}

void PeachWorldTree::control() {
    sead::Vector3f dir = al::getTrans(this) - al::getCameraPos(this, 0);
    dir.y = 0.0f;
    if (al::tryNormalizeOrZero(&dir, dir))
        al::setFront(this, -dir);
}
