#include "Library/Movement/WheelMovement.h"

#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathAngleUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"

namespace al {
WheelMovement::WheelMovement(LiveActor* actor, const ActorInitInfo& info)
    : NerveExecutor("車輪動作計算") {
    tryGetArg((s32*)&mRotateAxis, info, "\"車輪動作計算\"");
    tryGetArg(&mRotateAccel, info, "RotateAccel");
    tryGetArg(&mMoveEndDegree, info, "MoveEndDegree");
    tryGetArg(&mNoRotateWidth, info, "NoRotateWidth");

    getQuat(&_20, info);
    _10 = _20;

    sead::Vector3f local_80 = sead::Vector3f::ex;
    calcQuatLocalAxis(&local_80, _20, (s32)mRotateAxis);

    mMoveDir.setCross(local_80, sead::Vector3f::ey);
    if (isNearZero(mMoveDir, 0.001f))
        mMoveDir = sead::Vector3f::ez;

    if (isExistRail(actor)) {
        setSyncRailToNearestPos(actor);
        _64 = true;
        mIsRailPlusDir = isRailPlusDir(actor, mMoveDir);
        f32 railProgress = getRailCoord(actor) / getRailTotalLength(actor);
        _5c = railProgress;
        _60 = railProgress;

        if (mIsRailPlusDir)
            _44 = railProgress * mMoveEndDegree;
        else
            _44 = -(railProgress * mMoveEndDegree);

        rotateQuatLocalDirDegree(&_10, _20, (s32)mRotateAxis, modf(_44 + 360.0f, 360.0f) + 0.0f);
    }
}
}  // namespace al
