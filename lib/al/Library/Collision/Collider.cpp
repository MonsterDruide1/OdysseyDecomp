#include "Library/Collision/Collider.h"

#include "Library/Collision/Angle.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionUtil.h"
#include "Library/Collision/SphereInterpolator.h"
#include "Library/Math/MathUtil.h"

namespace al {

Collider::Collider(CollisionDirector* director, const sead::Matrix34f* mtxPtr,
                   const sead::Vector3f* transPtr, const sead::Vector3f* gravityPtr, f32 radius,
                   f32 offsetY, u32 planeNum)
    : mDirector(director), mActorMtxPtr(mtxPtr), mActorTransPtr(transPtr),
      mActorGravityPtr(gravityPtr), mRadius(radius), mOffsetY(offsetY), mPlaneNum(planeNum) {
    mCurrentTrans.set(*transPtr);
    mCurrentRadius = radius;

    if (mPlaneNum == 0)
        mPlanes = nullptr;
    else
        mPlanes = new SphereHitInfo[mPlaneNum];

    clear();
    mIsReactMovePower = true;
    mIsCheckMovingReaction = true;
    mIsRotateCheckOffset = false;
    mIsSkipStoredHitInfos = false;
    mIsCollidedGround = false;
    mIsCollidedWallFace = false;
    mIsCollidedCeilingFace = false;
}

void Collider::clear() {
    clearStoredPlaneNum();
    clearContactPlane();
}

void Collider::setTriangleFilter(const TriangleFilterBase* filter) {
    mTriFilter = filter;
}

void Collider::setCollisionPartsFilter(const CollisionPartsFilterBase* filter) {
    mPartsFilter = filter;
}

void Collider::updateRecentOnGroundInfo() {
    if (isCollidedGround()) {
        mNoGroundCounter = 0;
        mRecentOnGroundNormal = mGroundHit.triangle.getFaceNormal();
    } else if (mNoGroundCounter != sead::Mathu::maxNumber()) {
        mNoGroundCounter++;
    }
}

void Collider::clearStoredPlaneNum() {
    mStoredPlaneNum = 0;
}

void Collider::clearContactPlane() {
    mGroundDistance = -99999.0f;
    mWallDistance = -99999.0f;
    mCeilingDistance = -99999.0f;

    mFixReaction = {0.0f, 0.0f, 0.0f};
    mMovePower = {0.0f, 0.0f, 0.0f};
}

void Collider::onInvalidate() {
    clear();
    mNoGroundCounter = -1;
    calcCheckPos(&mCurrentTrans);
    mCurrentRadius = mRadius;
}

void Collider::calcCheckPos(sead::Vector3f* outPosition) const {
    // Many things are done manually here, which is visible in the assembly as unoptimized
    // reading/writing to memory - for example, the matrix multiplication here ends up writing to
    // outPosition 4 times

    *outPosition = *mActorTransPtr;

    if (mCheckOffset) {
        if (mIsRotateCheckOffset && mActorMtxPtr) {
            outPosition->x += getActorMtxSide().x * mCheckOffset->x;
            outPosition->x += getActorMtxUp().x * mCheckOffset->y;
            outPosition->x += getActorMtxFront().x * mCheckOffset->z;

            outPosition->y += getActorMtxSide().y * mCheckOffset->x;
            outPosition->y += getActorMtxUp().y * mCheckOffset->y;
            outPosition->y += getActorMtxFront().y * mCheckOffset->z;

            outPosition->z += getActorMtxSide().z * mCheckOffset->x;
            outPosition->z += getActorMtxUp().z * mCheckOffset->y;
            outPosition->z += getActorMtxFront().z * mCheckOffset->z;
        } else {
            outPosition->x += mCheckOffset->x;
            outPosition->y += mCheckOffset->y;
            outPosition->z += mCheckOffset->z;
        }
        return;
    }

    if (mActorMtxPtr) {
        outPosition->x += getActorMtxUp().x * mOffsetY;
        outPosition->y += getActorMtxUp().y * mOffsetY;
        outPosition->z += getActorMtxUp().z * mOffsetY;
    } else {
        outPosition->y += mOffsetY;
    }
}

static const sead::Vector3f sDefaultGroundNormal = {0.0f, 1.0f, 0.0f};

const sead::Vector3f& Collider::getRecentOnGroundNormal(u32 coyoteTime) const {
    if (isCollidedGround())
        return mGroundHit.triangle.getFaceNormal();

    if (mNoGroundCounter <= coyoteTime)
        return mRecentOnGroundNormal;

    return sDefaultGroundNormal;
}

const Triangle& Collider::getPlane(s32 index) const {
    return mPlanes[index].triangle;
}

bool Collider::calcMovePowerByContact(sead::Vector3f* outMovePower,
                                      const sead::Vector3f& position) {
    if (mGroundDistance < 0.0f)
        return false;
    if (!mGroundHit.triangle.isHostMoved())
        return false;

    mGroundHit.triangle.calcForceMovePower(outMovePower, position);

    const sead::Vector3f& normal = mGroundHit.triangle.getFaceNormal();
    if (normal.dot(*outMovePower) > 0.0f)
        verticalizeVec(outMovePower, normal, *outMovePower);

    return true;
}

u32 Collider::storeCurrentHitInfo(SphereHitInfo* planes, u32 planeNum) {
    u32 strikeInfoNum = alCollisionUtil::getStrikeSphereInfoNum(this);

    u32 idx;
    for (idx = 0; idx < strikeInfoNum; idx++) {
        if (idx + mStoredPlaneNum >= planeNum) {
            u32 stored = planeNum - mStoredPlaneNum;
            mStoredPlaneNum = planeNum;
            return stored;
        }

        const auto& info = alCollisionUtil::getStrikeSphereInfo(this, idx);
        planes[idx + mStoredPlaneNum] = info;
    }

    mStoredPlaneNum += idx;
    return idx;
}

static void updateMinMax(sead::Vector3f* outMax, sead::Vector3f* outMin,
                         const sead::Vector3f& value) {
    if (outMax->x < value.x)
        outMax->x = value.x;
    else if (value.x < outMin->x)
        outMin->x = value.x;

    if (outMax->y < value.y)
        outMax->y = value.y;
    else if (value.y < outMin->y)
        outMin->y = value.y;

    if (outMax->z < value.z)
        outMax->z = value.z;
    else if (value.z < outMin->z)
        outMin->z = value.z;
}

// NON_MATCHING: Register allocation/Instruction ordering (https://decomp.me/scratch/4jDXG)
void Collider::obtainMomentFixReaction(SphereHitInfo* planes, sead::Vector3f* outFixReaction,
                                       sead::Vector3f* outFixReactionNormal, bool isFirst,
                                       u32 startIndex) {
    if (isFirst) {
        // This check is required, such that the isFirst parameter isn't considered unused
    }

    mIsCollidedGround = false;
    mIsCollidedWallFace = false;
    mIsCollidedCeilingFace = false;

    for (u32 idx = startIndex; idx < mStoredPlaneNum; idx++) {
        const SphereHitInfo& hitInfo = planes[idx];
        const sead::Vector3f& normal = hitInfo.triangle.getFaceNormal();

        if (isFloorPolygon(normal, *mActorGravityPtr)) {
            if (hitInfo.isCollisionAtFace())
                mIsCollidedGround = true;
        } else if (isWallPolygon(normal, *mActorGravityPtr)) {
            if (hitInfo.isCollisionAtFace())
                mIsCollidedWallFace = true;
        } else {
            if (hitInfo.isCollisionAtFace())
                mIsCollidedCeilingFace = true;
        }
    }

    sead::Vector3f minFix = {0.0f, 0.0f, 0.0f};
    sead::Vector3f maxFix = {0.0f, 0.0f, 0.0f};
    sead::Vector3f maxFixNormal = {0.0f, 0.0f, 0.0f};
    sead::Vector3f minFixNormal = {0.0f, 0.0f, 0.0f};

    for (u32 idx = startIndex; idx < mStoredPlaneNum; idx++) {
        const auto& plane = planes[idx];
        const auto& normal = plane.triangle.getFaceNormal();

        sead::Vector3f fix, fixNormal;
        if (isFloorPolygon(normal, *mActorGravityPtr))
            if (!mIsDisableGroundNormalCheck || mIsCollidedGround)
                plane.calcFixVectorNormal(&fix, &fixNormal);
            else
                plane.calcFixVector(&fix, &fixNormal);
        else if (isWallPolygon(normal, *mActorGravityPtr))
            if (mIsCollidedWallFace)
                plane.calcFixVectorNormal(&fix, &fixNormal);
            else
                plane.calcFixVector(&fix, &fixNormal);
        else if (!mIsCollidedCeilingFace)
            plane.calcFixVector(&fix, &fixNormal);
        else
            plane.calcFixVectorNormal(&fix, &fixNormal);

        updateMinMax(&maxFix, &minFix, fix);
        if (outFixReactionNormal)
            updateMinMax(&maxFixNormal, &minFixNormal, fixNormal);

        if (!mIsCheckMovingReaction || !plane.triangle.isHostMoved())
            continue;
        if (fix.dot(plane.movingReaction) < 0.0f)
            continue;

        updateMinMax(&maxFix, &minFix, plane.movingReaction);
        if (outFixReactionNormal)
            updateMinMax(&maxFixNormal, &minFixNormal, plane.movingReaction);
    }

    outFixReaction->setAdd(maxFix, minFix);
    if (outFixReactionNormal)
        *outFixReactionNormal = maxFixNormal + minFixNormal;
}

void Collider::storeContactPlane(SphereHitInfo* planes) {
    for (u32 idx = 0; idx < mStoredPlaneNum; idx++) {
        const auto& plane = planes[idx];
        const auto& normal = plane.triangle.getNormal(0);

        if (isFloorPolygon(normal, *mActorGravityPtr)) {
            if (mGroundDistance < plane.distance) {
                mGroundHit = plane;
                mGroundDistance = plane.distance;
            }
        } else if (isWallPolygon(normal, *mActorGravityPtr)) {
            if (mWallDistance < plane.distance) {
                mWallHit = plane;
                mWallDistance = plane.distance;
            }
        } else if (mCeilingDistance < plane.distance) {
            mCeilingHit = plane;
            mCeilingDistance = plane.distance;
        }
    }
}

// NON_MATCHING: fadd operands swapped (https://decomp.me/scratch/QutF9)
sead::Vector3f Collider::collide(const sead::Vector3f& velocity) {
    u32 planeNum;
    SphereHitInfo* planes;

    if (mPlaneNum != 0) {
        planes = mPlanes;
        planeNum = mPlaneNum;
    } else {
        getCollisionDirector()->getSphereHitInfoArrayForCollider(&planes, &planeNum);
    }

    sead::Vector3f checkPos = {0.0f, 0.0f, 0.0f};
    calcCheckPos(&checkPos);

    sead::Vector3f position = mCurrentTrans;
    f32 radius = mCurrentRadius;
    f32 step =
        sead::Mathf::clampMax(sead::Mathf::min(0.9f * mRadius, 0.9f * mCurrentRadius), 35.0f);

    sead::Vector3f movePower = {0.0f, 0.0f, 0.0f};
    if (mIsReactMovePower)
        calcMovePowerByContact(&movePower, checkPos);

    clear();
    mMovePower = movePower;

    SphereInterpolator interp;
    interp.startInterp(mCurrentTrans, checkPos + movePower, mCurrentRadius, mRadius, step);
    s32 hitNum = 0;

    sead::Vector3f preMove = (checkPos + movePower) - mCurrentTrans;
    if (!isNearZero(preMove) || !isNearZero(mCurrentRadius - mRadius))
        preCollide(&interp, &position, &radius, preMove, planes, planeNum);

    step = sead::Mathf::clampMax(mCurrentRadius * 0.9f, 35.0f);
    interp.startInterp(position, position + velocity, mRadius, mRadius, step);
    hitNum = 0;

    sead::Vector3f result;
    if (!findCollidePos(&hitNum, &interp, planes, planeNum) && interp.isEnd()) {
        result = position - checkPos + velocity;
    } else {
        sead::Vector3f fixReaction = {0.0f, 0.0f, 0.0f};
        s32 startIndex = 0;
        bool isFirst = true;
        bool isContinue;

        do {
            sead::Vector3f fix = {0.0f, 0.0f, 0.0f};
            sead::Vector3f fixNormal = {0.0f, 0.0f, 0.0f};
            obtainMomentFixReaction(planes, &fix, &fixNormal, isFirst, startIndex);
            isFirst = false;
            startIndex += hitNum;

            sead::Vector3f remainMove;
            interp.calcInterp(&position, &radius, &remainMove);
            position += fix;
            fixReaction += fix;

            sead::Vector3f fixDir = fix;
            if (isNearZero(fixDir))
                fixDir.set(fixNormal);
            tryNormalizeOrZero(&fixDir);

            f32 remainDot = remainMove.dot(fixDir);
            if (remainDot < 0.0f)
                remainMove.setScaleAdd(-remainDot, fixDir, remainMove);

            isContinue = false;

            if (!(velocity.dot(remainMove) < 0.0f)) {
                interp.startInterp(position, position + remainMove, radius, mRadius, step);
                interp.nextStep();
                hitNum = 0;
                bool isHit = findCollidePos(&hitNum, &interp, planes, planeNum);

                if (isHit && !interp.isDone()) {
                    isContinue = true;
                } else {
                    interp.calcInterpPos(&position);
                    if (isHit && hitNum > 0) {
                        obtainMomentFixReaction(planes, &fix, nullptr, false, startIndex);
                        position += fix;
                        startIndex += hitNum;
                    }
                }
            }
        } while (isContinue);

        storeContactPlane(planes);
        mFixReaction = fixReaction;
        result = position - checkPos;
    }

    mCurrentTrans.setAdd(checkPos, result);
    mCurrentRadius = mRadius;
    updateRecentOnGroundInfo();

    return result;
}

bool Collider::preCollide(SphereInterpolator* interp, sead::Vector3f* outPosition, f32* outRadius,
                          const sead::Vector3f& velocity, SphereHitInfo* planes, u32 planeNum) {
    auto* triFilter = mTriFilter;
    auto* partsFilter = mPartsFilter;

    sead::Vector3f totalFix = {0.0f, 0.0f, 0.0f};
    bool isHit = false;
    u32 totalStored = 0;

    while (!interp->isEnd()) {
        sead::Vector3f position;
        f32 radius;
        interp->calcInterp(&position, &radius, nullptr);

        u32 strikeNum;
        if (mIsCheckMovingReaction) {
            strikeNum = alCollisionUtil::checkStrikeSphereMovingReaction(
                this, totalFix + position, radius, velocity, partsFilter, triFilter);
        } else {
            strikeNum =
                alCollisionUtil::checkStrikeSphere(this, position, radius, partsFilter, triFilter);
        }

        if (strikeNum != 0) {
            s32 stored = storeCurrentHitInfo(planes, planeNum);

            sead::Vector3f fix = {0.0f, 0.0f, 0.0f};
            obtainMomentFixReaction(planes, &fix, nullptr, false, totalStored);
            totalFix += fix;
            isHit = true;

            if (interp->isDone())
                break;

            if (mIsSkipStoredHitInfos)
                totalStored += stored;
        }

        interp->nextStep();
    }

    interp->calcInterp(outPosition, outRadius, nullptr);
    *outPosition += totalFix;

    if (isHit) {
        storeContactPlane(planes);
        clearStoredPlaneNum();
    }

    return isHit;
}

bool Collider::findCollidePos(s32* outStored, SphereInterpolator* interp, SphereHitInfo* planes,
                              u32 planeNum) {
    auto* triFilter = mTriFilter;
    auto* partsFilter = mPartsFilter;

    while (!interp->isEnd()) {
        sead::Vector3f position;
        sead::Vector3f remainMove;
        f32 radius;
        interp->calcInterp(&position, &radius, &remainMove);

        u32 strikeNum;
        if (mIsCheckMovingReaction) {
            strikeNum = alCollisionUtil::checkStrikeSphereMovingReaction(
                this, position, radius, remainMove, partsFilter, triFilter);
        } else {
            strikeNum =
                alCollisionUtil::checkStrikeSphere(this, position, radius, partsFilter, triFilter);
        }

        if (strikeNum != 0) {
            u32 stored = storeCurrentHitInfo(planes, planeNum);
            if (outStored)
                *outStored = stored;

            return true;
        }

        interp->nextStep();
    }

    return false;
}

CollisionDirector* Collider::getCollisionDirector() const {
    return mDirector;
}

}  // namespace al
