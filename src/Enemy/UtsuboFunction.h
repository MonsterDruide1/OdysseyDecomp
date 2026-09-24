#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class AreaObjGroup;
class LiveActor;
}  // namespace al

namespace UtsuboFunction {

struct UtsuboAttackParam {
    s32 playerMoveFrame;
    f32 radiusMoveArea;
    f32 radiusNoMoveArea;
    f32 height;
    f32 actorUpOffset;
};

void calcPlayerPosAfterMove(sead::Vector3f* out, const al::LiveActor* actor,
                            const sead::Vector3f& playerPos, const UtsuboAttackParam& param);
bool isInSerchRange(const al::LiveActor* actor, const sead::Vector3f& playerPos, bool isMove,
                    const UtsuboAttackParam& param);
bool isMove(const al::AreaObjGroup* areaGroup);

}  // namespace UtsuboFunction
