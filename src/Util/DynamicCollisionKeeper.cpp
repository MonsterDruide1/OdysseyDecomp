#include "Util/DynamicCollisionKeeper.h"

#include <prim/seadSafeString.h>

#include "Library/Base/StringUtil.h"
#include "Library/Obj/DynamicCollisionActor.h"
#include "Library/Obj/DynamicDrawActor.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

DynamicCollisionKeeper::DynamicCollisionKeeper() = default;

void DynamicCollisionKeeper::endInit() {}

s32 DynamicCollisionKeeper::checkStrikePoint(al::HitInfo* hitInfo,
                                             const al::CollisionCheckInfoBase& checkInfo) const {
    return 0;
}

s32 DynamicCollisionKeeper::checkStrikeSphere(al::SphereHitResultBuffer* resultBuffer,
                                              const al::SphereCheckInfo& checkInfo,
                                              bool isFollowMovingCollision,
                                              const sead::Vector3f& movement) const {
    return 0;
}

s32 DynamicCollisionKeeper::checkStrikeArrow(al::ArrowHitResultBuffer* resultBuffer,
                                             const al::ArrowCheckInfo& checkInfo) const {
    return 0;
}

s32 DynamicCollisionKeeper::checkStrikeSphereForPlayer(al::SphereHitResultBuffer* resultBuffer,
                                                       const al::SphereCheckInfo& checkInfo) const {
    return 0;
}

s32 DynamicCollisionKeeper::checkStrikeDisk(al::DiskHitResultBuffer* resultBuffer,
                                            const al::DiskCheckInfo& checkInfo) const {
    return 0;
}

void DynamicCollisionKeeper::movement() {}

void DynamicCollisionKeeper::addCollisionParts(al::CollisionParts* parts) {}

void DynamicCollisionKeeper::connectToCollisionPartsList(al::CollisionParts* parts) {}

void DynamicCollisionKeeper::disconnectToCollisionPartsList(al::CollisionParts* parts) {}

void DynamicCollisionKeeper::searchWithSphere(
    const al::SphereCheckInfo& checkInfo, sead::IDelegate1<al::CollisionParts*>& callback) const {}

al::DynamicCollisionActor* rs::createDynamicCollisionActor(const al::ActorInitInfo& initInfo,
                                                           s32 vertexNum,
                                                           const sead::Matrix34f* baseMtx,
                                                           al::HitSensor* sensor) {
    const u8* attributeByml =
        al::getBymlFromObjectResource(sead::SafeString("DynamicCollisionAttribute"),
                                      sead::SafeString("DynamicCollisionAttribute"));

    return new al::DynamicCollisionActor(initInfo, vertexNum, baseMtx, sensor, attributeByml);
}

s32 rs::searchDynamicCollisionAttributeIndex(const char* attributeName) {
    const u8* attributeByml =
        al::getBymlFromObjectResource(sead::SafeString("DynamicCollisionAttribute"),
                                      sead::SafeString("DynamicCollisionAttribute"));
    al::ByamlIter attributeIter(attributeByml);

    s32 dataNum = al::getByamlIterDataNum(attributeIter);
    for (s32 i = 0; i < dataNum; i++) {
        al::ByamlIter entryIter;
        al::getByamlIterByIndex(&entryIter, attributeIter, i);

        const char* name;
        if (!al::tryGetByamlString(&name, entryIter, "Name"))
            return 0;

        if (al::isEqualString(name, attributeName))
            return i;
    }

    return 0;
}

al::DynamicDrawActor* rs::createDynamicDrawActor(const al::ActorInitInfo& initInfo, s32 vertexNum,
                                                 const char* archiveName, bool isDrawDepthShadow) {
    return new al::DynamicDrawActor(initInfo, vertexNum, archiveName,
                                    al::DynamicDrawActor::DrawCategory_Default, isDrawDepthShadow);
}

al::DynamicDrawActor* rs::createDynamicDrawActorForward(const al::ActorInitInfo& initInfo,
                                                        s32 vertexNum, const char* archiveName,
                                                        bool isDrawDepthShadow) {
    return new al::DynamicDrawActor(initInfo, vertexNum, archiveName,
                                    al::DynamicDrawActor::DrawCategory_Forward, isDrawDepthShadow);
}

al::DynamicDrawActor* rs::createDynamicDrawActorIndirect(const al::ActorInitInfo& initInfo,
                                                         s32 vertexNum, const char* archiveName,
                                                         bool isDrawDepthShadow) {
    return new al::DynamicDrawActor(initInfo, vertexNum, archiveName,
                                    al::DynamicDrawActor::DrawCategory_Indirect, isDrawDepthShadow);
}

al::DynamicDrawActor* rs::createDynamicDrawActorPreSilhouette(const al::ActorInitInfo& initInfo,
                                                              s32 vertexNum,
                                                              const char* archiveName,
                                                              bool isDrawDepthShadow) {
    return new al::DynamicDrawActor(initInfo, vertexNum, archiveName,
                                    al::DynamicDrawActor::DrawCategory_PreSilhouette,
                                    isDrawDepthShadow);
}
