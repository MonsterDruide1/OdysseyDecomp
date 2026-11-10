#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
}

class PlayerColliderHackCap;
class PlayerSeparateCapFlag;
class PlayerInput;
class IJudge;

namespace HackCapFunction {
void resetPositionAndCollision(al::LiveActor* actor, PlayerColliderHackCap* collider);
bool isKeepSeparateHold(const PlayerSeparateCapFlag* flag, IJudge* judge, bool isSeparate);
f32 calcSeparateJumpGravity(bool* isSeparateJump, s32* separateJumpTimer, bool* isSeparateJumpHold,
                            s32* separateJumpCount, const al::LiveActor* actor,
                            const PlayerColliderHackCap* collider, const PlayerInput* input, f32 gravity,
                            s32 maxCount);
}  // namespace HackCapFunction
