#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionPartsKeeper.h"
#include "Library/Collision/KTriangle.h"
#include "Library/Collision/CollisionCheckInfo.h"
#include "Library/Execute/ExecuteTableHolderUpdate.h"
#include "container/seadObjArray.h"

#include <cstdio>
#include <new>

#define DEREF_NULL *(volatile int*)0;
#define WARN_UNIMPL printf("Function not implemented: %s (%s:%d)\n", __func__, __FILE__, __LINE__)
#define CRASH {WARN_UNIMPL;DEREF_NULL}

namespace al {

// NON_MATCHING: mismatch in allocBuffer
CollisionDirector::CollisionDirector(ExecuteDirector* executeDirector) : mCollisionPartsKeeperPtrArray(new CollisionPartsKeeperPtrArray()) {
    mStrikeArrowHitInfos = new ArrowHitResultBuffer();
    mStrikeArrowHitInfos->allocBuffer(512, nullptr);
    
    mStrikeSphereHitInfos = new SphereHitResultBuffer();
    mStrikeSphereHitInfos->allocBuffer(512, nullptr);

    mStrikeDiskHitInfos = new DiskHitResultBuffer();
    mStrikeDiskHitInfos->allocBuffer(512, nullptr);

    mSphereHitArray = new SphereHitInfo[64];
    mDiskHitArray = new DiskHitInfo[64];

    al::registerExecutorUser(this, executeDirector, "コリジョンディレクター");

    setPartsKeeper(new CollisionPartsKeeperOctree(256, 8, 1000.0f));
}
void CollisionDirector::setPartsKeeper(ICollisionPartsKeeper* partsKeeper) {
    mPartsKeeper = partsKeeper;
    if(mActivePartsKeeper != mCollisionPartsKeeperPtrArray)
        mActivePartsKeeper = mPartsKeeper;
}
void CollisionDirector::endInit() {
    mActivePartsKeeper->endInit();
}
void CollisionDirector::setPartsFilter(const CollisionPartsFilterBase* filter) {
    mCollisionPartsFilterBase = filter;
}
void CollisionDirector::setTriFilter(const TriangleFilterBase* filter) {
    mTriangleFilterBase = filter;
}
// NON_MATCHING: additional "AND 1" at the end
bool CollisionDirector::checkStrikePoint(const sead::Vector3f& point, HitInfo* info) {
    auto checkInfo = CollisionCheckInfoBase {point, mCollisionPartsFilterBase, mTriangleFilterBase };
    u32 result = mActivePartsKeeper->checkStrikePoint(info, checkInfo);
    mCollisionPartsFilterBase = nullptr;
    mTriangleFilterBase = nullptr;
    return result;
}
// NON_MATCHING: additional "AND 1" at the end
bool CollisionDirector::checkStrikeSphere(const sead::Vector3f& pos, f32 radius, bool unk3, const sead::Vector3f& unk4) {
    mStrikeSphereHitInfos->clear();

    auto checkInfo = SphereCheckInfo {pos, radius};
    checkInfo.mCollisionPartsFilter = mCollisionPartsFilterBase;
    checkInfo.mTriFilterBase = mTriangleFilterBase;

    auto* infos = mStrikeSphereHitInfos;
    bool result = mActivePartsKeeper->checkStrikeSphere(infos, checkInfo, unk3, unk4);

    mCollisionPartsFilterBase = nullptr;
    mTriangleFilterBase = nullptr;
    return result;
}
// NON_MATCHING: Incremental BoundBox for ArrowCheckInfo, additional "AND 1" at the end
bool CollisionDirector::checkStrikeArrow(const sead::Vector3f& pos, const sead::Vector3f& unk2) {
    mStrikeArrowHitInfos->clear();

    auto checkInfo = ArrowCheckInfo {pos, unk2};
    checkInfo.mCollisionPartsFilter = mCollisionPartsFilterBase;
    checkInfo.mTriFilterBase = mTriangleFilterBase;

    auto* infos = mStrikeArrowHitInfos;
    bool result = mActivePartsKeeper->checkStrikeArrow(infos, checkInfo);

    mCollisionPartsFilterBase = nullptr;
    mTriangleFilterBase = nullptr;
    return result;
}
// NON_MATCHING: additional "AND 1" at the end
bool CollisionDirector::checkStrikeSphereForPlayer(const sead::Vector3f& pos, f32 radius) {
    mStrikeSphereHitInfos->clear();

    auto checkInfo = SphereCheckInfo {pos, radius};
    checkInfo.mCollisionPartsFilter = mCollisionPartsFilterBase;
    checkInfo.mTriFilterBase = mTriangleFilterBase;

    auto* infos = mStrikeSphereHitInfos;
    bool result = mActivePartsKeeper->checkStrikeSphereForPlayer(infos, checkInfo);

    mCollisionPartsFilterBase = nullptr;
    mTriangleFilterBase = nullptr;
    return result;
}
// NON_MATCHING: inlined constructor of DiskCheckInfo instead of explicit call
bool CollisionDirector::checkStrikeDisk(const sead::Vector3f& unk1, f32 unk2, f32 unk3, const sead::Vector3f& unk4) {
    mStrikeDiskHitInfos->clear();

    auto checkInfo = DiskCheckInfo {unk1, unk2, unk3, unk4};
    checkInfo.mCollisionPartsFilter = mCollisionPartsFilterBase;
    checkInfo.mTriFilterBase = mTriangleFilterBase;

    auto* infos = mStrikeDiskHitInfos;
    bool result = mActivePartsKeeper->checkStrikeDisk(infos, checkInfo);

    mCollisionPartsFilterBase = nullptr;
    mTriangleFilterBase = nullptr;
    return result;
}
ArrowHitInfo* CollisionDirector::getStrikeArrowInfo(u32 index) {
    return mStrikeArrowHitInfos->unsafeAt(index);
}
u32 CollisionDirector::getStrikeArrowInfoNum() const {
    return mStrikeArrowHitInfos->size();
}
SphereHitInfo* CollisionDirector::getStrikeSphereInfo(u32 index) {
    return mStrikeSphereHitInfos->unsafeAt(index);
}
u32 CollisionDirector::getStrikeSphereInfoNum() const {
    return mStrikeSphereHitInfos->size();
}
DiskHitInfo* CollisionDirector::getStrikeDiskInfo(u32 index) {
    return mStrikeDiskHitInfos->unsafeAt(index);
}
u32 CollisionDirector::getStrikeDiskInfoNum() const {
    return mStrikeDiskHitInfos->size();
}
void CollisionDirector::getSphereHitInfoArrayForCollider(SphereHitInfo** infoArray, u32* count) {
    *infoArray = mSphereHitArray;
    *count = 64;
}
void CollisionDirector::getDiskHitInfoArrayForCollider(DiskHitInfo** infoArray, u32* count) {
    *infoArray = mDiskHitArray;
    *count = 64;
}
void CollisionDirector::execute() {
    if(mActivePartsKeeper)
        mActivePartsKeeper->movement();
}
void CollisionDirector::searchCollisionPartsWithSphere(const sead::Vector3f& pos, f32 radius,
                                    sead::IDelegate1<CollisionParts*>& delegate,
                                    const CollisionPartsFilterBase* filter) const {
    auto checkInfo = SphereCheckInfo {pos, radius};
    checkInfo.mCollisionPartsFilter = filter;

    if(mPartsKeeper)
        mPartsKeeper->searchWithSphere(checkInfo, delegate);
}
void CollisionDirector::validateCollisionPartsPtrArray(sead::PtrArray<CollisionParts>* arr) {
    mCollisionPartsKeeperPtrArray->setPtrArray(arr);
    mActivePartsKeeper = mCollisionPartsKeeperPtrArray;
}
void CollisionDirector::invalidateCollisionPartsPtrArray() {
    mActivePartsKeeper = mPartsKeeper;
}
sead::PtrArray<CollisionParts>* CollisionDirector::getCollisionPartsPtrArray() const {
    if(mActivePartsKeeper == mPartsKeeper)
        return nullptr;
    return mCollisionPartsKeeperPtrArray->getPtrArray();
}

}
