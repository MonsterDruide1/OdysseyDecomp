#pragma once

#include <basis/seadTypes.h>

namespace al {
struct ActorInitInfo;
class CollisionObj;
class LiveActor;
}  // namespace al

class CollisionAnimCtrl {
public:
    CollisionAnimCtrl(const al::LiveActor* actor, const al::ActorInitInfo& info,
                      const char** collisionNames, s32 collisionNum);

    void startBreak(const char* collisionName);
    void killAll();

private:
    al::CollisionObj** mCollisionObjs = nullptr;
    s32 mCollisionNum = 0;
};

static_assert(sizeof(CollisionAnimCtrl) == 0x10);
