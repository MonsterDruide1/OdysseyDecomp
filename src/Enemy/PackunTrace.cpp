#include "Enemy/PackunTrace.h"

#include "Library/Base/StringUtil.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Placement/PlacementFunction.h"

PackunTrace::PackunTrace(al::LiveActor* parentActor) : al::LiveActor("パックンの茎") {}

void PackunTrace::init(const al::ActorInitInfo& info) {
    const char* objectName;
    al::getObjectName(&objectName, info);

    const char* archiveName = "PackunTrace";
    if (al::isEqualString("PackunFlowerBig", objectName) ||
        al::isEqualString("PackunPoisonBig", objectName))
        archiveName = "PackunTraceBig";

    al::initActorWithArchiveName(this, info, archiveName, nullptr);

    mMtxConnector = al::tryCreateMtxConnector(this, info);

    makeActorDead();
}

void PackunTrace::initAfterPlacement() {
    if (mMtxConnector)
        al::attachMtxConnectorToCollision(mMtxConnector, this, false);
}

void PackunTrace::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void PackunTrace::appear() {
    al::LiveActor::appear();
    al::startHitReaction(this, "出現");
}
