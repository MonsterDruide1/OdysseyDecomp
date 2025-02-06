#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

namespace Player2DDokanActionFunction {
bool isPlayer2DActionDokanInPlayingGreater(const al::LiveActor* actor, s32 frame);
bool isPlayer2DActionDokanOutPlayingLessEqual(const al::LiveActor* actor, s32 frame);
}  // namespace Player2DDokanActionFunction
