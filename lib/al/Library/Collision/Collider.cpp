#include "Library/Collision/Collider.h"

#include "Library/Collision/Angle.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Math/MathUtil.h"

namespace al {

Collider::Collider(CollisionDirector* director, const sead::Matrix34f* actorBaseMtx,
                   const sead::Vector3f* actorTrans, const sead::Vector3f* actorGravity, f32 radius,
                   f32 offsetY, u32 planeNum)
    : mCollisionDirector(director), mActorBaseMtx(actorBaseMtx), mActorTrans(actorTrans),
      mActorGravity(actorGravity), mRadius(radius), mOffsetY(offsetY), mPlaneNum(planeNum) {
    mCurrentTrans.set(*actorTrans);
    mCurrentRadius = radius;
    if (mPlaneNum != 0) {
        mPlanes = new SphereHitInfo[mPlaneNum];
        __asm("");
    } else
        mPlanes = nullptr;
    clear();
    flags2 = (flags2 & 0x80) | 3;
}

void Collider::clear() {
    clearStoredPlaneNum();
    clearContactPlane();
}

void Collider::setTriangleFilter(const TriangleFilterBase* filter) {
    mTriangleFilter = filter;
}

void Collider::setCollisionPartsFilter(const CollisionPartsFilterBase* filter) {
    mCollisionPartsFilter = filter;
}

void Collider::updateRecentOnGroundInfo() {
    if (mFloor_70 >= 0.0f) {
        mNoGroundCounter = 0;
        mRecentOnGroundNormal = mFloorHit.triangle.getFaceNormal();
    } else if (mNoGroundCounter != -1) {
        mNoGroundCounter++;
    }
}

void Collider::clearStoredPlaneNum() {
    mStoredPlaneNum = 0;
}

void Collider::clearContactPlane() {
    mFloor_70 = -99999.0f;
    mWall_70 = -99999.0f;
    mCeiling_70 = -99999.0f;
    mFixReaction = {0.0f, 0.0f, 0.0f};
    mMovePower = {0.0f, 0.0f, 0.0f};
}

void Collider::onInvalidate() {
    clear();
    mNoGroundCounter = -1;
    calcCheckPos(&mCurrentTrans);
    mCurrentRadius = mRadius;
}

void Collider::calcCheckPos(sead::Vector3f* outPos) const {
    // many things are done manually here, which is visible in the assembly as unoptimized
    // reading/writing to memory - for example, the matrix multiplication here ends up writing to
    // outPos 4 times

    *outPos = *mActorTrans;

    if (unknown) {
        if ((flags2 & 0x4) && mActorBaseMtx) {
            outPos->x += mActorBaseMtx->getBase(0).x * unknown->x;
            outPos->x += mActorBaseMtx->getBase(1).x * unknown->y;
            outPos->x += mActorBaseMtx->getBase(2).x * unknown->z;
            outPos->y += mActorBaseMtx->getBase(0).y * unknown->x;
            outPos->y += mActorBaseMtx->getBase(1).y * unknown->y;
            outPos->y += mActorBaseMtx->getBase(2).y * unknown->z;
            outPos->z += mActorBaseMtx->getBase(0).z * unknown->x;
            outPos->z += mActorBaseMtx->getBase(1).z * unknown->y;
            outPos->z += mActorBaseMtx->getBase(2).z * unknown->z;
            return;
        }
        outPos->x += unknown->x;
        outPos->y += unknown->y;
        outPos->z += unknown->z;
        return;
    }

    if (mActorBaseMtx) {
        outPos->x += mActorBaseMtx->getBase(1).x * mOffsetY;
        outPos->y += mActorBaseMtx->getBase(1).y * mOffsetY;
        outPos->z += mActorBaseMtx->getBase(1).z * mOffsetY;
        return;
    }

    outPos->y += mOffsetY;
}

static const sead::Vector3f sDefaultGroundNormal = {0.0f, 1.0f, 0.0f};

const sead::Vector3f& Collider::getRecentOnGroundNormal(u32 coyoteTime) const {
    if (mFloor_70 >= 0.0f)
        return mFloorHit.triangle.getFaceNormal();

    if ((u32)mNoGroundCounter <= coyoteTime)
        return mRecentOnGroundNormal;

    return sDefaultGroundNormal;
}

const Triangle& Collider::getPlane(s32 index) const {
    return mPlanes[index].triangle;
}

bool Collider::calcMovePowerByContact(sead::Vector3f* movePower, const sead::Vector3f& pos) {
    if (mFloor_70 < 0.0f)
        return false;
    if (!mFloorHit.triangle.isHostMoved())
        return false;

    mFloorHit.triangle.calcForceMovePower(movePower, pos);
    const sead::Vector3f& normal = mFloorHit.triangle.getFaceNormal();
    if (normal.dot(*movePower) > 0.0f)
        verticalizeVec(movePower, normal, *movePower);
    return true;
}

u32 Collider::storeCurrentHitInfo(SphereHitInfo* buffer, u32 bufferSize) {
    u32 strikeInfoNum = alCollisionUtil::getStrikeSphereInfoNum(this);
    u32 i;
    for (i = 0; i < strikeInfoNum; i++) {
        if (i + mStoredPlaneNum >= bufferSize) {
            u32 stored = bufferSize - mStoredPlaneNum;
            mStoredPlaneNum = bufferSize;
            return stored;
        }

        const al::SphereHitInfo& info = alCollisionUtil::getStrikeSphereInfo(this, i);
        buffer[i + mStoredPlaneNum] = info;
    }

    mStoredPlaneNum += i;
    return i;
}

// TODO: cleanup
void al::Collider::obtainMomentFixReaction(al::SphereHitInfo* a2, sead::Vector3f* a3,
                                           sead::Vector3f* a4, bool a5, u32 a6) {
    flags2 &= ~(0x10 | 0x20 | 0x40);
    for (u32 i = a6; i < mStoredPlaneNum; i++) {
        const sead::Vector3f& normal = a2[i].triangle.getFaceNormal();
        if (al::isFloorPolygon(normal, *mActorGravity)) {
            if (a2[i].isCollisionAtFace())
                flags2 |= 0x10;
        } else if (al::isWallPolygon(normal, *mActorGravity)) {
            if (a2[i].isCollisionAtFace())
                flags2 |= 0x20;
        } else {
            if (a2[i].isCollisionAtFace())
                flags2 |= 0x40;
        }
    }

    f32 maxAX = 0.0f;
    f32 maxAY = 0.0f;
    f32 maxAZ = 0.0f;
    f32 minAX = 0.0f;
    f32 minAY = 0.0f;
    f32 minAZ = 0.0f;
    f32 maxBX = 0.0f;
    f32 maxBY = 0.0f;
    f32 maxBZ = 0.0f;
    f32 minBX = 0.0f;
    f32 minBY = 0.0f;
    f32 minBZ = 0.0f;

    for (u32 i = a6; i < mStoredPlaneNum; i++) {
        const sead::Vector3f& normal = a2[i].triangle.getFaceNormal();
        sead::Vector3f a;
        sead::Vector3f b;
        if (al::isFloorPolygon(normal, *mActorGravity))
            if (!flags1 || (flags2 & 0x10) != 0)
                a2[i].calcFixVectorNormal(&a, &b);
            else
                a2[i].calcFixVector(&a, &b);
        else if (al::isWallPolygon(normal, *mActorGravity))
            if ((flags2 & 0x20) == 0)
                a2[i].calcFixVector(&a, &b);
            else
                a2[i].calcFixVectorNormal(&a, &b);
        else if ((flags2 & 0x40) == 0)
            a2[i].calcFixVector(&a, &b);
        else
            a2[i].calcFixVectorNormal(&a, &b);

        if (maxAX < a.x)
            maxAX = a.x;
        else if (a.x < minAX)
            minAX = a.x;

        if (maxAY < a.y)
            maxAY = a.y;
        else if (a.y < minAY)
            minAY = a.y;

        if (maxAZ < a.z)
            maxAZ = a.z;
        else if (a.z < minAZ)
            minAZ = a.z;

        if (a4) {
            if (maxBX < b.x)
                maxBX = b.x;
            else if (b.x < minBX)
                minBX = b.x;

            if (maxBY < b.y)
                maxBY = b.y;
            else if (b.y < minBY)
                minBY = b.y;

            if (maxBZ < b.z)
                maxBZ = b.z;
            else if (b.z < minBZ)
                minBZ = b.z;
        }

        if ((flags2 & 2) == 0 || !a2[i].triangle.isHostMoved())
            continue;

        sead::Vector3f collisionMovingReaction = a2[i].collisionMovingReaction;
        if (a.dot(collisionMovingReaction) < 0.0f)
            continue;

        if (maxAX < collisionMovingReaction.x)
            maxAX = collisionMovingReaction.x;
        else if (collisionMovingReaction.x < minAX)
            minAX = collisionMovingReaction.x;

        if (maxAY < collisionMovingReaction.y)
            maxAY = collisionMovingReaction.y;
        else if (collisionMovingReaction.y < minAY)
            minAY = collisionMovingReaction.y;

        if (maxAZ < collisionMovingReaction.z)
            maxAZ = collisionMovingReaction.z;
        else if (collisionMovingReaction.z < minAZ)
            minAZ = collisionMovingReaction.z;

        if (a4) {
            if (maxBX < collisionMovingReaction.x)
                maxBX = collisionMovingReaction.x;
            else if (collisionMovingReaction.x < minBX)
                minBX = collisionMovingReaction.x;

            if (maxBY < collisionMovingReaction.y)
                maxBY = collisionMovingReaction.y;
            else if (collisionMovingReaction.y < minBY)
                minBY = collisionMovingReaction.y;

            if (maxBZ < collisionMovingReaction.z)
                maxBZ = collisionMovingReaction.z;
            else if (!(collisionMovingReaction.z < minBZ))  // FIXME: wrong way around?
                minBZ = collisionMovingReaction.z;
        }
    }

    a3->x = maxAX + minAX;
    a3->y = maxAY + minAY;
    a3->z = maxAZ + minAZ;
    if (a4) {
        a4->x = maxBX + minBX;
        a4->y = maxBY + minBY;
        a4->z = maxBZ + minBZ;
    }
}

void Collider::storeContactPlane(SphereHitInfo* hitInfos) {
    for (u32 i = 0; i < mStoredPlaneNum; i++) {
        const sead::Vector3f& normal = hitInfos[i].triangle.getNormal(0);
        if (isFloorPolygon(normal, *mActorGravity)) {
            if (mFloor_70 < hitInfos[i]._70) {
                mFloorHit = hitInfos[i];
                mFloor_70 = hitInfos[i]._70;
            }
        } else if (isWallPolygon(normal, *mActorGravity)) {
            if (mWall_70 < hitInfos[i]._70) {
                mWallHit = hitInfos[i];
                mWall_70 = hitInfos[i]._70;
            }
        } else if (mCeiling_70 < hitInfos[i]._70) {
            mCeilingHit = hitInfos[i];
            mCeiling_70 = hitInfos[i]._70;
        }
    }
}

// TODO: cleanup
sead::Vector3f al::Collider::collide(const sead::Vector3f& velocity) {
    al::SphereHitInfo* planes;
    u32 planeNum;
    if (mPlaneNum) {
        planes = mPlanes;
        planeNum = mPlaneNum;
    } else {
        getCollisionDirector()->getSphereHitInfoArrayForCollider(&planes, &planeNum);
    }

    sead::Vector3f checkPos = {0.0f, 0.0f, 0.0f};
    calcCheckPos(&checkPos);

    /*-----------------*/

    sead::Vector3f transStart = mCurrentTrans;
    f32 currentRadius = mCurrentRadius;
    f32 v8 = sead::Mathf::clampMax(sead::Mathf::min(0.9f * mRadius, 0.9f * mCurrentRadius), 35.0f);
    sead::Vector3f movePower = {0.0f, 0.0f, 0.0f};
    if ((flags2 & 1) != 0)
        calcMovePowerByContact(&movePower, checkPos);

    clear();
    mMovePower = movePower;
    al::SphereInterpolator interpolator;
    interpolator.startInterp(mCurrentTrans, checkPos + movePower, mCurrentRadius, mRadius, v8);
    s32 v56 = 0;
    sead::Vector3f moveDist = (checkPos + movePower) - mCurrentTrans;
    if (!al::isNearZero(moveDist) || !al::isNearZero(mCurrentRadius - mRadius))
        preCollide(&interpolator, &transStart, &currentRadius, moveDist, planes, planeNum);

    f32 v19 = sead::Mathf::clampMax(mCurrentRadius * 0.9f, 35.0f);
    interpolator.startInterp(transStart, transStart + velocity, mRadius, mRadius, v19);
    v56 = 0;
    sead::Vector3f v232526;
    if (!findCollidePos(&v56, &interpolator, planes, planeNum) &&
        interpolator.getPrevStep() == 1.0 && interpolator.getCurrentStep() == 1.0) {
        v232526 = transStart - checkPos + velocity;
    } else {
        sead::Vector3f v272829 = {0.0f, 0.0f, 0.0f};
        s32 v30 = 0;
        bool v31 = true;
        sead::Vector3f v53;
        do {
            sead::Vector3f v55 = {0.0f, 0.0f, 0.0f};
            sead::Vector3f v54 = {0.0f, 0.0f, 0.0f};
            obtainMomentFixReaction(planes, &v55, &v54, v31, v30);
            v30 += v56;

            interpolator.calcInterp(&transStart, &currentRadius, &v53);
            transStart += v55;
            v272829 += v55;
            sead::Vector3f a1a = v55;
            if (al::isNearZero(a1a))
                a1a = v54;

            al::tryNormalizeOrZero(&a1a);
            f32 v42 = v53.dot(a1a);
            if (v42 < 0.0f)
                v53 -= a1a * v42;

            if (velocity.dot(v53) < 0.0f)
                break;

            interpolator.startInterp(transStart, transStart + v53, currentRadius, mRadius, v19);
            interpolator.nextStep();
            v56 = 0;
            v31 = false;
            bool CollidePos = findCollidePos(&v56, &interpolator, planes, planeNum);
            if (!CollidePos || interpolator.getCurrentStep() >= 1.0f) {
                interpolator.calcInterpPos(&transStart);
                if (CollidePos && v56 > 0) {
                    obtainMomentFixReaction(planes, &v55, nullptr, false, v30);
                    transStart += v55;
                    v30 += v56;
                }
                break;
            } else {
                continue;
            }
        } while (true);

        storeContactPlane(planes);
        mFixReaction = v272829;
        v232526 = transStart - checkPos;
    }

    mCurrentTrans = v232526 + checkPos;
    mCurrentRadius = mRadius;
    updateRecentOnGroundInfo();

    return v232526;
}

// TODO: cleanup
bool Collider::preCollide(al::SphereInterpolator* interpolator, sead::Vector3f* trans,
                          f32* currentRadius, const sead::Vector3f& moveDist,
                          al::SphereHitInfo* buffer, u32 bufferSize) {
    sead::Vector3f v71415 = {0.0f, 0.0f, 0.0f};
    const al::TriangleFilterBase* triangleFilter = mTriangleFilter;
    const al::CollisionPartsFilterBase* collisionPartsFilter = mCollisionPartsFilter;
    bool foundHit = false;
    u32 totalStored = 0;
    while (interpolator->getPrevStep() != 1.0f || interpolator->getCurrentStep() != 1.0f) {
        sead::Vector3f pos;
        f32 size;
        interpolator->calcInterp(&pos, &size, nullptr);

        s32 hits;
        if ((flags2 & 2) != 0) {
            hits = alCollisionUtil::checkStrikeSphereMovingReaction(
                this, v71415 + pos, size, moveDist, collisionPartsFilter, triangleFilter);
        } else {
            hits = alCollisionUtil::checkStrikeSphere(this, pos, size, collisionPartsFilter,
                                                      triangleFilter);
        }

        if (hits != 0) {
            s32 stored = storeCurrentHitInfo(buffer, bufferSize);
            sead::Vector3f a2a = {0.0f, 0.0f, 0.0f};
            obtainMomentFixReaction(buffer, &a2a, nullptr, false, totalStored);
            v71415 += a2a;
            foundHit = true;
            if (interpolator->getCurrentStep() >= 1.0f)
                break;

            if (flags2 & 0x8)
                totalStored += stored;
        }

        interpolator->nextStep();
    }

    interpolator->calcInterp(trans, currentRadius, nullptr);
    *trans += v71415;
    if (foundHit) {
        storeContactPlane(buffer);
        mStoredPlaneNum = 0;
    }

    return foundHit;
}

bool Collider::findCollidePos(s32* bufferStored, al::SphereInterpolator* interpolator,
                              al::SphereHitInfo* buffer, u32 bufferSize) {
    const al::TriangleFilterBase* triangleFilter = mTriangleFilter;
    const al::CollisionPartsFilterBase* collisionPartsFilter = mCollisionPartsFilter;
    while (interpolator->getPrevStep() != 1.0 || interpolator->getCurrentStep() != 1.0) {
        sead::Vector3f pos, remainMoveVec;
        f32 size;
        interpolator->calcInterp(&pos, &size, &remainMoveVec);

        s32 hits;
        if (flags2 & 0x2) {
            hits = alCollisionUtil::checkStrikeSphereMovingReaction(
                this, pos, size, remainMoveVec, collisionPartsFilter, triangleFilter);
        } else {
            hits = alCollisionUtil::checkStrikeSphere(this, pos, size, collisionPartsFilter,
                                                      triangleFilter);
        }

        if (hits != 0) {
            u32 stored = storeCurrentHitInfo(buffer, bufferSize);
            if (bufferStored)
                *bufferStored = stored;

            return true;
        }

        interpolator->nextStep();
    }

    return false;
}

CollisionDirector* Collider::getCollisionDirector() const {
    return mCollisionDirector;
}

}  // namespace al
