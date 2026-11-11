#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

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
void updateSeparateWaitMove(al::LiveActor* actor, sead::Vector3f* pos,
                            const PlayerColliderHackCap* collider, const al::LiveActor* playerActor,
                            const PlayerInput* input, f32, f32, f32, const sead::Vector3f& offset);
f32 calcSeparateJumpGravity(bool* isSeparateJump, s32* separateJumpTimer, bool* isSeparateJumpHold,
                            s32* separateJumpCount, const al::LiveActor* actor,
                            const PlayerColliderHackCap* collider, const PlayerInput* input, f32 gravity,
                            s32 maxCount);
}  // namespace HackCapFunction
