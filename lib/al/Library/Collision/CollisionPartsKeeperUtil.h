#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>
#include <math/seadVector.h>

#include "Library/Collision/CollisionPartsTriangle.h"

namespace sead {
template <typename>
class IDelegate1;
}

namespace al {
struct ArrowHitInfo;
class CollisionPartsFilterBase;
class CollisionParts;
class CollisionCheckInfoBase;
struct DiskHitInfo;
struct HitInfo;
class HitSensor;
class ICollisionPartsKeeper;
class IUseCollision;
class LiveActor;
struct SphereHitInfo;
class Triangle;
class TriangleFilterBase;

const char* getMaterialCodeName(const Triangle&);
const char* getCollisionCodeName(const Triangle&, const char*);
const char* getFloorCodeName(const Triangle&);
const char* getWallCodeName(const Triangle&);
const char* getCameraCodeName(const Triangle&);
const char* getMaterialCodeName(const HitInfo*);
const char* getFloorCodeName(const HitInfo*);
const char* getWallCodeName(const HitInfo*);
const char* getCameraCodeName(const HitInfo*);
const char* getCollisionCodeName(const HitInfo*, const char*);
bool isMaterialCode(const Triangle&, const char*);
bool isFloorCode(const Triangle&, const char*);
bool isWallCode(const Triangle&, const char*);
bool isCameraCode(const Triangle&, const char*);
bool isCollisionCode(const Triangle&, const char*, const char*);
bool isMaterialCode(const HitInfo*, const char*);
bool isFloorCode(const Triangle&, const char*);
bool isWallCode(const HitInfo*, const char*);
bool isCameraCode(const HitInfo*, const char*);
bool isCollisionCode(const HitInfo*, const char*, const char*);
const char* getSpecialPurposeName(const HitInfo*);
bool isCollisionValid(const CollisionParts*);
bool isCollisionMoving(const CollisionParts*);
bool calcForceMovePower(sead::Vector3f*, const CollisionParts*, const sead::Vector3f&);
bool calcForceMovePowerExceptNormal(sead::Vector3f*, const CollisionParts*, const sead::Vector3f&,
                                    const sead::Vector3f&);
}  // namespace al

namespace alCollisionUtil {
struct SphereMoveHitInfo {
    f32 _0;
    f32 _4;
    sead::Vector3f _8;
    al::Triangle triangle;

    static bool compare(const SphereMoveHitInfo& a1, const SphereMoveHitInfo& a2) {
        return a1._0 < a2._0;
    }
};

static_assert(sizeof(SphereMoveHitInfo) == 0x88);

al::ICollisionPartsKeeper* getCollisionPartsKeeper(const al::IUseCollision*);
bool isCollisionMoving(const al::HitInfo*);
const sead::Vector3f& getCollisionHitPos(const al::HitInfo*);
const sead::Vector3f& getCollisionHitNormal(const al::HitInfo*);
const sead::Vector3f& getCollisionMovingReaction(const al::HitInfo*);
al::HitSensor* getCollisionHitSensor(const al::HitInfo*);
al::CollisionParts* getCollisionHitParts(const al::HitInfo*);
al::LiveActor* getCollisionHitActor(const al::HitInfo*);
s32 checkStrikeSphere(const al::IUseCollision*, const sead::Vector3f&, f32,
                      const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
s32 checkStrikeSphereMovingReaction(const al::IUseCollision*, const sead::Vector3f&, f32,
                                    const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                                    const al::TriangleFilterBase*);
s32 checkStrikeDisk(const al::IUseCollision*, const sead::Vector3f&, f32, f32,
                    const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                    const al::TriangleFilterBase*);
s32 checkStrikeArrow(const al::IUseCollision*, const sead::Vector3f& origin,
                     const sead::Vector3f& delta, const al::CollisionPartsFilterBase*,
                     const al::TriangleFilterBase*);
s32 checkStrikeSphereForPlayer(const al::IUseCollision*, const sead::Vector3f&, f32,
                               const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);

bool getFirstPolyOnArrow(const al::IUseCollision*, const al::ArrowHitInfo**, const sead::Vector3f&,
                         const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                         const al::TriangleFilterBase*);
const al::ArrowHitInfo& getStrikeArrowInfo(const al::IUseCollision*, u32);
bool getFirstPolyOnArrow(const al::IUseCollision*, sead::Vector3f*, al::Triangle*,
                         const sead::Vector3f&, const sead::Vector3f&, const char*);
bool getFirstPolyOnArrow(const al::IUseCollision*, sead::Vector3f*, al::Triangle*,
                         const sead::Vector3f&, const sead::Vector3f&,
                         const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
bool getHitPosOnArrow(const al::IUseCollision*, sead::Vector3f*, const sead::Vector3f&,
                      const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                      const al::TriangleFilterBase*);
bool getHitPosAndNormalOnArrow(const al::IUseCollision*, sead::Vector3f*, sead::Vector3f*,
                               const sead::Vector3f&, const sead::Vector3f&,
                               const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
bool getHitPosAndNormalAndSensorOnArrow(const al::IUseCollision*, sead::Vector3f*, sead::Vector3f*,
                                        al::HitSensor**, const sead::Vector3f&,
                                        const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                                        const al::TriangleFilterBase*);
bool getFirstCollisionSensorOnArrow(const al::IUseCollision*, sead::Vector3f*, al::HitSensor**,
                                    const sead::Vector3f&, const sead::Vector3f&,
                                    const al::CollisionPartsFilterBase*,
                                    const al::TriangleFilterBase*);
bool getLastCollisionSensorOnArrow(const al::IUseCollision*, sead::Vector3f*, al::HitSensor**,
                                   const sead::Vector3f&, const sead::Vector3f&,
                                   const al::CollisionPartsFilterBase*,
                                   const al::TriangleFilterBase*);
bool getLastPolyOnArrow(const al::IUseCollision*, sead::Vector3f*, al::Triangle*,
                        const sead::Vector3f&, const sead::Vector3f&,
                        const al::CollisionPartsFilterBase*, const al::TriangleFilterBase*);
bool getLastPolyOnArrow(const al::IUseCollision*, const al::ArrowHitInfo**, const sead::Vector3f&,
                        const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                        const al::TriangleFilterBase*);
al::CollisionParts* getStrikeArrowCollisionParts(const al::IUseCollision*, sead::Vector3f*,
                                                 const sead::Vector3f&, const sead::Vector3f&,
                                                 const al::CollisionPartsFilterBase*,
                                                 const al::TriangleFilterBase*);
al::HitSensor* tryGetStrikeArrowCollisionSensor(const al::IUseCollision*, const sead::Vector3f&,
                                                const sead::Vector3f&,
                                                const al::CollisionPartsFilterBase*,
                                                const al::TriangleFilterBase*);
s32 getStrikeArrowInfoNum(const al::IUseCollision*);
const al::SphereHitInfo& getStrikeSphereInfo(const al::IUseCollision*, u32);
s32 getStrikeSphereInfoNum(const al::IUseCollision*);
const sead::Vector3f& getStrikeSphereHitPos(const al::IUseCollision*, u32);
const al::DiskHitInfo& getStrikeDiskInfo(const al::IUseCollision*, u32);
s32 getStrikeDiskInfoNum(const al::IUseCollision*);
const sead::Vector3f& getStrikeDiskHitPos(const al::IUseCollision*, u32);
bool isInvalidParts(const al::CollisionParts&, const al::CollisionCheckInfoBase&);
bool isFarAway(const al::CollisionParts&, const sead::Vector3f&, f32);
s32 checkStrikeSphereMove(const al::IUseCollision*, SphereMoveHitInfo*, s32, const sead::Vector3f&,
                          f32, const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                          const al::TriangleFilterBase*);
void searchCollisionParts(const al::IUseCollision*, const sead::Vector3f&, f32,
                          sead::IDelegate1<al::CollisionParts*>&,
                          const al::CollisionPartsFilterBase*);
void validateCollisionPartsPtrArray(const al::IUseCollision*, sead::PtrArray<al::CollisionParts>*);
void invalidateCollisionPartsPtrArray(const al::IUseCollision*);
sead::PtrArray<al::CollisionParts>* getCollisionPartsPtrArray(const al::IUseCollision*);

}  // namespace alCollisionUtil
