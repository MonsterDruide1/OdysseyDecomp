#pragma once

#include "Library/LiveActor/LiveActor.h"

#include "MapObj/RouteGuideArrowBase.h"
#include "Util/IUseDimension.h"

class ActorDimensionKeeper;

namespace al {
struct ActorInitInfo;
class CollisionParts;
class CollisionPartsConnector;
}  // namespace al

class RouteGuideRailArrow : public al::LiveActor, public RouteGuideArrowBase, public IUseDimension {
public:
    RouteGuideRailArrow(const char* name);

    void makeActorAlive() override;
    void makeActorDead() override;
    void init(const al::ActorInitInfo& info) override;
    void control() override;
    const sead::Vector3f& getGuideTrans() const override;
    void setGuideAlpha(f32 alpha) override;
    void invalidateGuide() override;

    void attach(const al::CollisionParts* collisionParts);
    void setBaseQuatTrans();

    ActorDimensionKeeper* getActorDimensionKeeper() const override { return mDimensionKeeper; }

private:
    al::CollisionPartsConnector* mCollisionPartsConnector = nullptr;
    ActorDimensionKeeper* mDimensionKeeper = nullptr;
    f32 mDisplayOffsetY = 0.0f;
};

static_assert(sizeof(RouteGuideRailArrow) == 0x138);
