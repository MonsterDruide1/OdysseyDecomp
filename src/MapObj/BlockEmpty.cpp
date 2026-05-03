#include "MapObj/BlockEmpty.h"

#include <math/seadVector.h>

#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Obj/PartsFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Shadow/ActorShadowUtil.h"

#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(BlockEmpty, Wait);
NERVE_IMPL(BlockEmpty, Reaction);
NERVE_IMPL(BlockEmpty, ReactionTransparent);

NERVES_MAKE_NOSTRUCT(BlockEmpty, Wait, Reaction, ReactionTransparent);
}  // namespace

BlockEmpty::BlockEmpty(const char* name, const char* archiveName)
    : al::LiveActor(name), mArchiveName(archiveName) {}

void BlockEmpty::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, mArchiveName, nullptr);
    al::initNerve(this, &Wait, 0);

    mMtxConnector = al::tryCreateMtxConnector(this, info);
    if (mMtxConnector)
        al::tryGetArg(&mIsConnectToCollisionBack, info, "IsConnectToCollisionBack");

    makeActorAlive();
    al::tryAddDisplayOffset(this, info);
    al::syncSensorScaleY(this);
    al::invalidateHitSensor(this, "Body");
}

void BlockEmpty::initAfterPlacement() {
    if (!mMtxConnector)
        return;

    if (mIsConnectToCollisionBack) {
        sead::Vector3f frontDir = {0.0f, 0.0f, 0.0f};
        al::calcFrontDir(&frontDir, this);
        al::attachMtxConnectorToCollision(
            mMtxConnector, this, al::getTrans(this) + 50.0f * frontDir, -400.0f * frontDir);
    } else {
        al::attachMtxConnectorToCollision(mMtxConnector, this, 50.0f, 400.0f);
    }
}

void BlockEmpty::control() {
    if (mMtxConnector)
        al::connectPoseQT(this, mMtxConnector);
}

void BlockEmpty::attackSensor(al::HitSensor* self, al::HitSensor* other) {
    if (al::isSensorName(self, "Body"))
        rs::sendMsgPushToPlayer(other, self);
}

bool BlockEmpty::receiveMsg(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self) {
    if (rs::isMsgTRexAttackCollideAll(msg)) {
        al::appearBreakModelRandomRotateY(al::getSubActor(this, "Body"));
        kill();
    }

    return al::isMsgPlayerDisregard(msg);
}

void BlockEmpty::setShadowDropLength(f32 shadowDropLength) {
    if (al::isNearZero(shadowDropLength, 0.001f)) {
        al::invalidateShadowMask(this);
        return;
    }

    al::setShadowMaskDropLength(this, shadowDropLength, "本体");
    al::expandClippingRadiusByShadowLength(this, &mShadowDropClippingCenter, shadowDropLength);
}

void BlockEmpty::startReaction() {
    al::startAction(this, "Reaction");
    al::setNerve(this, &Reaction);
}

void BlockEmpty::startReactionTransparent() {
    al::startAction(this, "Reaction");
    al::setNerve(this, &ReactionTransparent);
    al::validateHitSensor(this, "Body");
}

void BlockEmpty::exeWait() {}

void BlockEmpty::exeReaction() {
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void BlockEmpty::exeReactionTransparent() {
    if (al::isActionEnd(this)) {
        al::setNerve(this, &Wait);
        al::invalidateHitSensor(this, "Body");
    }
}
