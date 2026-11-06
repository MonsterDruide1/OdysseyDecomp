#include "Library/Collision/CollisionPartsKeeperPtrArray.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsCheck.h"
#include "Library/Collision/CollisionUtil.h"

namespace al {

CollisionPartsKeeperPtrArray::CollisionPartsKeeperPtrArray() = default;

u32 CollisionPartsKeeperPtrArray::checkStrikePoint(HitInfo* outHit,
                                                   const CollisionCheckInfoBase& checkInfo) const {
    s32 size = mPartsPtrArray->size();
    for (s32 i = 0; i < size; i++) {
        if (mPartsPtrArray->at(i)->checkStrikePoint(outHit, checkInfo.position,
                                                    checkInfo.triFilter) == 0)
            continue;

        return 1;
    }

    return 0;
}

u32 CollisionPartsKeeperPtrArray::checkStrikeSphere(SphereHitResultBuffer* resultBuffer,
                                                    const SphereCheckInfo& checkInfo,
                                                    bool checkMoving,
                                                    const sead::Vector3f& moveDirection) const {
    u32 strikeNum = 0;

    s32 size = mPartsPtrArray->size();
    for (s32 i = 0; i < size; i++) {
        strikeNum += mPartsPtrArray->at(i)->checkStrikeSphere(resultBuffer, checkInfo.position,
                                                              checkInfo.radius, checkMoving,
                                                              moveDirection, checkInfo.triFilter);
        if (resultBuffer->isFull())
            break;
    }

    return strikeNum;
}

u32 CollisionPartsKeeperPtrArray::checkStrikeArrow(ArrowHitResultBuffer* resultBuffer,
                                                   const ArrowCheckInfo& checkInfo) const {
    u32 strikeNum = 0;

    s32 size = mPartsPtrArray->size();
    for (s32 i = 0; i < size; i++) {
        strikeNum += mPartsPtrArray->at(i)->checkStrikeArrow(
            resultBuffer, checkInfo.position, checkInfo.direction, checkInfo.triFilter);
        if (resultBuffer->isFull())
            break;
    }

    return strikeNum;
}

u32 CollisionPartsKeeperPtrArray::checkStrikeSphereForPlayer(
    SphereHitResultBuffer* resultBuffer, const SphereCheckInfo& checkInfo) const {
    u32 strikeNum = 0;

    s32 size = mPartsPtrArray->size();
    for (s32 i = 0; i < size; i++) {
        strikeNum += mPartsPtrArray->at(i)->checkStrikeSphere(
            resultBuffer, checkInfo.position, checkInfo.radius, false, sead::Vector3f::zero,
            checkInfo.triFilter);
        if (resultBuffer->isFull())
            break;
    }

    return strikeNum;
}

u32 CollisionPartsKeeperPtrArray::checkStrikeDisk(DiskHitResultBuffer* resultBuffer,
                                                  const DiskCheckInfo& checkInfo) const {
    // Not implemented
    return 0;
}

void CollisionPartsKeeperPtrArray::searchWithSphere(const SphereCheckInfo& checkInfo,
                                                    CollisionPartsCallback& callback) const {
    s32 size = mPartsPtrArray->size();
    for (s32 i = 0; i < size; i++) {
        if (alCollisionUtil::isFarAway(*mPartsPtrArray->at(i), checkInfo.position,
                                       checkInfo.radius))
            continue;

        callback.invoke(mPartsPtrArray->at(i));
    }
}

void CollisionPartsKeeperPtrArray::endInit() {}

void CollisionPartsKeeperPtrArray::addCollisionParts(CollisionParts* parts) {}

void CollisionPartsKeeperPtrArray::connectToCollisionPartsList(CollisionParts* parts) {}

void CollisionPartsKeeperPtrArray::disconnectToCollisionPartsList(CollisionParts* parts) {}

void CollisionPartsKeeperPtrArray::resetToCollisionPartsList(CollisionParts* parts) {}

void CollisionPartsKeeperPtrArray::movement() {}
}  // namespace al
