#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CollisionObj;
struct ActorInitInfo;
}  // namespace al

class KinokoUfo : public al::LiveActor {
public:
    KinokoUfo(const char* actorName);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;

    void listenClose();
    void listenOpen();

private:
    al::CollisionObj* mCollisionObj = nullptr;
};

static_assert(sizeof(KinokoUfo) == 0x110);
