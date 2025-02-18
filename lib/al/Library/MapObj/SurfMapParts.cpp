#include "Library/MapObj/SurfMapParts.h"

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
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

void SurfMapParts::exeWait() {
    sead::Vector3f hitPos;
    Triangle triangle;
    sead::Vector3f trans = getTrans(this);

    if (alCollisionUtil::getFirstPolyOnArrow(
            this, &hitPos, &triangle, getTrans(this) + mCheckOffset * sead::Vector3f::ey * 0.5f,
            -mCheckOffset * sead::Vector3f::ey, mCollisionPartsFilterActor, nullptr)) {
        setTrans(this, trans * 0.9f + hitPos * 0.1f);
        if (mIsEnableSlope) {
            sead::Quatf quat;
            sead::Vector3f normal = triangle.getNormal(0);
            turnQuatYDirRate(&quat, getQuat(this), normal, 0.1f);
            calcQuatUp(&mUpDir, quat);
        }
    }

    if (mIsEnableSlope)
        turnQuatYDirRate(getQuatPtr(this), mQuat, mUpDir, 1.0f);
}
}  // namespace al
