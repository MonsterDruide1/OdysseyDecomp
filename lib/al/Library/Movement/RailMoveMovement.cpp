#include "Library/Movement/RailMoveMovement.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"

namespace {
using namespace al;

NERVE_IMPL(RailMoveMovement, Move)

NERVES_MAKE_STRUCT(RailMoveMovement, Move)
}  // namespace

namespace al {
// NON_MATCHING
RailMoveMovement::RailMoveMovement(LiveActor* host, const ActorInitInfo& info)
    : HostStateBase<LiveActor>("レール移動挙動", host) {
    tryGetArg(&mSpeed, info, "Speed");
    tryGetArg((s32*)&mMoveType, info, "MoveType");

    if ((u32)mMoveType > (u32)MoveType::Stop)
        mMoveType = MoveType::Loop;

    initNerve(&NrvRailMoveMovement.Move, 0);
}

void RailMoveMovement::exeMove() {
    if (!isExistRail(getHost()))
        return;

    switch (mMoveType) {
    case MoveType::Loop:
        moveSyncRailLoop(getHost(), mSpeed);
        break;
    case MoveType::Turn:
        moveSyncRailTurn(getHost(), mSpeed);
        break;
    case MoveType::Stop:
        moveSyncRail(getHost(), mSpeed);
        break;
    default:
        break;
    }
}

RailMoveMovement* tryCreateRailMoveMovement(LiveActor* host, const ActorInitInfo& info) {
    if (!isExistRail(host))
        return nullptr;

    setSyncRailToNearestPos(host);

    return new RailMoveMovement(host, info);
}
}  // namespace al
