#pragma once

#include <math/seadVector.h>

namespace al {

f32 getRandom();
f32 getRandom(f32);
f32 getRandom(f32, f32);
s32 getRandom(s32);
s32 getRandom(s32, s32);
f32 getRandomDegree();
f32 getRandomRadian();
void getRandomVector(sead::Vector3f*, f32);
void getRandomDir(sead::Vector3f*);
void getRandomDirH(sead::Vector3f*, const sead::Vector3f&);
void getRandomOnCircle(sead::Vector2f*, f32);
void getRandomInCircle(sead::Vector2f*, f32);
void getRandomInCircle(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void getRandomOnSphere(sead::Vector3f*, f32);
void getRandomInSphere(sead::Vector3f*, f32);
void calcRandomDirInCone(sead::Vector3f*, const sead::Vector3f&, f32);
void getRandomInSphereMinMaxRadius(sead::Vector3f*, f32, f32);
void initRandomSeed(u32);
void initRandomSeedByTick();
void initRandomSeedByString(const char*);
void getRandomContext(u32*, u32*, u32*, u32*);
void setRandomContext(u32, u32, u32, u32);
void makeRandomDirXZ(sead::Vector3f*);
void calcBoxMullerRandomGauss();
void makeBoxMullerRandomGauss(sead::Vector2f*, f32, f32);
void addRandomVector(sead::Vector3f*, const sead::Vector3f&, f32);
void turnRandomVector(sead::Vector3f*, const sead::Vector3f&, f32);

}  // namespace al
