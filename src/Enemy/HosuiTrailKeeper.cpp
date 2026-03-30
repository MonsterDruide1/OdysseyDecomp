#include "Enemy/HosuiTrailKeeper.h"

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"

#include "Enemy/HosuiTrail.h"

HosuiTrailKeeper::HosuiTrailKeeper(const al::ActorInitInfo& initInfo) {
    mTrails.allocBuffer(16, nullptr);
    for (s32 i = 0; i < 16; i++) {
        HosuiTrail* trail = new HosuiTrail();
        al::initCreateActorNoPlacementInfo(trail, initInfo);
        mTrails.pushBack(trail);
    }
}

void HosuiTrailKeeper::appearTrail(const sead::Vector3f& pos, const sead::Vector3f& dir) {
    s32 count = mTrails.size();

    s32 prevIdx = al::modi(mIndex + count - 1 + count, count);
    HosuiTrail* prevTrail = mTrails[prevIdx];

    if (al::isAlive(prevTrail) && (pos - al::getTrans(prevTrail)).length() < 120.0f)
        return;

    HosuiTrail* currentTrail = mTrails[mIndex];

    sead::Vector3f hitPos;
    al::Triangle triangle;
    if (!alCollisionUtil::getFirstPolyOnArrow(currentTrail, &hitPos, &triangle, pos, dir * -50.0f,
                                              nullptr, nullptr))
        return;

    if (al::isAlive(currentTrail))
        currentTrail->kill();

    sead::Quatf randomQuat;
    al::makeQuatYDegree(&randomQuat, al::getRandomDegree());

    sead::Quatf dirQuat;
    al::makeQuatRotationRate(&dirQuat, sead::Vector3f::ey, dir, 1.0f);

    if ((u32)mIndex < (u32)count) {
        HosuiTrail* trail = mTrails[mIndex];
        al::setQuat(trail, dirQuat * randomQuat);
        al::resetPosition(trail, pos);
        trail->appear();
        trail->setFollowCollisionParts(triangle.getCollisionParts());
    }

    s32 disappearIdx = al::modi(mIndex + 5 + count, count);
    HosuiTrail* disappearTrail = mTrails[disappearIdx];
    if (al::isAlive(disappearTrail))
        disappearTrail->disappear();

    mIndex = al::modi(mIndex + 1 + count, count);
}

void HosuiTrailKeeper::forceKillAll() {
    s32 size = mTrails.size();
    for (s32 i = 0; i < size; i++)
        if (al::isAlive(mTrails[i]))
            mTrails[i]->kill();
}
