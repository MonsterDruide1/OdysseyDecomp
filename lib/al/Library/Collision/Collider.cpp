#include "Library/Collision/Collider.h"
#include "Library/Collision/Angle.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Math/MathUtil.h"

namespace al {

// TODO: match
Collider::Collider(CollisionDirector* director, const sead::Matrix34f* actorBaseMtx,
                   const sead::Vector3f* actorTrans, const sead::Vector3f* actorGravity, f32 radius,
                   f32 offsetY, u32 planeNum)
    : mCollisionDirector(director), mActorBaseMtx(actorBaseMtx), mActorTrans(actorTrans),
      mActorGravity(actorGravity), mRadius(radius), mOffsetY(offsetY), mPlaneNum(planeNum) {
    mCurrentTrans.set(*actorTrans);
    mCurrentRadius = radius;
    if (mPlaneNum != 0)
        mPlanes = new SphereHitInfo[mPlaneNum];
    else
        mPlanes = nullptr;
    // mPlanes = mPlaneNum != 0 ? new SphereHitInfo[mPlaneNum] : nullptr;
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
    _64 = {0.0f, 0.0f, 0.0f};
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

        al::SphereHitInfo* strikeInfo = alCollisionUtil::getStrikeSphereInfo(this, i);
        buffer[i + mStoredPlaneNum] = *strikeInfo;
    }

    mStoredPlaneNum += i;
    return i;
}

// TODO: cleanup
void al::Collider::obtainMomentFixReaction(al::SphereHitInfo* a2, sead::Vector3f* a3,
                                           sead::Vector3f* a4, bool a5, u32 a6) {
    flags2 &= 0x8Fu;
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

    float maxVecX;
    float maxVecY;
    float maxVecZ;
    float minVecX;
    float minVecY;
    float minVecZ;
    float minNormX;
    float minNormY;
    float minNormZ;
    float sX;
    float sY;
    float sZ;
    float internalX;
    float internalY;
    float internalZ;
    al::SphereHitInfo* v35;
    sead::Vector3f fixNormal;
    sead::Vector3f fixVector;

    maxVecX = 0.0f;
    maxVecY = 0.0f;
    maxVecZ = 0.0f;
    minVecX = 0.0f;
    minVecY = 0.0f;
    minVecZ = 0.0f;
    minNormX = 0.0f;
    minNormY = 0.0f;
    minNormZ = 0.0f;
    sX = 0.0f;
    sY = 0.0f;
    sZ = 0.0f;
    internalX = 0.0f;
    internalY = 0.0f;
    internalZ = 0.0f;

    for (u32 i2 = a6; i2 < mStoredPlaneNum; i2++) {
        for (; 1;) {
            const sead::Vector3f& normal = a2[i2].triangle.getFaceNormal();
            if (al::isFloorPolygon(normal, *mActorGravity))
                if (!flags1 || (flags2 & 0x10) != 0)
                    a2[i2].calcFixVectorNormal(&fixVector, &fixNormal);
                else
                    a2[i2].calcFixVector(&fixVector, &fixNormal);
            else if (al::isWallPolygon(normal, *mActorGravity))
                if ((flags2 & 0x20) == 0)
                    a2[i2].calcFixVector(&fixVector, &fixNormal);
                else
                    a2[i2].calcFixVectorNormal(&fixVector, &fixNormal);
            else if ((flags2 & 0x40) == 0)
                a2[i2].calcFixVector(&fixVector, &fixNormal);
            else
                a2[i2].calcFixVectorNormal(&fixVector, &fixNormal);

            if (maxVecX < fixVector.x) {
                maxVecX = fixVector.x;
            } else if (fixVector.x < internalX) {
                minVecX = fixVector.x;
                goto endX;
            }

            minVecX = internalX;

        endX:
            if (maxVecY < fixVector.y) {
                maxVecY = fixVector.y;
            } else if (fixVector.y < internalY) {
                minVecY = fixVector.y;
                goto endY;
            }

            minVecY = internalY;

        endY:
            if (maxVecZ < fixVector.z) {
                maxVecZ = fixVector.z;
            } else if (fixVector.z < internalZ) {
                minVecZ = fixVector.z;
                goto endZ;
            }

            minVecZ = internalZ;

        endZ:
            internalY = sZ;
            internalX = sY;
            internalZ = sX;

            if (a4) {
                if (minNormX < fixNormal.x) {
                    minNormX = fixNormal.x;
                } else if (fixNormal.x < internalZ) {
                    internalZ = fixNormal.x;
                    goto end2X;
                }

                internalZ = internalZ;

            end2X:
                if (minNormY < fixNormal.y) {
                    minNormY = fixNormal.y;
                } else if (fixNormal.y < internalX) {
                    internalX = fixNormal.y;
                    goto end2Y;
                }

                internalX = internalX;

            end2Y:
                if (minNormZ < fixNormal.z) {
                    minNormZ = fixNormal.z;
                } else if (fixNormal.z < internalY) {
                    internalY = fixNormal.z;
                    goto end2Z;
                }

                internalY = internalY;

            end2Z:;
            }

            if ((flags2 & 2) == 0)
                goto LABEL_80;

            if (!a2[i2].triangle.isHostMoved())
                goto LABEL_80;

            sX = a2[i2].collisionMovingReaction.x;
            sY = a2[i2].collisionMovingReaction.y;
            sZ = a2[i2].collisionMovingReaction.z;
            if ((((fixVector.x * sX) + (fixVector.y * sY)) + (fixVector.z * sZ)) < 0.0)
                goto LABEL_80;

            if (maxVecX < sX)
                maxVecX = sX;
            else if (sX < minVecX)
                minVecX = sX;

            if (maxVecY < sY)
                maxVecY = sY;
            else if (sY < minVecY)
                minVecY = sY;

            if (maxVecZ < sZ)
                maxVecZ = sZ;
            else if (sZ < minVecZ)
                minVecZ = sZ;

            if (a4) {
                if (minNormX < sX)
                    minNormX = sX;
                else if (sX < internalZ)
                    goto end3X;

                sX = internalZ;

            end3X:
                if (minNormY < sY)
                    minNormY = sY;
                else if (sY < internalX)
                    goto end3Y;

                sY = internalX;

            end3Y:
                if (minNormZ < sZ)
                    minNormZ = sZ;
                else if (sZ < internalY)
                    goto end3Z;
                sZ = internalY;

            end3Z:;
            } else {
            LABEL_80:;
                sX = internalZ;
                sY = internalX;
                sZ = internalY;
            }

            internalX = minVecX;
            internalY = minVecY;
            internalZ = minVecZ;
            i2++;
            if (i2 >= mStoredPlaneNum)
                goto loopBreak;
        }
    }

loopBreak:
    a3->x = maxVecX + minVecX;
    a3->y = maxVecY + minVecY;
    a3->z = maxVecZ + minVecZ;
    if (a4) {
        a4->x = minNormX + sX;
        a4->y = minNormY + sY;
        a4->z = minNormZ + sZ;
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
    f32 a4 = mCurrentRadius;
    f32 v8 = sead::Mathf::clampMax(sead::Mathf::min(mRadius * 0.9f, a4 * 0.9f), 35.0f);
    sead::Vector3f movePower = {0.0f, 0.0f, 0.0f};
    sead::Vector3f xyz = {0.0f, 0.0f, 0.0f};
    sead::Vector3f v101314 = {0.0f, 0.0f, 0.0f};
    if ((flags2 & 1) != 0 && calcMovePowerByContact(&movePower, checkPos)) {
        v101314 = movePower;
        xyz = movePower;
    }

    clear();
    _64 = v101314;
    al::SphereInterpolator v58;
    v58.startInterp(mCurrentTrans, checkPos + xyz, mCurrentRadius, mRadius, v8);
    s32 v56 = 0;
    sead::Vector3f transEnd = (checkPos + movePower) - mCurrentTrans;
    if (!al::isNearZero(transEnd) || !al::isNearZero(mCurrentRadius - mRadius))
        preCollide(&v58, &transStart, &a4, transEnd, planes, planeNum);

    f32 v19 = sead::Mathf::clampMax(mCurrentRadius * 0.9f, 35.0f);
    v58.startInterp(transStart, transStart + velocity, mRadius, mRadius, v19);
    v56 = 0;
    sead::Vector3f v212224;
    sead::Vector3f v232526;
    if ((findCollidePos(&v56, &v58, planes, planeNum) & 1) == 0 && v58.getPrevStep() == 1.0 &&
        v58.getCurrentStep() == 1.0) {
        v212224 = checkPos;
        v232526 = transStart - checkPos + velocity;
    } else {
        sead::Vector3f v272829 = {0.0f, 0.0f, 0.0f};
        s32 v30 = 0;
        bool v31 = true;
        sead::Vector3f v363738;
        sead::Vector3f v53;
        do {
            sead::Vector3f v55 = {0.0f, 0.0f, 0.0f};
            sead::Vector3f v54 = {0.0f, 0.0f, 0.0f};
            obtainMomentFixReaction(planes, &v55, &v54, v31, v30);
            s32 v32 = v56;

            v58.calcInterp(&transStart, &a4, &v53);
            sead::Vector3f v333435;
            v333435 = v55;
            transStart += v55;
            sead::Vector3f a1a = v55;
            if (al::isNearZero(a1a))
                a1a = v54;

            v363738 = v272829 + v333435;
            v30 += v32;
            al::tryNormalizeOrZero(&a1a);
            f32 v42 = v53.dot(a1a);
            if (v42 < 0.0)
                v53 -= a1a * v42;

            v272829 = v363738;
            if (velocity.dot(v53) < 0.0)
                break;

            v58.startInterp(transStart, transStart + v53, a4, mRadius, v19);
            v58.nextStep();
            v56 = 0;
            bool CollidePos = findCollidePos(&v56, &v58, planes, planeNum);
            if (CollidePos && v58.getCurrentStep() < 1.0f) {
                v31 = false;
                continue;
            } else {
                v58.calcInterpPos(&transStart);
                if (CollidePos && (v56 > 0)) {
                    obtainMomentFixReaction(planes, &v55, 0LL, 0, v30);
                    transStart += v55;
                    v30 += v56;
                }
                break;
            }
        } while (true);

        storeContactPlane(planes);
        mFixReaction = v363738;
        v212224 = checkPos;
        v232526 = transStart - checkPos;
    }

    mCurrentTrans = v232526 + v212224;
    mCurrentRadius = mRadius;
    updateRecentOnGroundInfo();

    return v232526;
}

// TODO: cleanup
bool Collider::preCollide(al::SphereInterpolator* interpolator, sead::Vector3<float>* a3, float* a4,
                          sead::Vector3<float> const& a5, al::SphereHitInfo* buffer,
                          u32 bufferSize) {
    auto a1 = this;
    const al::TriangleFilterBase* triangleFilter;
    const al::CollisionPartsFilterBase* collisionPartsFilter;
    unsigned int v13;
    int v20;
    int v21;
    int v22;
    float a3a;
    sead::Vector3f v28;

    sead::Vector3f v71415 = {0.0f, 0.0f, 0.0f};
    triangleFilter = mTriangleFilter;
    collisionPartsFilter = mCollisionPartsFilter;
    v13 = 0;
    bool foundHit = 0;
    while (interpolator->getPrevStep() != 1.0 || interpolator->getCurrentStep() != 1.0) {
        interpolator->calcInterp(&v28, &a3a, 0LL);
        if ((flags2 & 2) != 0) {
            v20 = alCollisionUtil::checkStrikeSphereMovingReaction(
                a1, v71415 + v28, a3a, a5, collisionPartsFilter, triangleFilter);
        } else {
            v20 = alCollisionUtil::checkStrikeSphere(a1, v28, a3a, collisionPartsFilter,
                                                     triangleFilter);
        }

        if (v20) {
            v21 = storeCurrentHitInfo(buffer, bufferSize);
            sead::Vector3f a2a = {0.0f, 0.0f, 0.0f};
            obtainMomentFixReaction(buffer, &a2a, nullptr, false, v13);
            v71415 += a2a;
            if (interpolator->getCurrentStep() >= 1.0) {
                foundHit = 1;
                break;
            }

            v22 = v21 & (flags2 << 28 >> 31);
            foundHit = 1;
            v13 += v22;
        }

        interpolator->nextStep();
    }

    interpolator->calcInterp(a3, a4, 0LL);
    *a3 += v71415;
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
