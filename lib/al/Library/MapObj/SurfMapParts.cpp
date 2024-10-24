#include "Library/MapObj/SurfMapParts.h"

#include "Library/Collision/KCollisionServer.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"

namespace {
using namespace al;

NERVE_IMPL(SurfMapParts, Wait)

NERVES_MAKE_STRUCT(SurfMapParts, Wait)
}  // namespace

namespace al {
SurfMapParts::SurfMapParts(const char* name) : LiveActor(name) {}

void SurfMapParts::init(const ActorInitInfo& info) {
    initMapPartsActor(this, info, nullptr);
    tryGetQuatPtr(this);
    initNerve(this, &NrvSurfMapParts.Wait, 0);
    registerAreaHostMtx(this, info);

    tryGetArg(&mIsEnableSlope, info, "IsEnableSlope");

    initMaterialCode(this, info);

    mQuat = getQuat(this);
    mTrans = getTrans(this);
    calcQuatUp(&mUpDir, mQuat);

    mCollisionPartsFilterActor = new CollisionPartsFilterActor(this);

    trySyncStageSwitchAppear(this);
}

// TODO: Implement SurfMapParts::exeWait
// void SurfMapParts::exeWait();
}  // namespace al
