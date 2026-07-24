#pragma once

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"

namespace {

class CollisionPartsFilterMultiActor : public al::CollisionPartsFilterBase {
public:
    CollisionPartsFilterMultiActor(const al::LiveActor* actor, const al::LiveActor* actorBase,
                                   const al::LiveActor* actorExtra)
        : mActor(actor), mActorBase(actorBase), mActorExtra(actorExtra) {}

    bool isInvalidParts(al::CollisionParts* collisionParts) override {
        al::HitSensor* sensor = collisionParts->getConnectedSensor();
        if (!sensor)
            return false;

        al::LiveActor* host = al::getSensorHost(sensor);
        if (mActor && host == mActor)
            return true;
        if (mActorBase && host == mActorBase)
            return true;
        return mActorExtra && host == mActorExtra;
    }

private:
    const al::LiveActor* mActor;
    const al::LiveActor* mActorBase;
    const al::LiveActor* mActorExtra;
};

}  // namespace
