#pragma once

#include <container/seadObjList.h>
#include <container/seadSafeArray.h>
#include <container/seadTList.h>
#include <math/seadBoundBox.h>
#include <math/seadVector.h>
#include <prim/seadDelegate.h>

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/ICollisionPartsKeeper.h"

namespace al {

class CollisionPartsKeeperOctree : public ICollisionPartsKeeper {
public:
    CollisionPartsKeeperOctree(s32 maxNodes, s32 maxDepth, f32 minCubeSize);

    void endInit() override;
    void insertMovingPartsListToOctNode();

    void addCollisionParts(CollisionParts* parts) override;
    void connectToCollisionPartsList(CollisionParts* parts) override;

    void disconnectToCollisionPartsList(CollisionParts* parts) override;
    void resetToCollisionPartsList(CollisionParts* parts) override;

    u32 checkStrikePoint(HitInfo* hitInfo, const CollisionCheckInfoBase& checkInfo) const override;
    u32 checkStrikeSphere(SphereHitResultBuffer* resultBuffer, const SphereCheckInfo& checkInfo,
                          bool checkMoving, const sead::Vector3f& moveDirecrion) const override;
    u32 checkStrikeArrow(ArrowHitResultBuffer* resultBuffer,
                         const ArrowCheckInfo& checkInfo) const override;
    u32 checkStrikeSphereForPlayer(SphereHitResultBuffer* resultBuffer,
                                   const SphereCheckInfo& checkInfo) const override;
    u32 checkStrikeDisk(DiskHitResultBuffer* resultBuffer,
                        const DiskCheckInfo& checkInfo) const override;

    void searchWithSphere(const SphereCheckInfo& checkInfo,
                          CollisionPartsCallback& callback) const override;

    void movement() override;

private:
    struct OctNode {
        OctNode() { children.fill(nullptr); }

        OctNode* parent = nullptr;
        sead::UnsafeArray<OctNode*, 8> children;

        sead::BoundBox3f box;
        f32 cubeSize = 0.0f;
        s32 depth = -1;

        CollisionPartsList partsList;
    };

    static_assert(sizeof(OctNode) == 0x80);

    OctNode* insertLooseOctree(OctNode* node, CollisionParts* parts);

    bool isFitsInBox(const CollisionParts* parts, const sead::BoundBox3f& box) const;
    f32 calcNodeCubeSizeHalf(s32 depth) const;
    bool isEnableSearchNextDepthNode(const OctNode* node);
    bool isSphereFitsInBox(const sead::Vector3f& position, f32 radius,
                           const sead::BoundBox3f& box) const;

    using SphereCheckDelegate = u32(SphereHitResultBuffer* resultBuffer,
                                    const CollisionPartsList& partsList,
                                    const SphereCheckInfo& checkInfo, bool checkMoving,
                                    const sead::Vector3f& moveDirecrion);

    u32 checkStrikeSphereRecursive(SphereHitResultBuffer* resultBuffer,
                                   const SphereCheckInfo& checkInfo, const OctNode* node,
                                   bool checkMoving, const sead::Vector3f& moveDirection,
                                   SphereCheckDelegate* checkFunc) const;
    u32 checkStrikeArrowRecursive(ArrowHitResultBuffer* resultBuffer,
                                  const ArrowCheckInfo& checkInfo, const OctNode* node) const;
    u32 checkStrikeDiskRecursive(DiskHitResultBuffer* resultBuffer, const DiskCheckInfo& checkInfo,
                                 const OctNode* node) const;

    void searchWithSphereRecursive(const SphereCheckInfo& checkInfo,
                                   CollisionPartsCallback& callback, const OctNode* node) const;

    void updateOctNodeCollisionPartsRecursive(OctNode* node);

    sead::BoundBox3f mBoundBox;
    f32 mRootCubeSize = 0.0f;
    OctNode mRootNode;

    sead::ObjList<OctNode> mNodeList;
    CollisionPartsList mPartsList;

    s32 mMaxDepth;
    f32 mMinCubeSize;
    bool mIsInitialized = false;
};

static_assert(sizeof(CollisionPartsKeeperOctree) == 0x100);

}  // namespace al
