#include "MapObj/TransparentWall.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorMsgFunction.h"
#include "Library/LiveActor/LiveActorUtil.h"
#include "Library/Placement/PlacementFunction.h"

#include "Util/ActorDimensionKeeper.h"

TransparentWall::TransparentWall(const char* name) : al::LiveActor(name) {}

void TransparentWall::init(const al::ActorInitInfo& info) {
    const char* objectName;
    al::getObjectName(&objectName, info);

    if (al::isEqualString("TransparentWall2D", objectName)) {
        al::initActorWithArchiveName(this, info, "TransparentWall", nullptr);
        al::setCollisionPartsSpecialPurposeName(this, rs::getSpecialPurposeName2DOnly());
    } else {
        al::initActor(this, info);
    }

    al::setClippingInfo(this, al::getScaleX(this) * 500.0f * sqrtf(2.0f), nullptr);

    al::trySyncStageSwitchAppearAndKill(this);
}

bool TransparentWall::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self) {
    return al::isMsgScreenPointInvalidCollisionParts(message);
}
