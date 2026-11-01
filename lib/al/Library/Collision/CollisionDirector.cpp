#include "Library/Collision/CollisionDirector.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsCheck.h"
#include "Library/Collision/CollisionPartsKeeperOctree.h"
#include "Library/Collision/CollisionPartsKeeperPtrArray.h"
#include "Library/Collision/ICollisionPartsKeeper.h"
#include "Library/CollisionFilter/PartsFilter.h"
#include "Library/Execute/ExecuteUtil.h"

namespace al {

static constexpr u32 cStrikeHitInfoCapacity = 512;
static constexpr u32 cHitInfoForColliderCapacity = 64;

CollisionDirector::CollisionDirector(ExecuteDirector* executeDirector)
    : mPtrArrayPartsKeeper(new CollisionPartsKeeperPtrArray()) {
    mStrikeArrowHitInfoArray = new ArrowHitResultBuffer();
    mStrikeArrowHitInfoArray->allocBuffer(cStrikeHitInfoCapacity, nullptr);

    mStrikeSphereHitInfoArray = new SphereHitResultBuffer();
    mStrikeSphereHitInfoArray->allocBuffer(cStrikeHitInfoCapacity, nullptr);

    mStrikeDiskHitInfoArray = new DiskHitResultBuffer();
    mStrikeDiskHitInfoArray->allocBuffer(cStrikeHitInfoCapacity, nullptr);

    mSphereHitInfoArrayForCollider = new SphereHitInfo[cHitInfoForColliderCapacity];
    mDiskHitInfoArrayForCollider = new DiskHitInfo[cHitInfoForColliderCapacity];

    registerExecutorUser(this, executeDirector, "コリジョンディレクター");

    setPartsKeeper(new CollisionPartsKeeperOctree(256, 8, 1000.0f));
}

void CollisionDirector::setPartsKeeper(ICollisionPartsKeeper* partsKeeper) {
    mRootPartsKeeper = partsKeeper;

    if (mActivePartsKeeper != mPtrArrayPartsKeeper)
        mActivePartsKeeper = partsKeeper;
}

void CollisionDirector::endInit() {
    mActivePartsKeeper->endInit();
}

void CollisionDirector::setPartsFilter(const CollisionPartsFilterBase* partsFilter) {
    mPartsFilter = partsFilter;
}

void CollisionDirector::setTriFilter(const TriangleFilterBase* triFilter) {
    mTriFilter = triFilter;
}

u32 CollisionDirector::checkStrikePoint(const sead::Vector3f& position, HitInfo* outHit) {
    CollisionCheckInfoBase checkInfo(position);
    checkInfo.partsFilter = mPartsFilter;
    checkInfo.triFilter = mTriFilter;

    u32 strikeNum = mActivePartsKeeper->checkStrikePoint(outHit, checkInfo);

    mPartsFilter = nullptr;
    mTriFilter = nullptr;

    return strikeNum;
}

u32 CollisionDirector::checkStrikeSphere(const sead::Vector3f& position, f32 radius,
                                         bool checkMoving, const sead::Vector3f& moveDirection) {
    mStrikeSphereHitInfoArray->clear();

    SphereCheckInfo checkInfo(position, radius);
    checkInfo.partsFilter = mPartsFilter;
    checkInfo.triFilter = mTriFilter;

    auto resultBuffer = mStrikeSphereHitInfoArray;
    u32 strikeNum =
        mActivePartsKeeper->checkStrikeSphere(resultBuffer, checkInfo, checkMoving, moveDirection);

    mPartsFilter = nullptr;
    mTriFilter = nullptr;

    return strikeNum;
}

u32 CollisionDirector::checkStrikeArrow(const sead::Vector3f& position,
                                        const sead::Vector3f& direction) {
    mStrikeArrowHitInfoArray->clear();

    ArrowCheckInfo checkInfo(position, direction);
    checkInfo.partsFilter = mPartsFilter;
    checkInfo.triFilter = mTriFilter;

    u32 strikeNum = mActivePartsKeeper->checkStrikeArrow(mStrikeArrowHitInfoArray, checkInfo);

    mPartsFilter = nullptr;
    mTriFilter = nullptr;

    return strikeNum;
}

u32 CollisionDirector::checkStrikeSphereForPlayer(const sead::Vector3f& position, f32 radius) {
    mStrikeSphereHitInfoArray->clear();

    SphereCheckInfo checkInfo(position, radius);
    checkInfo.partsFilter = mPartsFilter;
    checkInfo.triFilter = mTriFilter;

    auto resultBuffer = mStrikeSphereHitInfoArray;
    u32 result = mActivePartsKeeper->checkStrikeSphereForPlayer(resultBuffer, checkInfo);

    mPartsFilter = nullptr;
    mTriFilter = nullptr;

    return result;
}

u32 CollisionDirector::checkStrikeDisk(const sead::Vector3f& position, f32 radius, f32 halfHeight,
                                       const sead::Vector3f& normal) {
    mStrikeDiskHitInfoArray->clear();
    auto resultBuffer = mStrikeDiskHitInfoArray;

    DiskCheckInfo checkInfo(position, radius, halfHeight, normal);
    checkInfo.partsFilter = mPartsFilter;
    checkInfo.triFilter = mTriFilter;

    u32 result = mActivePartsKeeper->checkStrikeDisk(resultBuffer, checkInfo);

    mPartsFilter = nullptr;
    mTriFilter = nullptr;

    return result;
}

ArrowHitInfo* CollisionDirector::getStrikeArrowInfo(u32 index) {
    return mStrikeArrowHitInfoArray->unsafeAt(index);
}

u32 CollisionDirector::getStrikeArrowInfoNum() const {
    return mStrikeArrowHitInfoArray->size();
}

SphereHitInfo* CollisionDirector::getStrikeSphereInfo(u32 index) {
    return mStrikeSphereHitInfoArray->unsafeAt(index);
}

u32 CollisionDirector::getStrikeSphereInfoNum() const {
    return mStrikeSphereHitInfoArray->size();
}

DiskHitInfo* CollisionDirector::getStrikeDiskInfo(u32 index) {
    return mStrikeDiskHitInfoArray->unsafeAt(index);
}

u32 CollisionDirector::getStrikeDiskInfoNum() const {
    return mStrikeDiskHitInfoArray->size();
}

void CollisionDirector::getSphereHitInfoArrayForCollider(SphereHitInfo** outArray, u32* outSize) {
    *outArray = mSphereHitInfoArrayForCollider;
    *outSize = cHitInfoForColliderCapacity;
}

void CollisionDirector::getDiskHitInfoArrayForCollider(DiskHitInfo** outArray, u32* outSize) {
    *outArray = mDiskHitInfoArrayForCollider;
    *outSize = cHitInfoForColliderCapacity;
}

void CollisionDirector::execute() {
    if (mActivePartsKeeper != nullptr)
        mActivePartsKeeper->movement();
}

void CollisionDirector::searchCollisionPartsWithSphere(
    const sead::Vector3f& position, f32 radius,
    ICollisionPartsKeeper::CollisionPartsCallback& callback,
    const CollisionPartsFilterBase* partsFilter) const {
    SphereCheckInfo checkInfo(position, radius);
    checkInfo.partsFilter = partsFilter;

    if (mRootPartsKeeper)
        mRootPartsKeeper->searchWithSphere(checkInfo, callback);
}

void CollisionDirector::validateCollisionPartsPtrArray(
    sead::PtrArray<CollisionParts>* partsPtrArray) {
    mPtrArrayPartsKeeper->setPartsPtrArray(partsPtrArray);
    mActivePartsKeeper = mPtrArrayPartsKeeper;
}

void CollisionDirector::invalidateCollisionPartsPtrArray() {
    mActivePartsKeeper = mRootPartsKeeper;
}

sead::PtrArray<CollisionParts>* CollisionDirector::getCollisionPartsPtrArray() const {
    if (mActivePartsKeeper == mRootPartsKeeper)
        return nullptr;

    return mPtrArrayPartsKeeper->getPartsPtrArray();
}

}  // namespace al
