#pragma once

#include <math/seadVector.h>
#include "Library/Collision/KTriangle.h"
#include "math/seadBoundBox.h"

namespace al {
class CollisionPartsFilterBase;
class TriangleFilterBase;
class CollisionParts;
class IUseCollision;
class HitInfo;
class ArrowHitInfo;

bool isNearCollideSphereAabb(const sead::Vector3f&, f32, const sead::BoundBox3f&);

bool checkHitSegmentSphere(sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,float,sead::Vector3<float>*,sead::Vector3<float>*);

bool isFloorCode(const al::HitInfo*, const char*);
bool isFloorCode(const al::Triangle&, const char*);
bool isWallCode(const al::HitInfo*, const char*);

}  // namespace al

namespace alCollisionUtil {

bool isFarAway(const al::CollisionParts&, const sead::Vector3f&, f32);
bool checkStrikeArrow(al::IUseCollision const*,sead::Vector3<float> const&,sead::Vector3<float> const&,al::CollisionPartsFilterBase const*,al::TriangleFilterBase const*);
bool isCollisionMoving(const al::HitInfo*);
const sead::Vector3f& getCollisionHitPos(const al::HitInfo*);
const sead::Vector3f& getCollisionHitNormal(const al::HitInfo*);
const al::CollisionParts* getCollisionHitParts(const al::HitInfo*);
const sead::Vector3f& getCollisionMovingReaction(const al::HitInfo*);
bool getHitPosAndNormalOnArrow(al::IUseCollision const*,sead::Vector3<float> *,sead::Vector3<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&,al::CollisionPartsFilterBase const*,al::TriangleFilterBase const*);

bool getFirstPolyOnArrow(al::IUseCollision const*,al::ArrowHitInfo const**,sead::Vector3<float> const&,sead::Vector3<float> const&,al::CollisionPartsFilterBase const*,al::TriangleFilterBase const*);
bool getFirstPolyOnArrow(al::IUseCollision const*,sead::Vector3<float> *,al::Triangle *,sead::Vector3<float> const&,sead::Vector3<float> const&,char const*);
bool getFirstPolyOnArrow(al::IUseCollision const*,sead::Vector3<float> *,al::Triangle *,sead::Vector3<float> const&,sead::Vector3<float> const&,al::CollisionPartsFilterBase const*,al::TriangleFilterBase const*);

al::ArrowHitInfo* getStrikeArrowInfo(al::IUseCollision const*,u32);

bool getLastPolyOnArrow(const al::IUseCollision*, const al::ArrowHitInfo**, const sead::Vector3f&,
                        const sead::Vector3f&, const al::CollisionPartsFilterBase*,
                        const al::TriangleFilterBase*);

}
