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

    s32 prevIdx = al::modi(mIndex + count * 2 - 1, count);
    HosuiTrail* prevTrail = mTrails.at(prevIdx);

    if (al::isAlive(prevTrail)) {
        const sead::Vector3f& prevTrans = al::getTrans(prevTrail);
        sead::Vector3f diff = pos - prevTrans;
        if (diff.length() < 120.0f)
            return;
    }

    HosuiTrail* currentTrail = mTrails.at(mIndex);

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

    s32 trailIdx = mIndex;
    if ((u32)trailIdx < (u32)count) {
        HosuiTrail* trail = mTrails.at(trailIdx);
        sead::Quatf combinedQuat = dirQuat * randomQuat;
        al::setQuat(trail, combinedQuat);
        al::resetPosition(trail, pos);
        trail->appear();
        trail->setFollowCollisionParts(triangle.getCollisionParts());
        trailIdx = mIndex;
    }

    s32 disappearIdx = al::modi(count + trailIdx + 5, count);
    HosuiTrail* disappearTrail = mTrails.at(disappearIdx);
    if (al::isAlive(disappearTrail))
        disappearTrail->disappear();

    mIndex = al::modi(count + mIndex + 1, count);
}

void HosuiTrailKeeper::forceKillAll() {
    s32 last = mTrails.size() - 1;
    if (last < 0)
        return;
    for (s32 i = 0; i <= last; i++)
        if (al::isAlive(mTrails.at(i)))
            mTrails.at(i)->kill();
}
