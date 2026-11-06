#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/HitInfo.h"

namespace al {
struct ArrowHitInfo;
class CollisionPartsFilterBase;
class CollisionParts;
struct CollisionCheckInfoBase;
struct DiskHitInfo;
struct HitInfo;
class HitSensor;
class ICollisionPartsKeeper;
class IUseCollision;
class LiveActor;
struct SphereHitInfo;
class Triangle;
class TriangleFilterBase;

const char* getMaterialCodeName(const Triangle& triangle);
const char* getCollisionCodeName(const Triangle& triangle, const char* category);
const char* getFloorCodeName(const Triangle& triangle);
const char* getWallCodeName(const Triangle& triangle);
const char* getCameraCodeName(const Triangle& triangle);

const char* getMaterialCodeName(const HitInfo* hitInfo);
const char* getFloorCodeName(const HitInfo* hitInfo);
const char* getWallCodeName(const HitInfo* hitInfo);
const char* getCameraCodeName(const HitInfo* hitInfo);
const char* getCollisionCodeName(const HitInfo* hitInfo, const char* category);

bool isMaterialCode(const Triangle& triangle, const char* code);
bool isFloorCode(const Triangle& triangle, const char* code);
bool isWallCode(const Triangle& triangle, const char* code);
bool isCameraCode(const Triangle& triangle, const char* code);
bool isCollisionCode(const Triangle& triangle, const char* code, const char* category);

bool isMaterialCode(const HitInfo* hitInfo, const char* code);
bool isFloorCode(const HitInfo* hitInfo, const char* code);
bool isWallCode(const HitInfo* hitInfo, const char* code);
bool isCameraCode(const HitInfo* hitInfo, const char* code);
bool isCollisionCode(const HitInfo* hitInfo, const char* code, const char* category);

const char* getSpecialPurposeName(const HitInfo* hitInfo);

bool isCollisionValid(const CollisionParts* parts);
bool isCollisionMoving(const CollisionParts* parts);

bool calcForceMovePower(sead::Vector3f* outMovePower, const CollisionParts* parts,
                        const sead::Vector3f& position);
bool calcForceMovePowerExceptNormal(sead::Vector3f* outMovePower, const CollisionParts* parts,
                                    const sead::Vector3f& position, const sead::Vector3f& normal);

}  // namespace al

namespace alCollisionUtil {

struct SphereMoveHitInfo {
    f32 moveInterp;
    f32 distance;
    sead::Vector3f position;
    al::Triangle triangle;

    static bool compare(const SphereMoveHitInfo& lhs, const SphereMoveHitInfo& rhs);
};

static_assert(sizeof(SphereMoveHitInfo) == 0x88);

al::ICollisionPartsKeeper* getCollisionPartsKeeper(const al::IUseCollision* user);

bool isCollisionMoving(const al::HitInfo*);

const sead::Vector3f& getCollisionHitPos(const al::HitInfo* hitInfo);
const sead::Vector3f& getCollisionHitNormal(const al::HitInfo* hitInfo);
const sead::Vector3f& getCollisionMovingReaction(const al::HitInfo* hitInfo);
const al::HitSensor* getCollisionHitSensor(const al::HitInfo* hitInfo);
const al::CollisionParts* getCollisionHitParts(const al::HitInfo* hitInfo);
const al::LiveActor* getCollisionHitActor(const al::HitInfo* hitInfo);

u32 checkStrikeSphere(const al::IUseCollision* user, const sead::Vector3f& position, f32 radius,
                      const al::CollisionPartsFilterBase* partsFilter,
                      const al::TriangleFilterBase* triFilter);
u32 checkStrikeSphereMovingReaction(const al::IUseCollision* user, const sead::Vector3f& position,
                                    f32 radius, const sead::Vector3f& moveDirection,
                                    const al::CollisionPartsFilterBase* partsFilter,
                                    const al::TriangleFilterBase* triFilter);
u32 checkStrikeDisk(const al::IUseCollision* user, const sead::Vector3f& position, f32 radius,
                    f32 halfHeight, const sead::Vector3f& normal,
                    const al::CollisionPartsFilterBase* partsFilter,
                    const al::TriangleFilterBase* triFilter);
u32 checkStrikeArrow(const al::IUseCollision* user, const sead::Vector3f& position,
                     const sead::Vector3f& direction,
                     const al::CollisionPartsFilterBase* partsFilter,
                     const al::TriangleFilterBase* triFilter);
u32 checkStrikeSphereForPlayer(const al::IUseCollision* user, const sead::Vector3f& position,
                               f32 radius, const al::CollisionPartsFilterBase* partsFilter,
                               const al::TriangleFilterBase* triFilter);

bool getFirstPolyOnArrow(const al::IUseCollision* user, const al::ArrowHitInfo** outHitInfo,
                         const sead::Vector3f& position, const sead::Vector3f& direction,
                         const al::CollisionPartsFilterBase* partsFilter,
                         const al::TriangleFilterBase* triFilter);

const al::ArrowHitInfo& getStrikeArrowInfo(const al::IUseCollision* user, u32 index);

bool getFirstPolyOnArrow(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                         al::Triangle* outTriangle, const sead::Vector3f& position,
                         const sead::Vector3f& direction, const char* specialPurpose);
bool getFirstPolyOnArrow(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                         al::Triangle* outTriangle, const sead::Vector3f& position,
                         const sead::Vector3f& direction,
                         const al::CollisionPartsFilterBase* partsFilter,
                         const al::TriangleFilterBase* triFilter);

bool getHitPosOnArrow(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                      const sead::Vector3f& position, const sead::Vector3f& direction,
                      const al::CollisionPartsFilterBase* partsFilter,
                      const al::TriangleFilterBase* triFilter);
bool getHitPosAndNormalOnArrow(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                               sead::Vector3f* outNormal, const sead::Vector3f& position,
                               const sead::Vector3f& direction,
                               const al::CollisionPartsFilterBase* partsFilter,
                               const al::TriangleFilterBase* triFilter);
bool getHitPosAndNormalAndSensorOnArrow(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                                        sead::Vector3f* outNormal, al::HitSensor** outSensor,
                                        const sead::Vector3f& position,
                                        const sead::Vector3f& direction,
                                        const al::CollisionPartsFilterBase* partsFilter,
                                        const al::TriangleFilterBase* triFilter);

bool getFirstCollisionSensorOnArrow(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                                    al::HitSensor** outSensor, const sead::Vector3f& position,
                                    const sead::Vector3f& direction,
                                    const al::CollisionPartsFilterBase* partsFilter,
                                    const al::TriangleFilterBase* triFilter);
bool getLastCollisionSensorOnArrow(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                                   al::HitSensor** outSensor, const sead::Vector3f& position,
                                   const sead::Vector3f& direction,
                                   const al::CollisionPartsFilterBase* partsFilter,
                                   const al::TriangleFilterBase* triFilter);

bool getLastPolyOnArrow(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                        al::Triangle* outTriangle, const sead::Vector3f& position,
                        const sead::Vector3f& direction,
                        const al::CollisionPartsFilterBase* partsFilter,
                        const al::TriangleFilterBase* triFilter);
bool getLastPolyOnArrow(const al::IUseCollision* user, const al::ArrowHitInfo** outHitInfo,
                        const sead::Vector3f& position, const sead::Vector3f& direction,
                        const al::CollisionPartsFilterBase* partsFilter,
                        const al::TriangleFilterBase* triFilter);

const al::CollisionParts*
getStrikeArrowCollisionParts(const al::IUseCollision* user, sead::Vector3f* outHitPos,
                             const sead::Vector3f& position, const sead::Vector3f& direction,
                             const al::CollisionPartsFilterBase* partsFilter,
                             const al::TriangleFilterBase* triFilter);
const al::HitSensor* tryGetStrikeArrowCollisionSensor(
    const al::IUseCollision* user, const sead::Vector3f& position, const sead::Vector3f& direction,
    const al::CollisionPartsFilterBase* partsFilter, const al::TriangleFilterBase* triFilter);

s32 getStrikeArrowInfoNum(const al::IUseCollision* user);

const al::SphereHitInfo& getStrikeSphereInfo(const al::IUseCollision* user, u32 index);
s32 getStrikeSphereInfoNum(const al::IUseCollision* user);

const sead::Vector3f& getStrikeSphereHitPos(const al::IUseCollision* user, u32 index);
const al::DiskHitInfo& getStrikeDiskInfo(const al::IUseCollision* user, u32 index);

s32 getStrikeDiskInfoNum(const al::IUseCollision* user);
const sead::Vector3f& getStrikeDiskHitPos(const al::IUseCollision* user, u32 index);

bool isInvalidParts(const al::CollisionParts& parts, const al::CollisionCheckInfoBase& checkInfo);
bool isFarAway(const al::CollisionParts& parts, const sead::Vector3f& position, f32 maxDistance);

s32 checkStrikeSphereMove(const al::IUseCollision* user, SphereMoveHitInfo* outHitInfos,
                          s32 hitNumMax, const sead::Vector3f& position, f32 radius,
                          const sead::Vector3f& moveDirection,
                          const al::CollisionPartsFilterBase* partsFilter,
                          const al::TriangleFilterBase* triFilter);

void searchCollisionParts(const al::IUseCollision* user, const sead::Vector3f& position, f32 radius,
                          al::CollisionPartsCallback& callback,
                          const al::CollisionPartsFilterBase* partsFilter);

void validateCollisionPartsPtrArray(const al::IUseCollision* user,
                                    sead::PtrArray<al::CollisionParts>* partsPtrArray);
void invalidateCollisionPartsPtrArray(const al::IUseCollision* user);
sead::PtrArray<al::CollisionParts>* getCollisionPartsPtrArray(const al::IUseCollision* user);

}  // namespace alCollisionUtil
