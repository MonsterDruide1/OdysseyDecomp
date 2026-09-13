#pragma once

#include <math/seadMatrix.h>

#include "Library/Collision/ICollisionPartsKeeper.h"

namespace al {
struct ActorInitInfo;
class DynamicCollisionActor;
class DynamicDrawActor;
class HitSensor;
}  // namespace al

class DynamicCollisionKeeper : public al::ICollisionPartsKeeper {
public:
    DynamicCollisionKeeper();

    void endInit() override;
    void addCollisionParts(al::CollisionParts* parts) override;
    void connectToCollisionPartsList(al::CollisionParts* parts) override;
    void disconnectToCollisionPartsList(al::CollisionParts* parts) override;
    void resetToCollisionPartsList(al::CollisionParts* parts) override = 0;
    s32 checkStrikePoint(al::HitInfo* hitInfo,
                         const al::CollisionCheckInfoBase& checkInfo) const override;
    s32 checkStrikeSphere(al::SphereHitResultBuffer* resultBuffer,
                          const al::SphereCheckInfo& checkInfo, bool isFollowMovingCollision,
                          const sead::Vector3f& movement) const override;
    s32 checkStrikeArrow(al::ArrowHitResultBuffer* resultBuffer,
                         const al::ArrowCheckInfo& checkInfo) const override;
    s32 checkStrikeSphereForPlayer(al::SphereHitResultBuffer* resultBuffer,
                                   const al::SphereCheckInfo& checkInfo) const override;
    s32 checkStrikeDisk(al::DiskHitResultBuffer* resultBuffer,
                        const al::DiskCheckInfo& checkInfo) const override;
    void searchWithSphere(const al::SphereCheckInfo& checkInfo,
                          sead::IDelegate1<al::CollisionParts*>& callback) const override;
    void movement() override;
};

static_assert(sizeof(DynamicCollisionKeeper) == 0x8);

namespace rs {

al::DynamicCollisionActor* createDynamicCollisionActor(const al::ActorInitInfo& initInfo,
                                                       s32 vertexNum,
                                                       const sead::Matrix34f* baseMtx,
                                                       al::HitSensor* sensor);
s32 searchDynamicCollisionAttributeIndex(const char* attributeName);
al::DynamicDrawActor* createDynamicDrawActor(const al::ActorInitInfo& initInfo, s32 vertexNum,
                                             const char* archiveName, bool isDrawDepthShadow);
al::DynamicDrawActor* createDynamicDrawActorForward(const al::ActorInitInfo& initInfo,
                                                    s32 vertexNum, const char* archiveName,
                                                    bool isDrawDepthShadow);
al::DynamicDrawActor* createDynamicDrawActorIndirect(const al::ActorInitInfo& initInfo,
                                                     s32 vertexNum, const char* archiveName,
                                                     bool isDrawDepthShadow);
al::DynamicDrawActor* createDynamicDrawActorPreSilhouette(const al::ActorInitInfo& initInfo,
                                                          s32 vertexNum, const char* archiveName,
                                                          bool isDrawDepthShadow);

}  // namespace rs
