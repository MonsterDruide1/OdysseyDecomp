#include "Library/Collision/CollisionParts.h"

#include <math/seadVector.h>

#include "Library/Collision/CollisionUtil.h"
#include "Library/Collision/HitInfo.h"
#include "Library/Collision/ICollisionPartsKeeper.h"
#include "Library/Collision/KCollisionFunc.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Collision/SphereInterpolator.h"
#include "Library/Collision/TriangleFilter.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Math/MatrixUtil.h"

namespace {
using namespace al;

class TriFilterMoveReaction : public TriangleFilterBase {
public:
    TriFilterMoveReaction(const sead::Vector3f* moveDir, const TriangleFilterBase* filter)
        : mMoveDir(moveDir), mFilter(filter) {}

    bool isInvalidTriangle(const Triangle& triangle) const override;

    void setMoveDirection(const sead::Vector3f* moveDir) { mMoveDir = moveDir; }

private:
    const sead::Vector3f* mMoveDir;
    const TriangleFilterBase* mFilter;
};
}  // namespace

namespace al {

// TODO: These are probably not actually defined here, but rather in some utility header
static sead::Vector3f calcVecDiff(sead::Vector3f vecA, sead::Vector3f vecB) {
    return vecB - vecA;
}

static bool isEqualMtx(const sead::Matrix34f& rA, const sead::Matrix34f& rB) {
    return rA.m[0][0] == rB.m[0][0] && rA.m[0][1] == rB.m[0][1] && rA.m[0][2] == rB.m[0][2] &&
           rA.m[0][3] == rB.m[0][3] && rA.m[1][0] == rB.m[1][0] && rA.m[1][1] == rB.m[1][1] &&
           rA.m[1][2] == rB.m[1][2] && rA.m[1][3] == rB.m[1][3] && rA.m[2][0] == rB.m[2][0] &&
           rA.m[2][1] == rB.m[2][1] && rA.m[2][2] == rB.m[2][2] && rA.m[2][3] == rB.m[2][3];
}

CollisionParts::CollisionParts(void* kclData, const void* bymlData) : TListNode(this) {
    mServer = new KCollisionServer();
    mServer->initKCollisionServer(kclData, bymlData);

    mPrevBaseMtx.makeIdentity();
    mBaseMtx.makeIdentity();
    mSyncMtx.makeIdentity();
    mPrevBaseInvMtx.makeIdentity();
    mBaseInvMtx.makeIdentity();

    calcInvMtxScale();
}

void CollisionParts::calcInvMtxScale() {
    calcMtxScale(&mMtxScaleVec, mBaseInvMtx);

    mMtxScale = (mMtxScaleVec.x + mMtxScaleVec.y + mMtxScaleVec.z) / 3.0f;
    mInvMtxScale = 1.0f / mMtxScale;
}

const LiveActor* CollisionParts::getConnectedHost() const {
    return getSensorHost(mConnectedSensor);
}

void CollisionParts::initParts(const sead::Matrix34f& mtx) {
    resetAllMtx(mtx);

    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    calcMtxScale(&scale, mBaseMtx);

    mServer->calcFarthestVertexDistance();

    updateBoundingSphereRange(scale);
}

void CollisionParts::resetAllMtx(const sead::Matrix34f& matrix) {
    if (!isUpdate())
        return;

    sead::Matrix34f mtx;
    mtx = matrix;
    makeEqualScale(&mtx);
    resetAllMtxPrivate(mtx);

    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    calcMtxScale(&scale, mBaseMtx);
    updateBoundingSphereRange(scale);
}

void CollisionParts::updateBoundingSphereRange(sead::Vector3f scale) {
    updateBoundingSphereRangePrivate((scale.x + scale.y + scale.z) / 3.0f);
}

void CollisionParts::validateByUser() {
    if (mIsValidatedByUser)
        return;

    bool wasValid = isValid();
    mIsValidatedByUser = true;

    if (wasValid) {
        mIsJustValidated = false;
    } else {
        mIsJustValidated = isValid();

        if (mIsJustValidated) {
            alCollisionUtil::getCollisionPartsKeeper(getConnectedHost())
                ->connectToCollisionPartsList(this);
        }
    }
}

void CollisionParts::invalidateByUser() {
    if (!mIsValidatedByUser)
        return;

    bool wasValid = isValid();
    mIsValidatedByUser = false;
    mIsJustValidated = false;

    if (wasValid && !isValid() && mList != nullptr) {
        alCollisionUtil::getCollisionPartsKeeper(getConnectedHost())
            ->disconnectToCollisionPartsList(this);
    }
}

void CollisionParts::validateBySystem() {
    if (mIsValidatedBySystem)
        return;

    bool wasValid = isValid();
    mIsValidatedBySystem = true;

    if (wasValid) {
        mIsJustValidated = false;
    } else {
        mIsJustValidated = isValid();

        if (mIsJustValidated) {
            alCollisionUtil::getCollisionPartsKeeper(getConnectedHost())
                ->connectToCollisionPartsList(this);
        }
    }
}

void CollisionParts::invalidateBySystem() {
    if (!mIsValidatedBySystem)
        return;

    bool wasValid = isValid();
    mIsValidatedBySystem = false;
    mIsJustValidated = false;

    if (wasValid && !isValid() && mList != nullptr) {
        alCollisionUtil::getCollisionPartsKeeper(getConnectedHost())
            ->disconnectToCollisionPartsList(this);
    }
}

void CollisionParts::onJoinList() {}

f32 CollisionParts::makeEqualScale(sead::Matrix34f* matrix) {
    sead::Vector3f scale;
    calcMtxScale(&scale, *matrix);

    if (isNearZero(sead::Vector3f(scale.x - scale.y, scale.y - scale.z, scale.z - scale.x)))
        return scale.x;  // Already mostly equal

    f32 targetScale;
    sead::Vector3f scaleMult;

    switch (mForceCollisionScale) {
    case ForceCollisionScale::One: {
        targetScale = 1.0f;

        scaleMult.x = targetScale / scale.x;
        scaleMult.y = targetScale / scale.y;
        scaleMult.z = targetScale / scale.z;
        break;
    }
    case ForceCollisionScale::Average: {
        targetScale = (scale.x + scale.y + scale.z) / 3.0f;

        scaleMult.x = targetScale / scale.x;
        scaleMult.y = targetScale / scale.y;
        scaleMult.z = targetScale / scale.z;
        break;
    }

    default:
        targetScale = 1.0f;
        break;
    }

    matrix->scaleBases(scaleMult.x, scaleMult.y, scaleMult.z);
    return targetScale;
}

void CollisionParts::resetAllMtxPrivate(const sead::Matrix34f& matrix) {
    mPrevBaseMtx = matrix;
    mBaseMtx = matrix;
    mSyncMtx = matrix;

    mPrevBaseInvMtx.setInverse(mPrevBaseMtx);
    mBaseInvMtx = mPrevBaseInvMtx;

    calcInvMtxScale();
}

void CollisionParts::resetAllMtx() {
    if (!isUpdate())
        return;

    sead::Matrix34f mtx;
    mtx = *mSyncCollisionMtxPtr;
    makeEqualScale(&mtx);
    resetAllMtxPrivate(mtx);
    updateBoundingSphereRange();
}

void CollisionParts::updateBoundingSphereRange() {
    sead::Matrix34f mtx;
    mtx = *mSyncCollisionMtxPtr;
    updateBoundingSphereRangePrivate(makeEqualScale(&mtx));
}

void CollisionParts::forceResetAllMtxAndSetUpdateMtxOneTime(const sead::Matrix34f& matrix) {
    resetAllMtxPrivate(matrix);
    mIsUpdateMtxOneTime = true;
}

void CollisionParts::forceResetAllMtxAndSetUpdateMtxOneTime() {
    sead::Matrix34f mtx;
    mtx = *mSyncCollisionMtxPtr;
    makeEqualScale(&mtx);
    forceResetAllMtxAndSetUpdateMtxOneTime(mtx);
}

void CollisionParts::syncMtx(const sead::Matrix34f& matrix) {
    mSyncMtx = matrix;
    makeEqualScale(&mSyncMtx);
}

void CollisionParts::syncMtx() {
    syncMtx(*mSyncCollisionMtxPtr);
}

void CollisionParts::updateMtx() {
    bool needUpdate = isUpdate();
    bool isSame = isEqualMtx(mSyncMtx, mBaseMtx);

    if (!needUpdate) {
        if (isSame)
            mNoMoveTime++;
        return;
    }

    if (isSame) {
        mNoMoveTime++;
    } else {
        mNoMoveTime = mIsUpdateMtxOneTime ? 1 : 0;
        updateScale();
    }

    mIsUpdateMtxOneTime = false;
    if (mNoMoveTime > 1)
        return;

    mPrevBaseMtx = mBaseMtx;
    mBaseMtx = mSyncMtx;

    mPrevBaseInvMtx = mBaseInvMtx;
    mBaseInvMtx.setInverse(mBaseMtx);

    calcInvMtxScale();
}

void CollisionParts::updateScale() {
    f32 syncMtxScale = makeEqualScale(&mSyncMtx);
    mSyncMtxScaleVec.set(syncMtxScale, syncMtxScale, syncMtxScale);

    if (!isNearZero(syncMtxScale - mBaseMtxScale))
        updateBoundingSphereRangePrivate(syncMtxScale);
}

void CollisionParts::updateBoundingSphereRangePrivate(f32 scale) {
    mBaseMtxScale = scale;
    mBoundingSphereRange = scale * mServer->getFarthestVertexDistance();
}

bool CollisionParts::checkBoundingSphereRange(const sead::Vector3f& position, f32 radius) {
    sead::Vector3f distance = calcVecDiff(mBaseMtx.getTranslation(), position);
    f32 range = mBoundingSphereRange + radius;

    if (range * range < distance.squaredLength())
        return false;

    return true;
}

static KCollisionServer::HitInfoBuffer sHitInfoBuffer;

u32 CollisionParts::checkStrikePoint(HitInfo* outHit, const sead::Vector3f& position,
                                     const TriangleFilterBase* triFilter) const {
    sead::Vector3f modelPos;
    modelPos.setMul(mBaseInvMtx, position);

    sead::Vector3f scale;
    calcMtxScale(&scale, mBaseInvMtx);

    f32 avgScale = (scale.x + scale.y + scale.z) / 3.0f;

    KCFxyz kcPos(modelPos);
    f32 dist;
    auto* data = mServer->checkPoint(&kcPos, avgScale, &dist);

    if (data == nullptr)
        return 0;

    if (triFilter) {
        Triangle triangle;
        triangle.fillData(*this, data, nullptr);

        if (triFilter->isInvalidTriangle(triangle))
            return 0;
    }

    if (outHit) {
        outHit->triangle.fillData(*this, data, nullptr);
        outHit->distance = dist / avgScale;
        outHit->hitPos = position + outHit->triangle.getNormal(0) * outHit->distance;
    }

    return 1;
}

u32 CollisionParts::checkStrikeSphere(SphereHitResultBuffer* resultBuffer,
                                      const sead::Vector3f& position, f32 radius, bool checkMoving,
                                      const sead::Vector3f& moveDirection,
                                      const TriangleFilterBase* triFilter) const {
    sead::Vector3f modelPos;
    modelPos.setMul(mBaseInvMtx, position);
    f32 modelRadius = mMtxScale * radius;

    sead::Vector3f moveReaction(0.0f, 0.0f, 0.0f);

    if (!checkMoving || !isMoving()) {
        return checkStrikeSphereCore(resultBuffer, position, modelPos, {0.0f, 0.0f, 0.0f},
                                     modelRadius, triFilter);
    }

    sead::Vector3f prevModelPos;
    prevModelPos.setMul(mPrevBaseInvMtx, position);

    sead::Vector3f moveDir;
    moveDir.setRotated(mBaseMtx, modelPos - prevModelPos);
    moveDir += moveDirection;

    f32 stepSize = sead::Mathf::clampMax(modelRadius * 0.9f, 35.0f);

    SphereInterpolator interp;
    interp.startInterp(prevModelPos, modelPos, modelRadius, modelRadius, stepSize);

    TriFilterMoveReaction filter(&moveDir, triFilter);

    while (!interp.isEnd()) {
        sead::Vector3f currPos;
        sead::Vector3f currRemain;
        f32 currRadius;
        interp.calcInterp(&currPos, &currRadius, &currRemain);

        moveReaction.setRotated(mBaseMtx, -currRemain);
        filter.setMoveDirection(interp.isDone() ? nullptr : &moveDir);

        u32 strikeNum = checkStrikeSphereCore(resultBuffer, position, currPos, moveReaction,
                                              currRadius, &filter);
        if (strikeNum != 0)
            return strikeNum;

        interp.nextStep();
    }

    return 0;
}

u32 CollisionParts::checkStrikeSphereCore(SphereHitResultBuffer* resultBuffer,
                                          const sead::Vector3f& worldPos,
                                          const sead::Vector3f& modelPos,
                                          const sead::Vector3f& moveReaction, f32 modelRadius,
                                          const TriangleFilterBase* triFilter) const {
    u32 maxHitNum = resultBuffer->capacity() - resultBuffer->size();
    sHitInfoBuffer.clear();
    mServer->checkSphere(&modelPos, modelRadius, mMtxScale, maxHitNum, &sHitInfoBuffer);

    u32 hitNum = sHitInfoBuffer.size();
    u32 strikeNum = 0;

    for (u32 idx = 0; idx < hitNum; idx++) {
        auto* hit = sHitInfoBuffer.get(idx);

        sead::Vector3f hitPos;
        alKCollisionFunc::calcSphereHitPos(&hitPos, mServer, modelPos, *hit->data, hit->header,
                                           hit->location);

        auto* info = resultBuffer->emplaceBack();
        info->checkPos = worldPos;
        info->hitPos.setMul(mBaseMtx, hitPos);
        info->triangle.fillData(*this, hit->data, hit->header);

        if (triFilter && triFilter->isInvalidTriangle(info->triangle)) {
            resultBuffer->erase(resultBuffer->size() - 1);
            continue;
        }

        info->distance = hit->distance * mInvMtxScale;
        info->movingReaction = moveReaction;
        info->location = hit->location;
        parallelizeVec(&info->movingReaction, info->triangle.getFaceNormal(), info->movingReaction);

        strikeNum++;
    }

    return strikeNum;
}

u32 CollisionParts::checkStrikeArrow(ArrowHitResultBuffer* resultBuffer,
                                     const sead::Vector3f& position,
                                     const sead::Vector3f& direction,
                                     const TriangleFilterBase* triFilter) const {
    f32 length = direction.length();

    sead::Vector3f modelPos;
    modelPos.setMul(mBaseInvMtx, position);

    sead::Vector3f modelDir;
    modelDir.setMul(mBaseInvMtx, position + direction);
    modelDir -= modelPos;

    u32 maxHitNum = resultBuffer->capacity() - resultBuffer->size();
    sHitInfoBuffer.clear();
    u32 arrowHitNum = 0;
    mServer->checkArrow(modelPos, modelDir, &sHitInfoBuffer, &arrowHitNum, maxHitNum);

    u32 hitNum = sHitInfoBuffer.size();
    u32 strikeNum = 0;

    for (u32 idx = 0; idx < hitNum; idx++) {
        auto* info = resultBuffer->emplaceBack();
        auto* hit = sHitInfoBuffer.get(idx);

        sead::Vector3f modelPosHit;
        modelPosHit.setScaleAdd(hit->distance, modelDir, modelPos);

        sead::Vector3f hitPos;
        hitPos.setMul(mBaseMtx, modelPosHit);

        info->triangle.fillData(*this, hit->data, hit->header);

        if (triFilter && triFilter->isInvalidTriangle(info->triangle)) {
            resultBuffer->erase(resultBuffer->size() - 1);
            continue;
        }

        info->distance = length * hit->distance;
        info->hitPos = hitPos;
        info->location = hit->location;

        strikeNum++;
    }

    return strikeNum;
}

u32 CollisionParts::checkStrikeSphereForPlayer(SphereHitResultBuffer* resultBuffer,
                                               const sead::Vector3f& position, f32 radius,
                                               const TriangleFilterBase* triFilter) const {
    sead::Vector3f modelPos;
    modelPos.setMul(mBaseInvMtx, position);

    f32 modelRadius = mMtxScale * radius;

    sead::Vector3f moveReaction(0.0f, 0.0f, 0.0f);
    sead::Vector3f unused;

    if (!isMoving()) {
        return checkStrikeSphereForPlayerCore(resultBuffer, position, modelPos, moveReaction,
                                              unused, modelRadius, triFilter);
    }

    sead::Vector3f prevModelPos;
    prevModelPos.setMul(mPrevBaseInvMtx, position);

    sead::Vector3f moveDir;
    moveDir.setRotated(mBaseMtx, modelPos - prevModelPos);

    f32 stepSize = sead::Mathf::clampMax(modelRadius * 0.9f, 35.0f);

    SphereInterpolator interp;
    interp.startInterp(prevModelPos, modelPos, modelRadius, modelRadius, stepSize);

    TriFilterMoveReaction filter(&moveDir, triFilter);

    while (!interp.isEnd()) {
        sead::Vector3f currPos;
        sead::Vector3f currRemain;
        f32 currRadius;
        interp.calcInterp(&currPos, &currRadius, &currRemain);

        moveReaction.setRotated(mBaseMtx, -currRemain);
        filter.setMoveDirection(nullptr);

        u32 strikeNum = checkStrikeSphereForPlayerCore(resultBuffer, position, currPos,
                                                       moveReaction, unused, currRadius, &filter);
        if (strikeNum != 0)
            return strikeNum;

        interp.nextStep();
    }

    return 0;
}

u32 CollisionParts::checkStrikeSphereForPlayerCore(SphereHitResultBuffer* resultBuffer,
                                                   const sead::Vector3f& worldPos,
                                                   const sead::Vector3f& modelPos,
                                                   const sead::Vector3f& moveReaction,
                                                   const sead::Vector3f& unused, f32 modelRadius,
                                                   const TriangleFilterBase* triFilter) const {
    u32 maxHitNum = resultBuffer->capacity() - resultBuffer->size();
    sHitInfoBuffer.clear();
    mServer->checkSphereForPlayer(&modelPos, modelRadius, mMtxScale, maxHitNum, &sHitInfoBuffer);

    u32 hitNum = sHitInfoBuffer.size();
    u32 strikeNum = 0;

    for (u32 i = 0; i < hitNum; i++) {
        auto* hit = sHitInfoBuffer.get(i);

        sead::Vector3f hitPos;
        alKCollisionFunc::calcSphereHitPos(&hitPos, mServer, modelPos, *hit->data, hit->header,
                                           hit->location);

        auto* info = resultBuffer->emplaceBack();
        info->checkPos = worldPos;
        info->hitPos.setMul(mBaseMtx, hitPos);
        info->triangle.fillData(*this, hit->data, hit->header);

        if (triFilter && triFilter->isInvalidTriangle(info->triangle)) {
            resultBuffer->erase(resultBuffer->size() - 1);
            continue;
        }

        info->distance = hit->distance * mInvMtxScale;
        info->movingReaction = moveReaction;
        info->location = hit->location;
        parallelizeVec(&info->movingReaction, info->triangle.getFaceNormal(), info->movingReaction);

        strikeNum++;
    }

    return strikeNum;
}

u32 CollisionParts::checkStrikeDisk(DiskHitResultBuffer* resultBuffer,
                                    const sead::Vector3f& position, f32 radius, f32 halfHeight,
                                    const sead::Vector3f& normal,
                                    const TriangleFilterBase* triFilter) const {
    sead::Vector3f modelPos;
    modelPos.setMul(mBaseInvMtx, position);

    sead::Vector3f modelNormal;
    modelNormal.setRotated(mBaseInvMtx, normal);
    tryNormalizeOrZero(&modelNormal);

    f32 modelRadius = mMtxScale * radius;
    f32 modelHalfHeight = mMtxScale * halfHeight;
    sead::Vector3f moveReaction(0.0f, 0.0f, 0.0f);

    return checkStrikeDiskCore(resultBuffer, position, modelPos, moveReaction, modelRadius,
                               modelHalfHeight, modelNormal, triFilter);
}

u32 CollisionParts::checkStrikeDiskCore(DiskHitResultBuffer* resultBuffer,
                                        const sead::Vector3f& worldPos,
                                        const sead::Vector3f& modelPos,
                                        const sead::Vector3f& moveReaction, f32 modelRadius,
                                        f32 modelHalfHeight, const sead::Vector3f& modelNormal,
                                        const TriangleFilterBase* triFilter) const {
    u32 maxHitNum = resultBuffer->capacity() - resultBuffer->size();
    sHitInfoBuffer.clear();
    mServer->checkDisk(&modelPos, modelRadius, modelHalfHeight, modelNormal, mMtxScale, maxHitNum,
                       &sHitInfoBuffer);

    u32 hitNum = sHitInfoBuffer.size();
    u32 strikeNum = 0;

    for (u32 i = 0; i < hitNum; i++) {
        auto* hit = sHitInfoBuffer.get(i);

        sead::Vector3f hitPos;
        alKCollisionFunc::calcDiskHitPos(&hitPos, mServer, modelPos, modelHalfHeight, modelNormal,
                                         *hit->data, hit->header, hit->location);

        auto* info = resultBuffer->emplaceBack();
        info->checkPos = worldPos;
        info->hitPos.setMul(mBaseMtx, hitPos);
        info->triangle.fillData(*this, hit->data, hit->header);

        if (triFilter && triFilter->isInvalidTriangle(info->triangle)) {
            resultBuffer->erase(resultBuffer->size() - 1);
            continue;
        }

        info->distance = hit->distance * mInvMtxScale;
        info->movingReaction = moveReaction;
        info->location = hit->location;
        parallelizeVec(&info->movingReaction, info->triangle.getFaceNormal(), info->movingReaction);

        strikeNum++;
    }

    return strikeNum;
}

void CollisionParts::calcForceMovePower(sead::Vector3f* outMovePower,
                                        const sead::Vector3f& position) const {
    sead::Vector3f prevModelPos;
    prevModelPos.setMul(mPrevBaseInvMtx, position);
    sead::Vector3f currWorldPos;
    currWorldPos.setMul(mBaseMtx, prevModelPos);

    outMovePower->setSub(currWorldPos, position);
}

void CollisionParts::calcForceRotatePower(sead::Quatf* outRotatePower) const {
    sead::Quatf prevModelRotate;
    mPrevBaseInvMtx.toQuat(prevModelRotate);
    sead::Quatf currWorldRotate;
    mBaseMtx.toQuat(currWorldRotate);

    outRotatePower->setMul(prevModelRotate, currWorldRotate);
}

}  // namespace al

namespace {
using namespace al;

bool TriFilterMoveReaction::isInvalidTriangle(const Triangle& triangle) const {
    if (mFilter && mFilter->isInvalidTriangle(triangle))
        return true;

    if (mMoveDir && mMoveDir->dot(triangle.getFaceNormal()) > 0.0f)
        return true;

    return false;
}
}  // namespace
