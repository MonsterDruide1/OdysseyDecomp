#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CollisionPartsConnector;
struct ActorInitInfo;
class CollisionParts;
}  // namespace al

class HosuiTrail : public al::LiveActor {
public:
    HosuiTrail();
    void init(const al::ActorInitInfo& info) override;
    void appear() override;
    void control() override;

    void disappear();
    void setFollowCollisionParts(const al::CollisionParts* parts);
    void exeAppear();
    void exeDisappear();

private:
    al::CollisionPartsConnector* mConnector = nullptr;
};

static_assert(sizeof(HosuiTrail) == 0x110);
