#pragma once

#include <basis/seadTypes.h>

#include "Library/Math/ParabolicPath.h"
#include "Library/Nerve/NerveStateBase.h"

namespace al {
class LiveActor;
}  // namespace al

class HomeCapStateMoveChair : public al::ActorStateBase {
public:
    HomeCapStateMoveChair(al::LiveActor* actor);

    void init() override;
    void appear() override;

    void appearMoveOtherChair(al::LiveActor* actor);
    void getCapAppearPos(sead::Vector3f* out);
    void exeMove();

private:
    al::ParabolicPath* mParabolicPath;
    al::LiveActor* mTargetActor = nullptr;
};

static_assert(sizeof(HomeCapStateMoveChair) == 0x30);
