#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;

class ActorCollisionController {
public:
    ActorCollisionController(LiveActor* actor);
    void setColliderRadius(f32 radius);
    void setColliderRadiusScale(f32 scale);
    void setColliderOffsetY(f32 offsetY);
    void update();
    void resetToOrigin(s32 delay);

private:
    LiveActor* mActor;
    f32 mRadius;
    f32 mOffsetY;
    s32 _10 = 0;
};

static_assert(sizeof(ActorCollisionController) == 0x18);
}  // namespace al
