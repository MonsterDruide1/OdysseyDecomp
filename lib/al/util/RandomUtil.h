#pragma once

#include <math/seadVector.h>

namespace al {

float getRandom();
float getRandom(float);
float getRandom(float, float);
int getRandom(int);
int getRandom(int, int);
float getRandomDegree();
float getRandomRadian();
void getRandomVector(sead::Vector3f*, float);
void getRandomDir(sead::Vector3f*);
void getRandomDirH(sead::Vector3f*, const sead::Vector3f&);
void getRandomOnCircle(sead::Vector2f*, float);
void getRandomInCircle(sead::Vector2f*, float);
void getRandomInCircle(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);
void getRandomOnSphere(sead::Vector3f*, float);
void getRandomInSphere(sead::Vector3f*, float);
void calcRandomDirInCone(sead::Vector3f*, const sead::Vector3f&, float);
void getRandomInSphereMinMaxRadius(sead::Vector3f*, float, float);
void initRandomSeed(unsigned int);
void initRandomSeedByTick();
void initRandomSeedByString(const char*);
void getRandomContext(unsigned int*, unsigned int*, unsigned int*, unsigned int*);
void setRandomContext(unsigned int, unsigned int, unsigned int, unsigned int);
void makeRandomDirXZ(sead::Vector3f*);
void calcBoxMullerRandomGauss();
void makeBoxMullerRandomGauss(sead::Vector2f*, float, float);
void addRandomVector(sead::Vector3f*, const sead::Vector3f&, float);
void turnRandomVector(sead::Vector3f*, const sead::Vector3f&, float);

}  // namespace al
