#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

class Collider2D3D;
class IUsePlayerCollision;

namespace al {
class HitSensor;
class LiveActor;
}  // namespace al

namespace rs {

Collider2D3D* createCollider2D3D(al::LiveActor*, f32, f32, const sead::Vector3f&, f32);
void updateCollider2D3D(al::LiveActor*, Collider2D3D*);
void collide2D3D(sead::Vector3f*, const al::LiveActor*, Collider2D3D*);
void resetCollider(Collider2D3D*);
void onCollide2D(Collider2D3D*);
void onCollide3D(Collider2D3D*);
bool isValidCollide2D(const Collider2D3D*);
f32 getColliderHalfThickness2D(const Collider2D3D*);
bool isCollided(const Collider2D3D*);
bool isCollidedGround(const Collider2D3D*);
bool isCollidedGroundVelocity(const al::LiveActor*, const Collider2D3D*);
bool isCollidedWall(const Collider2D3D*);
bool isCollidedWallVelocity(const al::LiveActor*, const Collider2D3D*);
bool isCollidedCeiling(const Collider2D3D*);
bool isCollidedCeilingVelocity(const al::LiveActor*, const Collider2D3D*);
const sead::Vector3f& getCollidedGroundNormal(const Collider2D3D*);
const sead::Vector3f& getCollidedWallNormal(const Collider2D3D*);
const sead::Vector3f& getCollidedCeilingNormal(const Collider2D3D*);
const sead::Vector3f& getCollidedGroundPos(const Collider2D3D*);
const sead::Vector3f& getCollidedWallPos(const Collider2D3D*);
const sead::Vector3f& getCollidedCeilingPos(const Collider2D3D*);
al::HitSensor* tryGetCollidedGroundSensor(const Collider2D3D*);
al::HitSensor* tryGetCollidedWallSensor(const Collider2D3D*);
al::HitSensor* tryGetCollidedCeilingSensor(const Collider2D3D*);
bool isCollidedDamageFire(const Collider2D3D*);
bool isCollidedPoison(const Collider2D3D*);
bool tryKillIfCollidedDamageFloor(al::LiveActor*, const Collider2D3D*, const char*);
bool trySlideIfOnFloorSlide(al::LiveActor*, f32, f32);
bool trySlideIfOnFloorSlide(al::LiveActor*, const IUsePlayerCollision*, f32, f32);
bool reboundVelocityFromCollision(al::LiveActor*, const Collider2D3D*, f32, f32, f32);
void solveCollisionInHacking(al::LiveActor*, const sead::Vector3f&);

}  // namespace rs
