#include "Library/Collision/CollisionUtil.h"

#include <algorithm>
#include <container/seadRingBuffer.h>

#include "Library/Base/StringUtil.h"
#include "Library/Collision/CollisionDirector.h"
#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsCheck.h"
#include "Library/Collision/HitInfo.h"
#include "Library/Collision/ICollisionPartsKeeper.h"
#include "Library/Collision/IUseCollision.h"
#include "Library/Collision/SphereInterpolator.h"
#include "Library/Collision/TriangleFilter.h"
#include "Library/CollisionFilter/PartsFilter.h"
#include "Library/Math/MathUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/ByamlUtil.h"

namespace {
using namespace al;

class TriBufferFilter : public TriangleFilterBase {
public:
    bool isInvalidTriangle(const Triangle& rTriangle) const override {
        if (mTriFilter != nullptr && mTriFilter->isInvalidTriangle(rTriangle))
            return true;

        for (const KCPrismData* prismData : mHitPrisms)
            if (prismData == rTriangle.getPrismData())
                return true;

        return false;
    }

    void reset(const TriangleFilterBase* triFilter) {
        mHitPrisms.clear();
        mTriFilter = triFilter;
    }

    void addHitPrism(const KCPrismData* pPrismData) { mHitPrisms.pushBack(pPrismData); }

private:
    sead::FixedRingBuffer<const KCPrismData*, 256> mHitPrisms;
    const TriangleFilterBase* mTriFilter = nullptr;
};

}  // namespace

namespace al {

const char* getMaterialCodeName(const Triangle& triangle) {
    return getCollisionCodeName(triangle, "MaterialCode");
}

const char* getCollisionCodeName(const Triangle& triangle, const char* category) {
    ByamlIter attributeIter;
    triangle.getAttributes(&attributeIter);

    if (!attributeIter.isValid())
        return nullptr;

    if (isTypeStringByKey(attributeIter, category)) {
        const char* code = nullptr;
        if (attributeIter.tryGetStringByKey(&code, category))
            return code;
    }

    ByamlIter codeIter;
    if (attributeIter.tryGetIterByKey(&codeIter, category)) {
        const char* code = nullptr;
        if (codeIter.tryGetStringByIndex(&code, 0))
            return code;
    }

    return nullptr;
}

const char* getFloorCodeName(const Triangle& triangle) {
    return getCollisionCodeName(triangle, "FloorCode");
}

const char* getWallCodeName(const Triangle& triangle) {
    return getCollisionCodeName(triangle, "WallCode");
}

const char* getCameraCodeName(const Triangle& triangle) {
    return getCollisionCodeName(triangle, "CameraCode");
}

const char* getMaterialCodeName(const HitInfo* hitInfo) {
    return getMaterialCodeName(hitInfo->triangle);
}

const char* getFloorCodeName(const HitInfo* hitInfo) {
    return getFloorCodeName(hitInfo->triangle);
}

const char* getWallCodeName(const HitInfo* hitInfo) {
    return getWallCodeName(hitInfo->triangle);
}

const char* getCameraCodeName(const HitInfo* hitInfo) {
    return getCameraCodeName(hitInfo->triangle);
}

const char* getCollisionCodeName(const HitInfo* hitInfo, const char* category) {
    return getCollisionCodeName(hitInfo->triangle, category);
}

bool isMaterialCode(const Triangle& triangle, const char* code) {
    return isEqualString(code, getMaterialCodeName(triangle));
}

bool isFloorCode(const Triangle& triangle, const char* code) {
    return isEqualString(code, getFloorCodeName(triangle));
}

bool isWallCode(const Triangle& triangle, const char* code) {
    return isEqualString(code, getWallCodeName(triangle));
}

bool isCameraCode(const Triangle& triangle, const char* code) {
    return isEqualString(code, getCameraCodeName(triangle));
}

bool isCollisionCode(const Triangle& triangle, const char* category, const char* code) {
    return isEqualString(code, getCollisionCodeName(triangle, category));
}

bool isMaterialCode(const HitInfo* hitInfo, const char* code) {
    return isMaterialCode(hitInfo->triangle, code);
}

bool isFloorCode(const HitInfo* hitInfo, const char* code) {
    return isFloorCode(hitInfo->triangle, code);
}

bool isWallCode(const HitInfo* hitInfo, const char* code) {
    return isWallCode(hitInfo->triangle, code);
}

bool isCameraCode(const HitInfo* hitInfo, const char* code) {
    return isCameraCode(hitInfo->triangle, code);
}

bool isCollisionCode(const HitInfo* hitInfo, const char* code, const char* category) {
    return isCollisionCode(hitInfo->triangle, code, category);
}

const char* getSpecialPurposeName(const HitInfo* hitInfo) {
    return hitInfo->triangle.getParts()->getSpecialPurposeName();
}

bool isCollisionValid(const CollisionParts* parts) {
    return parts->isValid();
}

bool isCollisionMoving(const CollisionParts* parts) {
    return parts->isMoving();
}

bool calcForceMovePower(sead::Vector3f* outMovePower, const CollisionParts* parts,
                        const sead::Vector3f& position) {
    if (!isCollisionMoving(parts)) {
        outMovePower->set(0.0f, 0.0f, 0.0f);
        return false;
    }

    parts->calcForceMovePower(outMovePower, position);

    return true;
}

bool calcForceMovePowerExceptNormal(sead::Vector3f* outMovePower, const CollisionParts* parts,
                                    const sead::Vector3f& position, const sead::Vector3f& normal) {
    if (!isCollisionMoving(parts)) {
        outMovePower->set(0.0f, 0.0f, 0.0f);
        return false;
    }

    parts->calcForceMovePower(outMovePower, position);
    if (outMovePower->dot(normal) > 0.0f)
        verticalizeVec(outMovePower, normal, *outMovePower);

    return true;
}

}  // namespace al

namespace alCollisionUtil {

static CollisionDirector* getDirector(const IUseCollision* user) {
    return user->getCollisionDirector();
}

ICollisionPartsKeeper* getCollisionPartsKeeper(const IUseCollision* user) {
    return getDirector(user)->getActivePartsKeeper();
}

bool isCollisionMoving(const HitInfo* hitInfo) {
    return hitInfo->triangle.isHostMoved();
}

const sead::Vector3f& getCollisionHitPos(const HitInfo* hitInfo) {
    return hitInfo->hitPos;
}

const sead::Vector3f& getCollisionHitNormal(const HitInfo* hitInfo) {
    return hitInfo->triangle.getNormal(0);
}

const sead::Vector3f& getCollisionMovingReaction(const HitInfo* hitInfo) {
    return hitInfo->movingReaction;
}

const HitSensor* getCollisionHitSensor(const HitInfo* hitInfo) {
    return hitInfo->triangle.getSensor();
}

const CollisionParts* getCollisionHitParts(const HitInfo* hitInfo) {
    return hitInfo->triangle.getParts();
}

const LiveActor* getCollisionHitActor(const HitInfo* hitInfo) {
    return getCollisionHitParts(hitInfo)->getConnectedHost();
}

u32 checkStrikeSphere(const IUseCollision* user, const sead::Vector3f& position, f32 radius,
                      const CollisionPartsFilterBase* partsFilter,
                      const TriangleFilterBase* triFilter) {
    auto* director = getDirector(user);

    director->setPartsFilter(partsFilter);
    director->setTriFilter(triFilter);
    return director->checkStrikeSphere(position, radius, false, sead::Vector3f::zero);
}

u32 checkStrikeSphereMovingReaction(const IUseCollision* user, const sead::Vector3f& position,
                                    f32 radius, const sead::Vector3f& moveDirection,
                                    const CollisionPartsFilterBase* partsFilter,
                                    const TriangleFilterBase* triFilter) {
    auto* director = getDirector(user);

    director->setPartsFilter(partsFilter);
    director->setTriFilter(triFilter);
    return director->checkStrikeSphere(position, radius, true, moveDirection);
}

u32 checkStrikeDisk(const IUseCollision* user, const sead::Vector3f& position, f32 radius,
                    f32 halfHeight, const sead::Vector3f& normal,
                    const CollisionPartsFilterBase* partsFilter,
                    const TriangleFilterBase* triFilter) {
    auto* director = getDirector(user);

    director->setPartsFilter(partsFilter);
    director->setTriFilter(triFilter);
    return director->checkStrikeDisk(position, radius, halfHeight, normal);
}

u32 checkStrikeArrow(const IUseCollision* user, const sead::Vector3f& position,
                     const sead::Vector3f& direction, const CollisionPartsFilterBase* partsFilter,
                     const TriangleFilterBase* triFilter) {
    auto* director = getDirector(user);

    director->setPartsFilter(partsFilter);
    director->setTriFilter(triFilter);
    return director->checkStrikeArrow(position, direction);
}

u32 checkStrikeSphereForPlayer(const IUseCollision* user, const sead::Vector3f& position,
                               f32 radius, const CollisionPartsFilterBase* partsFilter,
                               const TriangleFilterBase* triFilter) {
    auto* director = getDirector(user);

    director->setPartsFilter(partsFilter);
    director->setTriFilter(triFilter);
    return director->checkStrikeSphereForPlayer(position, radius);
}

bool getFirstPolyOnArrow(const IUseCollision* user, const ArrowHitInfo** outHitInfo,
                         const sead::Vector3f& position, const sead::Vector3f& direction,
                         const CollisionPartsFilterBase* partsFilter,
                         const TriangleFilterBase* triFilter) {
    u32 strikeNum = checkStrikeArrow(user, position, direction, partsFilter, triFilter);
    if (strikeNum == 0)
        return false;

    f32 minDist = 1000000.0f;
    s32 minIndex = -1;

    for (u32 i = 0; i != strikeNum; i++) {
        const auto& info = getStrikeArrowInfo(user, i);

        bool invalidTri = triFilter && triFilter->isInvalidTriangle(info.triangle);
        if (!invalidTri && info.distance < minDist) {
            minDist = info.distance;
            minIndex = i;
        }
    }

    if (minIndex == -1)
        return false;

    if (outHitInfo)
        *outHitInfo = &getStrikeArrowInfo(user, minIndex);

    return true;
}

const ArrowHitInfo& getStrikeArrowInfo(const IUseCollision* user, u32 index) {
    return getDirector(user)->getStrikeArrowInfo(index);
}

bool getFirstPolyOnArrow(const IUseCollision* user, sead::Vector3f* outHitPos,
                         Triangle* outTriangle, const sead::Vector3f& position,
                         const sead::Vector3f& direction, const char* specialPurpose) {
    CollisionPartsFilterSpecialPurpose filter(specialPurpose);
    return getFirstPolyOnArrow(user, outHitPos, outTriangle, position, direction, &filter, nullptr);
}

bool getFirstPolyOnArrow(const IUseCollision* user, sead::Vector3f* outHitPos,
                         Triangle* outTriangle, const sead::Vector3f& position,
                         const sead::Vector3f& direction,
                         const CollisionPartsFilterBase* partsFilter,
                         const TriangleFilterBase* triFilter) {
    const ArrowHitInfo* hitInfo = nullptr;
    if (!getFirstPolyOnArrow(user, &hitInfo, position, direction, partsFilter, triFilter))
        return false;

    if (outHitPos)
        *outHitPos = hitInfo->hitPos;

    if (outTriangle)
        *outTriangle = hitInfo->triangle;

    return true;
}

bool getHitPosOnArrow(const IUseCollision* user, sead::Vector3f* outHitPos,
                      const sead::Vector3f& position, const sead::Vector3f& direction,
                      const CollisionPartsFilterBase* partsFilter,
                      const TriangleFilterBase* triFilter) {
    Triangle triangle;
    return getFirstPolyOnArrow(user, outHitPos, &triangle, position, direction, partsFilter,
                               triFilter);
}

bool getHitPosAndNormalOnArrow(const IUseCollision* user, sead::Vector3f* outHitPos,
                               sead::Vector3f* outNormal, const sead::Vector3f& position,
                               const sead::Vector3f& direction,
                               const CollisionPartsFilterBase* partsFilter,
                               const TriangleFilterBase* triFilter) {
    Triangle triangle;
    if (!getFirstPolyOnArrow(user, outHitPos, &triangle, position, direction, partsFilter,
                             triFilter))
        return false;

    outNormal->set(triangle.getNormal(0));
    return true;
}

bool getHitPosAndNormalAndSensorOnArrow(const IUseCollision* user, sead::Vector3f* outHitPos,
                                        sead::Vector3f* outNormal, HitSensor** outSensor,
                                        const sead::Vector3f& position,
                                        const sead::Vector3f& direction,
                                        const CollisionPartsFilterBase* partsFilter,
                                        const TriangleFilterBase* triFilter) {
    Triangle triangle;
    if (!getFirstPolyOnArrow(user, outHitPos, &triangle, position, direction, partsFilter,
                             triFilter))
        return false;

    outNormal->set(triangle.getNormal(0));
    *outSensor = triangle.getParts()->getConnectedSensor();
    return true;
}

bool getFirstCollisionSensorOnArrow(const IUseCollision* user, sead::Vector3f* outHitPos,
                                    HitSensor** outSensor, const sead::Vector3f& position,
                                    const sead::Vector3f& direction,
                                    const CollisionPartsFilterBase* partsFilter,
                                    const TriangleFilterBase* triFilter) {
    Triangle triangle;
    if (!getFirstPolyOnArrow(user, outHitPos, &triangle, position, direction, partsFilter,
                             triFilter))
        return false;

    *outSensor = triangle.getParts()->getConnectedSensor();
    return true;
}

bool getLastCollisionSensorOnArrow(const IUseCollision* user, sead::Vector3f* outHitPos,
                                   HitSensor** outSensor, const sead::Vector3f& position,
                                   const sead::Vector3f& direction,
                                   const CollisionPartsFilterBase* partsFilter,
                                   const TriangleFilterBase* triFilter) {
    Triangle triangle;
    if (!getLastPolyOnArrow(user, outHitPos, &triangle, position, direction, partsFilter,
                            triFilter))
        return false;

    *outSensor = triangle.getParts()->getConnectedSensor();
    return true;
}

bool getLastPolyOnArrow(const IUseCollision* user, sead::Vector3f* outHitPos, Triangle* outTriangle,
                        const sead::Vector3f& position, const sead::Vector3f& direction,
                        const CollisionPartsFilterBase* partsFilter,
                        const TriangleFilterBase* triFilter) {
    const ArrowHitInfo* hitInfo = nullptr;
    if (!getLastPolyOnArrow(user, &hitInfo, position, direction, partsFilter, triFilter))
        return false;

    if (outHitPos)
        *outHitPos = hitInfo->hitPos;

    if (outTriangle)
        *outTriangle = hitInfo->triangle;

    return true;
}

bool getLastPolyOnArrow(const IUseCollision* user, const ArrowHitInfo** outHitInfo,
                        const sead::Vector3f& position, const sead::Vector3f& direction,
                        const CollisionPartsFilterBase* partsFilter,
                        const TriangleFilterBase* triFilter) {
    u32 strikeNum = checkStrikeArrow(user, position, direction, partsFilter, triFilter);
    if (strikeNum == 0)
        return false;

    f32 maxDist = 0.0f;
    s32 maxIndex = -1;

    for (u32 i = 0; i != strikeNum; i++) {
        const auto& info = getStrikeArrowInfo(user, i);

        bool invalidTri = triFilter && triFilter->isInvalidTriangle(info.triangle);
        if (!invalidTri && maxDist < info.distance) {
            maxDist = info.distance;
            maxIndex = i;
        }
    }

    if (maxIndex == -1)
        return false;

    if (outHitInfo)
        *outHitInfo = &getStrikeArrowInfo(user, maxIndex);

    return true;
}

const CollisionParts* getStrikeArrowCollisionParts(const IUseCollision* user,
                                                   sead::Vector3f* outHitPos,
                                                   const sead::Vector3f& position,
                                                   const sead::Vector3f& direction,
                                                   const CollisionPartsFilterBase* partsFilter,
                                                   const TriangleFilterBase* triFilter) {
    Triangle triangle;
    if (!getFirstPolyOnArrow(user, outHitPos, &triangle, position, direction, partsFilter,
                             triFilter))
        return nullptr;

    return triangle.getParts();
}

const HitSensor* tryGetStrikeArrowCollisionSensor(const IUseCollision* user,
                                                  const sead::Vector3f& position,
                                                  const sead::Vector3f& direction,
                                                  const CollisionPartsFilterBase* partsFilter,
                                                  const TriangleFilterBase* triFilter) {
    auto* parts =
        getStrikeArrowCollisionParts(user, nullptr, position, direction, partsFilter, triFilter);

    if (parts)
        return parts->getConnectedSensor();

    return nullptr;
}

s32 getStrikeArrowInfoNum(const IUseCollision* user) {
    return getDirector(user)->getStrikeArrowInfoNum();
}

const SphereHitInfo& getStrikeSphereInfo(const IUseCollision* user, u32 index) {
    return getDirector(user)->getStrikeSphereInfo(index);
}

s32 getStrikeSphereInfoNum(const IUseCollision* user) {
    return getDirector(user)->getStrikeSphereInfoNum();
}

const sead::Vector3f& getStrikeSphereHitPos(const IUseCollision* user, u32 index) {
    return getDirector(user)->getStrikeSphereInfo(index).hitPos;
}

const DiskHitInfo& getStrikeDiskInfo(const IUseCollision* user, u32 index) {
    return getDirector(user)->getStrikeDiskInfo(index);
}

s32 getStrikeDiskInfoNum(const IUseCollision* user) {
    return getDirector(user)->getStrikeDiskInfoNum();
}

const sead::Vector3f& getStrikeDiskHitPos(const IUseCollision* user, u32 index) {
    return getDirector(user)->getStrikeDiskInfo(index).hitPos;
}

bool isInvalidParts(const CollisionParts& parts, const CollisionCheckInfoBase& checkInfo) {
    if (parts.getSpecialPurposeName() != nullptr && checkInfo.partsFilter == nullptr)
        return true;

    if (!parts.isValid())
        return true;

    if (checkInfo.partsFilter && checkInfo.partsFilter->isInvalidParts(parts))
        return true;

    return false;
}

bool isFarAway(const CollisionParts& parts, const sead::Vector3f& position, f32 maxDistance) {
    f32 range = parts.getBoundingSphereRange() + maxDistance;
    const sead::Vector3f& trans = parts.getTrans();

    f32 dx = sead::Mathf::abs(trans.x - position.x);
    if (range < dx)
        return true;

    f32 dy = sead::Mathf::abs(trans.y - position.y);
    if (range < dy)
        return true;

    f32 dz = sead::Mathf::abs(trans.z - position.z);
    if (range < dz)
        return true;

    return sead::Vector3f(dx, dy, dz).squaredLength() > sead::Mathf::square(range);
}

static TriBufferFilter sTriBufferFilter;

static f32 calcSphereHitInterp(const sead::Vector3f& hitPos, const sead::Vector3f& center,
                               f32 radius, const sead::Vector3f& moveDirection) {
    f32 a = moveDirection.squaredLength();
    f32 b = (hitPos - center).dot(moveDirection) * 2.0f;
    f32 c = (hitPos - center).squaredLength() - radius * radius;

    f32 discriminant = (b * b) - (4.0f * a * c);
    if (discriminant < 0.0f)
        return 0.0f;

    f32 sqrtDiscriminant = sead::Mathf::sqrt(discriminant);

    f32 interp =
        1.0f / a * (b > 0.0f ? ((-b - sqrtDiscriminant) * 0.5f) : ((sqrtDiscriminant - b) * 0.5f));

    if (interp < 0.0f)
        interp = c / (a * interp);

    return interp;
}

// NON_MATCHING: Float math and instruction ordering (https://decomp.me/scratch/41pIb)
s32 checkStrikeSphereMove(const IUseCollision* user, SphereMoveHitInfo* outHitInfos, s32 hitNumMax,
                          const sead::Vector3f& position, f32 radius,
                          const sead::Vector3f& moveDirection,
                          const CollisionPartsFilterBase* partsFilter,
                          const TriangleFilterBase* triFilter) {
    sTriBufferFilter.reset(triFilter);

    SphereInterpolator interpolator;
    interpolator.startInterp(position, position + moveDirection, radius, radius, radius);
    interpolator.nextStep();

    s32 hitNum = 0;

    while (!interpolator.isEnd()) {
        sead::Vector3f currPos = {0.0f, 0.0f, 0.0f};
        f32 currRadius = 1.0f;
        interpolator.calcInterp(&currPos, &currRadius, nullptr);

        u32 strikeNum = checkStrikeSphereMovingReaction(user, currPos, currRadius, moveDirection,
                                                        partsFilter, &sTriBufferFilter);
        if (strikeNum != 0) {
            for (u32 i = 0; i < strikeNum; i++) {
                if (hitNum >= hitNumMax)
                    break;

                const auto& hitInfo = getStrikeSphereInfo(user, i);
                f32 dist = hitInfo.distance;
                const Triangle& tri = hitInfo.triangle;
                const sead::Vector3f& faceNormal = tri.getFaceNormal();

                if (!(faceNormal.dot(moveDirection) >= 0.0f)) {
                    if (hitInfo.isCollisionAtFace()) {
                        f32 moveInterp = dist / sead::Mathf::abs(faceNormal.dot(moveDirection));
                        sead::Vector3f moveBack = moveDirection * -moveInterp;
                        sead::Vector3f slide;
                        verticalizeVec(&slide, faceNormal, moveBack);
                        sead::Vector3f returnPos = hitInfo.hitPos + slide;

                        sead::Vector3f pos0 = returnPos - tri.getPos(0);
                        sead::Vector3f pos1 = returnPos - tri.getPos(1);
                        sead::Vector3f pos2 = returnPos - tri.getPos(2);

                        f32 dot0 = (pos0).dot(tri.getEdgeNormal(0));
                        f32 dot1 = (pos1).dot(tri.getEdgeNormal(1));
                        f32 dot2 = (pos2).dot(tri.getEdgeNormal(2));

                        if ((dot0 > 0.0f) || (dot1 > 0.0f) || (dot2 > 0.0f)) {
                            if (isNearZero(slide))
                                continue;

                            sead::Vector3f diff = returnPos - hitInfo.hitPos;

                            s32 edgeIndex = 5;
                            if (diff.dot(tri.getEdgeNormal(0)) > 0.0f) {
                                if (diff.dot(tri.getEdgeNormal(1)) > 0.0f)
                                    edgeIndex = 6;
                            } else {
                                edgeIndex = 4;
                            }
                            edgeIndex = modi(edgeIndex, 3);

                            sead::Vector3f toVertex = tri.getPos(edgeIndex) - hitInfo.hitPos;

                            sead::Vector3f cross;
                            cross.setCross(toVertex, diff);

                            if (cross.dot(faceNormal) > 0.0f)
                                edgeIndex = modi(edgeIndex + 4, 3);
                            else
                                edgeIndex = modi(edgeIndex + 3, 3);

                            const sead::Vector3f& rCrossEdgeNormal = tri.getEdgeNormal(edgeIndex);
                            f32 denom = diff.dot(rCrossEdgeNormal);

                            if (isNearZero(denom))
                                continue;

                            f32 rate = toVertex.dot(rCrossEdgeNormal) / denom;
                            sead::Vector3f edgePos = hitInfo.hitPos + diff * rate;
                            f32 hitInterp =
                                calcSphereHitInterp(edgePos, currPos, currRadius, moveDirection);

                            outHitInfos[hitNum].moveInterp =
                                interpolator.getCurrentInterp() - hitInterp;
                            outHitInfos[hitNum].position = edgePos;
                            outHitInfos[hitNum].triangle = tri;
                            outHitInfos[hitNum].distance = dist;
                        } else {
                            outHitInfos[hitNum].moveInterp =
                                interpolator.getCurrentInterp() - moveInterp;
                            outHitInfos[hitNum].position = returnPos;
                            outHitInfos[hitNum].triangle = tri;
                            outHitInfos[hitNum].distance = dist;
                        }
                    } else {
                        f32 hitInterp =
                            calcSphereHitInterp(hitInfo.hitPos, currPos, currRadius, moveDirection);

                        outHitInfos[hitNum].moveInterp =
                            interpolator.getCurrentInterp() - hitInterp;
                        outHitInfos[hitNum].position = hitInfo.hitPos;
                        outHitInfos[hitNum].triangle = tri;
                        outHitInfos[hitNum].distance = dist;
                    }

                    hitNum++;
                }

                sTriBufferFilter.addHitPrism(tri.getPrismData());
            }
        }

        if (hitNum >= hitNumMax)
            break;

        interpolator.nextStep();
    }

    std::sort(outHitInfos, outHitInfos + hitNum, SphereMoveHitInfo::compare);
    return hitNum;
}

bool SphereMoveHitInfo::compare(const SphereMoveHitInfo& lhs, const SphereMoveHitInfo& rhs) {
    return lhs.moveInterp < rhs.moveInterp;
}

void searchCollisionParts(const IUseCollision* user, const sead::Vector3f& position, f32 radius,
                          CollisionPartsCallback& callback,
                          const CollisionPartsFilterBase* partsFilter) {
    getDirector(user)->searchCollisionPartsWithSphere(position, radius, callback, partsFilter);
}

void validateCollisionPartsPtrArray(const IUseCollision* user,
                                    sead::PtrArray<CollisionParts>* partsPtrArray) {
    getDirector(user)->validateCollisionPartsPtrArray(partsPtrArray);
}

void invalidateCollisionPartsPtrArray(const IUseCollision* user) {
    getDirector(user)->invalidateCollisionPartsPtrArray();
}

sead::PtrArray<CollisionParts>* getCollisionPartsPtrArray(const IUseCollision* user) {
    return getDirector(user)->getCollisionPartsPtrArray();
}

}  // namespace alCollisionUtil
