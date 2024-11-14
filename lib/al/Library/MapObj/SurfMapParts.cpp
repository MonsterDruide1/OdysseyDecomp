#include "Library/MapObj/SurfMapParts.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/VectorUtil.h"
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
void SurfMapParts::exeWait() {
    sead::Vector3f local_50;
    Triangle triangle;

    if (alCollisionUtil::getFirstPolyOnArrow(
            this, &local_50, &triangle, sead::Vector3f::ey * _110 * 0.5f + getTrans(this),
            sead::Vector3f::ey * -_110, mCollisionPartsFilterActor, nullptr)) {
        local_50 = getTrans(this) * 0.9f + local_50 * 0.1f;
        setTrans(this, local_50);
        if (mIsEnableSlope) {
            sead::Quatf quat;
            turnQuatYDirRate(&quat, getQuat(this), triangle.getNormal(0), 0.1f);
            calcQuatUp(&mUpDir, quat);
        }
    }

    if (mIsEnableSlope) {
        turnQuatYDirRate(getQuatPtr(this), mQuat, triangle.getNormal(0), 0.1f);
        calcQuatUp(&mUpDir, mQuat);
    }
}
}  // namespace al
