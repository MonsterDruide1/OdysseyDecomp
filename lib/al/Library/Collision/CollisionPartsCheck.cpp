#include "Library/Collision/CollisionPartsCheck.h"

#include "Library/Collision/CollisionUtil.h"
#include "Library/Collision/ICollisionPartsKeeper.h"
#include "Library/Math/MathUtil.h"

namespace {
using namespace al;

class SimpleCollisionPartsKeeper : public ICollisionPartsKeeper {
public:
    SimpleCollisionPartsKeeper() = default;

    void endInit() override;

    void addCollisionParts(CollisionParts* parts) override;
    void connectToCollisionPartsList(CollisionParts* parts) override;
    void disconnectToCollisionPartsList(CollisionParts* parts) override;
    void resetToCollisionPartsList(CollisionParts* parts) override;

    u32 checkStrikePoint(HitInfo* outInfo, const CollisionCheckInfoBase& checkInfo) const override;
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
    CollisionPartsList mPartsList;
};
}  // namespace

namespace al {

u32 checkStrikePointCore(HitInfo* outHit, const CollisionPartsList& partsList,
                         const CollisionCheckInfoBase& checkInfo) {
    for (auto* parts : partsList) {
        if (alCollisionUtil::isInvalidParts(*parts, checkInfo))
            continue;

        if (alCollisionUtil::isFarAway(*parts, checkInfo.position, 0.0f))
            continue;

        if (parts->checkStrikePoint(outHit, checkInfo.position, checkInfo.triFilter) == 0)
            continue;

        return 1;
    }

    return 0;
}

u32 checkStrikeSphereCore(SphereHitResultBuffer* resultBuffer, const CollisionPartsList& partsList,
                          const SphereCheckInfo& checkInfo, bool checkMoving,
                          const sead::Vector3f& moveDirection) {
    u32 strikeNum = 0;

    for (CollisionParts* parts : partsList) {
        if (alCollisionUtil::isInvalidParts(*parts, checkInfo))
            continue;

        if (alCollisionUtil::isFarAway(*parts, checkInfo.position, checkInfo.radius))
            continue;

        strikeNum += parts->checkStrikeSphere(resultBuffer, checkInfo.position, checkInfo.radius,
                                              checkMoving, moveDirection, checkInfo.triFilter);

        if (resultBuffer->isFull())
            break;
    }

    return strikeNum;
}

u32 checkStrikeArrowCore(ArrowHitResultBuffer* resultBuffer, const CollisionPartsList& partsList,
                         const ArrowCheckInfo& checkInfo) {
    u32 strikeNum = 0;

    for (auto* parts : partsList) {
        if (alCollisionUtil::isInvalidParts(*parts, checkInfo))
            continue;

        sead::Vector3f trans;
        parts->getBaseMtx().getTranslation(trans);
        f32 range = parts->getBoundingSphereRange();

        if (!isNearCollideSphereAabb(trans, range, checkInfo.bounds))
            continue;

        if (!checkHitSegmentSphere(trans, checkInfo.position, checkInfo.target, range, nullptr,
                                   nullptr)) {
            continue;
        }

        strikeNum += parts->checkStrikeArrow(resultBuffer, checkInfo.position, checkInfo.direction,
                                             checkInfo.triFilter);

        if (resultBuffer->isFull())
            break;
    }

    return strikeNum;
}

u32 checkStrikeSphereForPlayerCore(SphereHitResultBuffer* resultBuffer,
                                   const CollisionPartsList& partsList,
                                   const SphereCheckInfo& checkInfo, bool checkMoving,
                                   const sead::Vector3f& moveDirection) {
    u32 strikeNum = 0;

    for (auto* parts : partsList) {
        if (alCollisionUtil::isInvalidParts(*parts, checkInfo))
            continue;

        if (alCollisionUtil::isFarAway(*parts, checkInfo.position, checkInfo.radius))
            continue;

        strikeNum += parts->checkStrikeSphereForPlayer(resultBuffer, checkInfo.position,
                                                       checkInfo.radius, checkInfo.triFilter);

        if (resultBuffer->isFull())
            break;
    }

    return strikeNum;
}

u32 checkStrikeDiskCore(DiskHitResultBuffer* resultBuffer, const CollisionPartsList& partsList,
                        const DiskCheckInfo& checkInfo) {
    u32 strikeNum = 0;

    for (auto* parts : partsList) {
        if (alCollisionUtil::isInvalidParts(*parts, checkInfo))
            continue;

        if (alCollisionUtil::isFarAway(*parts, checkInfo.position, checkInfo.boundingRange))
            continue;

        strikeNum +=
            parts->checkStrikeDisk(resultBuffer, checkInfo.position, checkInfo.radius,
                                   checkInfo.halfHeight, checkInfo.normal, checkInfo.triFilter);

        if (resultBuffer->isFull())
            break;
    }

    return strikeNum;
}

void updateCollisionParts(CollisionParts* parts) {
    if (!parts->isValidCollision())
        return;

    parts->updateMtx();

    if (parts->isJustValidated())
        parts->resetJustValidated();
}

void updateCollisionPartsList(CollisionPartsList* partsList) {
    for (auto* parts : *partsList)
        updateCollisionParts(parts);
}

void pushBackCollisionParts(CollisionPartsList* partsList, CollisionParts* parts) {
    partsList->pushBack(parts);
    parts->onJoinList();

    if (!parts->isValidCollision())
        partsList->erase(parts);
}

ICollisionPartsKeeper* createSimpleCollisionPartsKeeper() {
    return new SimpleCollisionPartsKeeper;
}

DiskCheckInfo::DiskCheckInfo(const sead::Vector3f& position, f32 _radius, f32 _halfHeight,
                             const sead::Vector3f& normal)
    : CollisionCheckInfoBase(position), radius(_radius), halfHeight(_halfHeight), normal(normal) {
    if (_radius > _halfHeight)
        boundingRange = sead::Mathf::sqrt(2.0f) * _radius;
    else
        boundingRange = sead::Mathf::sqrt(halfHeight * halfHeight + radius * radius);
}

}  // namespace al

namespace {
using namespace al;

void SimpleCollisionPartsKeeper::endInit() {}

void SimpleCollisionPartsKeeper::addCollisionParts(CollisionParts* parts) {
    pushBackCollisionParts(&mPartsList, parts);
}

void SimpleCollisionPartsKeeper::connectToCollisionPartsList(CollisionParts* parts) {
    pushBackCollisionParts(&mPartsList, parts);
}

void SimpleCollisionPartsKeeper::disconnectToCollisionPartsList(CollisionParts* parts) {
    parts->erase();
}

void SimpleCollisionPartsKeeper::resetToCollisionPartsList(CollisionParts* parts) {}

u32 SimpleCollisionPartsKeeper::checkStrikePoint(HitInfo* outHit,
                                                 const CollisionCheckInfoBase& checkInfo) const {
    return checkStrikePointCore(outHit, mPartsList, checkInfo);
}

u32 SimpleCollisionPartsKeeper::checkStrikeSphere(SphereHitResultBuffer* resultBuffer,
                                                  const SphereCheckInfo& checkInfo,
                                                  bool checkMoving,
                                                  const sead::Vector3f& moveDirection) const {
    return checkStrikeSphereCore(resultBuffer, mPartsList, checkInfo, checkMoving, moveDirection);
}

u32 SimpleCollisionPartsKeeper::checkStrikeArrow(ArrowHitResultBuffer* resultBuffer,
                                                 const ArrowCheckInfo& checkInfo) const {
    return checkStrikeArrowCore(resultBuffer, mPartsList, checkInfo);
}

u32 SimpleCollisionPartsKeeper::checkStrikeSphereForPlayer(SphereHitResultBuffer* resultBuffer,
                                                           const SphereCheckInfo& checkInfo) const {
    return checkStrikeSphereForPlayerCore(resultBuffer, mPartsList, checkInfo, false,
                                          sead::Vector3f::zero);
}

u32 SimpleCollisionPartsKeeper::checkStrikeDisk(DiskHitResultBuffer* resultBuffer,
                                                const DiskCheckInfo& checkInfo) const {
    return checkStrikeDiskCore(resultBuffer, mPartsList, checkInfo);
}

void SimpleCollisionPartsKeeper::searchWithSphere(const SphereCheckInfo& checkInfo,
                                                  CollisionPartsCallback& callback) const {
    for (auto it = mPartsList.begin(); it != mPartsList.end(); ++it) {
        auto* parts = *it;

        if (alCollisionUtil::isInvalidParts(*parts, checkInfo))
            continue;

        if (alCollisionUtil::isFarAway(*parts, checkInfo.position, checkInfo.radius))
            continue;

        callback.invoke(*it);
    }
}

void SimpleCollisionPartsKeeper::movement() {
    updateCollisionPartsList(&mPartsList);
}

}  // namespace
