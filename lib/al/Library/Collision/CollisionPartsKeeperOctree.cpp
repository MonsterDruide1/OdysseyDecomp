#include "Library/Collision/CollisionPartsKeeperOctree.h"

#include <container/seadObjList.h>
#include <math/seadBoundBox.h>
#include <math/seadVector.h>

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsCheck.h"
#include "Library/Collision/CollisionUtil.h"
#include "Library/Math/MathUtil.h"

namespace {

// TODO: These probably belong into sead::BoundBox3f itself
static void setFromCenterAndXYZ(sead::BoundBox3f* bounds, const sead::Vector3f& center, f32 sizeX,
                                f32 sizeY, f32 sizeZ) {
    auto* pMin = const_cast<sead::Vector3f*>(&bounds->getMin());
    auto* pMax = const_cast<sead::Vector3f*>(&bounds->getMax());

    sead::Vector3f halfSize(sizeX * 0.5f, sizeY * 0.5f, sizeZ * 0.5f);
    sead::Vector3f min = center - halfSize;
    sead::Vector3f max = halfSize + center;

    pMin->set(min);
    pMax->set(max);
}

static void expandFromCenterAndXYZ(sead::BoundBox3f* bounds, const sead::Vector3f& center,
                                   f32 sizeX, f32 sizeY, f32 sizeZ) {
    auto* pMin = const_cast<sead::Vector3f*>(&bounds->getMin());
    auto* pMax = const_cast<sead::Vector3f*>(&bounds->getMax());

    sead::Vector3f halfSize(sizeX * 0.5f, sizeY * 0.5f, sizeZ * 0.5f);
    sead::Vector3f min = center - halfSize;
    sead::Vector3f max = halfSize + center;

    if (pMin->x > min.x)
        pMin->x = min.x;
    if (pMin->y > min.y)
        pMin->y = min.y;
    if (pMin->z > min.z)
        pMin->z = min.z;

    if (pMax->x < max.x)
        pMax->x = max.x;
    if (pMax->y < max.y)
        pMax->y = max.y;
    if (pMax->z < max.z)
        pMax->z = max.z;
}

static bool isOverlapRange(f32 minA, f32 maxA, f32 minB, f32 maxB) {
    return minA <= maxB && minB <= maxA;
}

static bool isOverlapBox(const sead::BoundBox3f& boxA, const sead::BoundBox3f& boxB) {
    return isOverlapRange(boxA.getMin().x, boxA.getMax().x, boxB.getMin().x, boxB.getMax().x) &&
           isOverlapRange(boxA.getMin().y, boxA.getMax().y, boxB.getMin().y, boxB.getMax().y) &&
           isOverlapRange(boxA.getMin().z, boxA.getMax().z, boxB.getMin().z, boxB.getMax().z);
}

}  // namespace

namespace al {

// NON_MATCHING: Logic should be correct, but ASM is totally different
// (https://decomp.me/scratch/SWHPu)
CollisionPartsKeeperOctree::CollisionPartsKeeperOctree(s32 nodeNumMax, s32 maxDepth,
                                                       f32 minCubeSize)
    : mMaxDepth(maxDepth), mMinCubeSize(minCubeSize) {
    mNodeList.allocBuffer(nodeNumMax, nullptr);
}

void CollisionPartsKeeperOctree::endInit() {
    f32 maxSize = sead::Mathf::max(mBoundBox.getSizeX(),
                                   sead::Mathf::max(mBoundBox.getSizeY(), mBoundBox.getSizeZ()));

    mRootCubeSize = maxSize;

    setFromCenterAndXYZ(&mRootNode.box, mBoundBox.getCenter(), maxSize, maxSize, maxSize);
    mRootNode.cubeSize = maxSize;
    mRootNode.depth = 0;

    insertMovingPartsListToOctNode();
    mIsInitialized = true;
}

void CollisionPartsKeeperOctree::insertMovingPartsListToOctNode() {
    for (auto it = mPartsList.robustBegin(); it != mPartsList.robustEnd(); ++it) {
        CollisionParts* parts = it->mData;

        if (parts->isValid())
            insertLooseOctree(&mRootNode, parts);
        else
            parts->erase();
    }
}

void CollisionPartsKeeperOctree::addCollisionParts(CollisionParts* parts) {
    f32 range = parts->getBoundingSphereRange();
    expandFromCenterAndXYZ(&mBoundBox, parts->getTrans(), range * 2.0f, range * 2.0f, range * 2.0f);

    pushBackCollisionParts(&mPartsList, parts);
}

void CollisionPartsKeeperOctree::connectToCollisionPartsList(CollisionParts* parts) {
    if (mIsInitialized)
        insertLooseOctree(&mRootNode, parts);
}

CollisionPartsKeeperOctree::OctNode*
CollisionPartsKeeperOctree::insertLooseOctree(OctNode* node, CollisionParts* parts) {
    auto* currNode = node;
    while (isEnableSearchNextDepthNode(currNode)) {
        f32 childSizeHalf = calcNodeCubeSizeHalf(currNode->depth + 1);

        sead::Vector3f trans = parts->getTrans();
        sead::Vector3f center = currNode->box.getCenter();

        bool isLowerX = trans.x <= center.x;
        bool isLowerY = trans.y <= center.y;
        bool isLowerZ = trans.z <= center.z;
        u32 index = (!isLowerX << 0) | (!isLowerY << 1) | (!isLowerZ << 2);

        OctNode* child = currNode->children[index];
        sead::Vector3f childCenter;

        if (child) {
            child->box.getCenter(&childCenter);
        } else {
            f32 offset = mRootCubeSize / (2 << currNode->depth);
            childCenter.x = center.x + (!isLowerX ? offset * 0.5f : offset * -0.5f);
            childCenter.y = center.y + (!isLowerY ? offset * 0.5f : offset * -0.5f);
            childCenter.z = center.z + (!isLowerZ ? offset * 0.5f : offset * -0.5f);
        }

        f32 childSize = childSizeHalf * 2.0f;
        sead::BoundBox3f childBox;
        setFromCenterAndXYZ(&childBox, childCenter, childSize, childSize, childSize);

        if (!isFitsInBox(parts, childBox))
            break;

        if (!child) {
            child = mNodeList.emplaceBack();

            if (!child)
                break;

            child->parent = currNode;
            child->box = childBox;
            child->cubeSize = childSize;
            child->depth = currNode->depth + 1;
            currNode->children[index] = child;
        }

        currNode = child;
    }

    pushBackCollisionParts(&currNode->partsList, parts);
    return currNode;
}

void CollisionPartsKeeperOctree::disconnectToCollisionPartsList(CollisionParts* parts) {
    if (mIsInitialized)
        parts->erase();
}

void CollisionPartsKeeperOctree::resetToCollisionPartsList(CollisionParts* parts) {
    if (mIsInitialized && parts->mList != nullptr)
        pushBackCollisionParts(&mPartsList, parts);
}

u32 CollisionPartsKeeperOctree::checkStrikePoint(HitInfo* outHit,
                                                 const CollisionCheckInfoBase& checkInfo) const {
    return checkStrikePointCore(outHit, mRootNode.partsList, checkInfo);
}

u32 CollisionPartsKeeperOctree::checkStrikeSphere(SphereHitResultBuffer* resultBuffer,
                                                  const SphereCheckInfo& checkInfo,
                                                  bool checkMoving,
                                                  const sead::Vector3f& moveDirection) const {
    return checkStrikeSphereRecursive(resultBuffer, checkInfo, &mRootNode, checkMoving,
                                      moveDirection, checkStrikeSphereCore);
}

u32 CollisionPartsKeeperOctree::checkStrikeSphereRecursive(SphereHitResultBuffer* resultBuffer,
                                                           const SphereCheckInfo& checkInfo,
                                                           const OctNode* node, bool checkMoving,
                                                           const sead::Vector3f& moveDirection,
                                                           SphereCheckDelegate checkFunc) const {
    u32 strikeNum = checkFunc(resultBuffer, node->partsList, checkInfo, checkMoving, moveDirection);

    for (s32 idx = 0; idx < node->children.size(); idx++) {
        auto* child = node->children[idx];

        if (child && isNearCollideSphereAabb(checkInfo.position, checkInfo.radius, child->box)) {
            strikeNum += checkStrikeSphereRecursive(resultBuffer, checkInfo, child, checkMoving,
                                                    moveDirection, checkFunc);
        }
    }

    return strikeNum;
}

u32 CollisionPartsKeeperOctree::checkStrikeSphereForPlayer(SphereHitResultBuffer* resultBuffer,
                                                           const SphereCheckInfo& checkInfo) const {
    return checkStrikeSphereRecursive(resultBuffer, checkInfo, &mRootNode, false,
                                      sead::Vector3f::zero, checkStrikeSphereForPlayerCore);
}

u32 CollisionPartsKeeperOctree::checkStrikeDisk(DiskHitResultBuffer* resultBuffer,
                                                const DiskCheckInfo& checkInfo) const {
    return checkStrikeDiskRecursive(resultBuffer, checkInfo, &mRootNode);
}

u32 CollisionPartsKeeperOctree::checkStrikeDiskRecursive(DiskHitResultBuffer* resultBuffer,
                                                         const DiskCheckInfo& checkInfo,
                                                         const OctNode* node) const {
    u32 strikeNum = checkStrikeDiskCore(resultBuffer, node->partsList, checkInfo);

    for (s32 idx = 0; idx < node->children.size(); idx++) {
        auto* child = node->children[idx];

        if (child &&
            isNearCollideSphereAabb(checkInfo.position, checkInfo.boundingRange, child->box)) {
            strikeNum += checkStrikeDiskRecursive(resultBuffer, checkInfo, child);
        }
    }

    return strikeNum;
}

void CollisionPartsKeeperOctree::searchWithSphere(const SphereCheckInfo& checkInfo,
                                                  CollisionPartsCallback& callback) const {
    searchWithSphereRecursive(checkInfo, callback, &mRootNode);
}

void CollisionPartsKeeperOctree::searchWithSphereRecursive(const SphereCheckInfo& checkInfo,
                                                           CollisionPartsCallback& callback,
                                                           const OctNode* node) const {
    for (auto it = node->partsList.begin(); it != node->partsList.end(); ++it) {
        CollisionParts* parts = *it;

        if (alCollisionUtil::isInvalidParts(*parts, checkInfo))
            continue;

        if (alCollisionUtil::isFarAway(*parts, checkInfo.position, checkInfo.radius))
            continue;

        callback.invoke(*it);
    }

    for (s32 idx = 0; idx < node->children.size(); idx++) {
        auto* child = node->children[idx];

        if (child && isNearCollideSphereAabb(checkInfo.position, checkInfo.radius, child->box))
            searchWithSphereRecursive(checkInfo, callback, child);
    }
}

u32 CollisionPartsKeeperOctree::checkStrikeArrow(ArrowHitResultBuffer* resultBuffer,
                                                 const ArrowCheckInfo& checkInfo) const {
    return checkStrikeArrowRecursive(resultBuffer, checkInfo, &mRootNode);
}

u32 CollisionPartsKeeperOctree::checkStrikeArrowRecursive(ArrowHitResultBuffer* resultBuffer,
                                                          const ArrowCheckInfo& checkInfo,
                                                          const OctNode* node) const {
    u32 strikeNum = checkStrikeArrowCore(resultBuffer, node->partsList, checkInfo);

    for (s32 idx = 0; idx < node->children.size(); idx++) {
        auto* child = node->children[idx];

        if (child != nullptr && isOverlapBox(checkInfo.bounds, child->box))
            strikeNum += checkStrikeArrowRecursive(resultBuffer, checkInfo, child);
    }

    return strikeNum;
}

void CollisionPartsKeeperOctree::movement() {
    updateOctNodeCollisionPartsRecursive(&mRootNode);
    insertMovingPartsListToOctNode();
}

void CollisionPartsKeeperOctree::updateOctNodeCollisionPartsRecursive(OctNode* node) {
    for (auto it = node->partsList.robustBegin(); it != node->partsList.robustEnd(); ++it) {
        CollisionParts* parts = it->mData;

        updateCollisionParts(parts);

        if (parts->isMoving() && !isFitsInBox(parts, node->box))
            pushBackCollisionParts(&mPartsList, parts);
    }

    for (s32 i = 0; i < node->children.size(); i++) {
        auto* child = node->children[i];

        if (child)
            updateOctNodeCollisionPartsRecursive(child);
    }
}

bool CollisionPartsKeeperOctree::isFitsInBox(const CollisionParts* parts,
                                             const sead::BoundBox3f& box) const {
    f32 range = parts->getBoundingSphereRange();
    return isSphereFitsInBox(parts->getTrans(), range, box);
}

f32 CollisionPartsKeeperOctree::calcNodeCubeSizeHalf(s32 depth) const {
    return (mRootCubeSize * 2) / (2 << depth);
}

bool CollisionPartsKeeperOctree::isEnableSearchNextDepthNode(const OctNode* node) {
    return node->depth + 1 < mMaxDepth && !(node->cubeSize < (mMinCubeSize * 2));
}

bool CollisionPartsKeeperOctree::isSphereFitsInBox(const sead::Vector3f& position, f32 radius,
                                                   const sead::BoundBox3f& box) const {
    if (position.x - radius < box.getMin().x || box.getMax().x < position.x + radius)
        return false;
    if (position.y - radius < box.getMin().y || box.getMax().y < position.y + radius)
        return false;
    if (position.z - radius < box.getMin().z || box.getMax().z < position.z + radius)
        return false;

    return true;
}

}  // namespace al
