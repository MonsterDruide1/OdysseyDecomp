#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace sead {
class Color4f;
}  // namespace sead

namespace al {
class VisitCellCallBack;
enum class Axis : s32;

template <typename T>
inline T clamp(T value, T min, T max) {
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

enum EaseType : s32 {
    EaseType_EaseIn = 1,
    EaseType_EaseOut,
    EaseType_EaseInOut,
    EaseType_SquareIn,
    EaseType_SquareOut
};

f32 calcAngleRadian(const sead::Vector3f& a, const sead::Vector3f& b);
f32 calcAngleDegree(const sead::Vector3f& a, const sead::Vector3f& b);
f32 calcAngleDegree(const sead::Vector2f& a, const sead::Vector2f& b);
bool isNearZero(const sead::Vector2f& vec, f32 tolerance = 0.001f);
bool tryCalcAngleDegree(f32* angle, const sead::Vector3f& a, const sead::Vector3f& b);
bool isNearZero(const sead::Vector3f& vec, f32 tolerance = 0.001f);
f32 calcAngleOnPlaneRadian(const sead::Vector3f& a, const sead::Vector3f& b,
                           const sead::Vector3f& vertical);
void verticalizeVec(sead::Vector3f* out, const sead::Vector3f& vertical, const sead::Vector3f& vec);
f32 calcAngleOnPlaneDegree(const sead::Vector3f& a, const sead::Vector3f& b,
                           const sead::Vector3f& vertical);
f32 calcAngleOnPlaneDegreeOrZero(const sead::Vector3f& a, const sead::Vector3f& b,
                                 const sead::Vector3f& vertical);
bool tryCalcAngleOnPlaneDegree(f32* angle, const sead::Vector3f& a, const sead::Vector3f& b,
                               const sead::Vector3f& vertical);
bool isParallelDirection(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance = 0.01f);
s32 calcAngleSignOnPlane(const sead::Vector3f& a, const sead::Vector3f& b,
                         const sead::Vector3f& vertical);
bool isNearAngleRadian(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance);
void normalize(sead::Vector2f* out, const sead::Vector2f& vec);
bool isNearAngleRadian(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance);
void normalize(sead::Vector3f* out, const sead::Vector3f& vec);
bool isNearAngleDegree(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance);
bool isNearAngleDegree(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance);
bool isNearAngleRadianHV(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                         f32);
bool tryNormalizeOrZero(sead::Vector3f* out, const sead::Vector3f& vec);
bool tryNormalizeOrZero(sead::Vector3f* out);
bool isNearAngleDegreeHV(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                         f32);
bool isInAngleOnPlaneDegreeHV(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                              f32, f32, f32, f32);
bool isNear(f32 value, f32 target, f32 tolerance = 0.001f);
bool isNear(const sead::Vector2f& value, const sead::Vector2f& target, f32 tolerance = 0.001f);
bool isNear(const sead::Vector3f& value, const sead::Vector3f& target, f32 tolerance = 0.001f);
bool isNear(const sead::Color4f& value, const sead::Color4f& target, f32 tolerance = 0.001f);
bool isNearZero(f32 value, f32 tolerance = 0.001f);
bool isNearZero(const sead::Matrix34f& value, f32 tolerance = 0.001f);
bool isNearZeroOrGreater(f32 value, f32 tolerance = 0.001f);
bool isNearZeroOrLess(f32 value, f32 tolerance = 0.001f);
bool isExistNearZeroVal(const sead::Vector3f& vec, f32 tolerance);
bool isNormalize(const sead::Vector3f& vec, f32 tolerance = 0.001f);
bool isNormalize(const sead::Matrix34f& mtx);
bool isParallelDirection(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance = 0.01f);
bool isReverseDirection(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance = 0.01f);
bool isNearDirection(const sead::Vector2f& a, const sead::Vector2f& b, f32 tolerance = 0.01f);
bool isNearDirection(const sead::Vector3f& a, const sead::Vector3f& b, f32 tolerance = 0.01f);
bool isInRange(s32 x, s32 a, s32 b);
bool isInRange(f32 x, f32 a, f32 b);
void normalize(sead::Vector2f* vec);
void normalize(sead::Vector3f* vec);
void normalize(sead::Matrix33f* mtx);
void normalize(sead::Matrix34f* mtx);
bool tryNormalizeOrZero(sead::Vector2f* vec);
bool tryNormalizeOrZero(sead::Vector2f* out, const sead::Vector2f& vec);
bool tryNormalizeOrDirZ(sead::Vector3f* vec);
bool tryNormalizeOrDirZ(sead::Vector3f* out, const sead::Vector3f& vec);
void normalizeComplement(sead::Matrix34f* mtx);
u32 getMaxAbsElementIndex(const sead::Vector3f& vec);
void setLength(sead::Vector3f* vec, f32 length);
void setProjectionLength(sead::Vector3f* out, const sead::Vector3f& vec, f32 length);
bool limitLength(sead::Vector2f* out, const sead::Vector2f& vec, f32 limit);
bool limitLength(sead::Vector3f* out, const sead::Vector3f& vec, f32 limit);
f32 normalizeAbs(f32 x, f32 min, f32 max);
f32 normalize(f32 x, f32 min, f32 max);
f32 normalize(s32 x, s32 min, s32 max);
f32 sign(f32 x);
s32 sign(s32 x);
f32 cubeRoot(f32 x);
void clampV3f(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void clampV2f(sead::Vector2f*, const sead::Vector2f&, const sead::Vector2f&);
f32 easeIn(f32 t);
f32 easeOut(f32 t);
f32 easeInOut(f32 t);
f32 squareIn(f32 t);
f32 squareOut(f32 t);
f32 powerIn(f32 t, f32 exp);
f32 powerOut(f32 t, f32 exp);
f32 logarithmIn(f32 t, f32 base);
f32 logarithmOut(f32 t, f32 base);
f32 exponentIn(f32 t, f32 exp);
f32 exponentOut(f32 t, f32 exp);
f32 hermiteRate(f32 t, f32 m0, f32 m1);
f32 calcFourthOrderRate(f32 t, f32 scale);
f32 calcTriangleWave01(f32, f32);
f32 calcTriangleWave(f32, f32, f32, f32);
f32 lerpValue(f32 a, f32 b, f32 t);
f32 calcRate01(f32, f32, f32);
f32 easeByType(f32 t, s32 easeType);
f32 lerpValue(f32, f32, f32, f32, f32);
f32 lerpDegree(f32, f32, f32);
f32 lerpRadian(f32, f32, f32);
void lerpVec(sead::Vector2f*, const sead::Vector2f&, const sead::Vector2f&, f32);
void lerpVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void lerpVecHV(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
               f32, f32);
void separateVectorHV(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                      const sead::Vector3f&);
void lerpColor(sead::Color4f*, const sead::Color4f&, const sead::Color4f&, f32);
f32 lerpLogValueEaseIn(f32, f32, f32, f32);
f32 lerpLogValueEaseOut(f32, f32, f32, f32);
void lerpLogVecEaseIn(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
void lerpLogVecEaseOut(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
f32 lerpExponentValueEaseIn(f32, f32, f32, f32);
f32 lerpExponentValueEaseOut(f32, f32, f32, f32);
void lerpExponentVecEaseIn(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
void lerpExponentVecEaseOut(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32,
                            f32);
f32 clampLeapMinAbs(f32, f32, f32, f32);
f32 hermite(f32 y0, f32 m0, f32 y1, f32 m1, f32 t);
f32 hermite(f32 y0, f32 m0, f32 y1, f32 m1, f32 t, f32 width);
void hermiteVec(sead::Vector3f* out, const sead::Vector3f& p0, const sead::Vector3f& v0,
                const sead::Vector3f& p1, const sead::Vector3f& v1, f32 t);
s32 converge(s32, s32, s32);
f32 converge(f32, f32, f32);
f32 convergeDegree(f32, f32, f32);
f32 convergeRadian(f32, f32, f32);
void convergeVec(sead::Vector2f*, const sead::Vector2f&, const sead::Vector2f&, f32);
void convergeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
f32 diffNearAngleDegree(f32, f32);
bool isInRangeAngleDegree(f32, f32, f32);
bool calcEyesAnimAngleInRange(f32*, const sead::Vector3f&, const sead::Vector3f&,
                              const sead::Vector3f&, const sead::Vector3f&, f32, f32, f32, f32);
bool isSameSign(f32, f32);
u8 reverseBit8(u8);
u16 reverseBit16(u16);
u32 reverseBit32(u32);
f32 calcVanDerCorput(u32);
void calcHammersleyPoint(sead::Vector2f*, u32, u32);
s32 findMaxFromArray(const s32*, s32);
void separateMinMax(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&);
s32 findMinFromArray(const s32*, s32);
f32 getRandom();
f32 getRandom(f32 max);
f32 getRandom(f32 min, f32 max);
s32 getRandom(s32 max);
s32 getRandom(s32 min, s32 max);
f32 getRandomDegree();
f32 getRandomRadian();
void getRandomVector(sead::Vector3f* vec, f32 maxComponent);
void getRandomDir(sead::Vector3f* vec);
void getRandomDirH(sead::Vector3f*, const sead::Vector3f&);
void rotateVectorDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void getRandomOnCircle(sead::Vector2f*, f32);
void getRandomInCircle(sead::Vector2f*, f32);
void getRandomInCircleMinMaxRadius(sead::Vector2f*, f32, f32);
void getRandomInCircle(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void getRandomOnSphere(sead::Vector3f*, f32);
void getRandomInSphere(sead::Vector3f*, f32);
bool calcRandomDirInCone(sead::Vector3f*, const sead::Vector3f&, f32);
void getRandomInSphereMinMaxRadius(sead::Vector3f*, f32, f32);
void initRandomSeed(u32);
void initRandomSeedByTick();
void initRandomSeedByString(const char*);
bool isHalfProbability();
bool isPercentProbability(f32);
void getRandomContext(u32*, u32*, u32*, u32*);
void setRandomContext(u32, u32, u32, u32);
void makeRandomDirXZ(sead::Vector3f*);
f32 calcBoxMullerRandomGauss();
void makeBoxMullerRandomGauss(sead::Vector2f*, f32, f32);
f32 modf(f32 a, f32 b);
s32 modi(s32 a, s32 b);
f32 calcSpeedMax(f32, f32);
f32 calcAccel(f32, f32);
f32 calcFriction(f32, f32);
bool separateScalarAndDirection(f32*, sead::Vector2f*, const sead::Vector2f&);
bool separateScalarAndDirection(f32*, sead::Vector3f*, const sead::Vector3f&);
void limitVectorSeparateHV(sead::Vector3f*, const sead::Vector3f&, f32, f32);
void parallelizeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void calcVectorSeparateHV(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32, f32);
void limitVectorParallelVertical(sead::Vector3f*, const sead::Vector3f&, f32, f32);
void separateVectorParallelVertical(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                                    const sead::Vector3f&);
bool addVectorLimit(sead::Vector3f*, const sead::Vector3f&, f32);
void alongVectorNormalH(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                        const sead::Vector3f&);
f32 calcDistanceVecToPlane(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                           const sead::Vector3f&);
void limitPlanePos(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                   const sead::Vector3f&);
bool limitCylinderInPos(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                        const sead::Vector3f&, f32);
bool limitCylinderInDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                        const sead::Vector3f&, const sead::Vector3f&);
bool limitCylinderInPos(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
bool limitCylinderInDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                        const sead::Vector3f&);
void roundOffVec(sead::Vector3f*, const sead::Vector3f&);
void roundOffVec(sead::Vector3f*);
void roundOffVec(sead::Vector2f*, const sead::Vector2f&);
void roundOffVec(sead::Vector2f*);
f32 snapToGrid(f32, f32, f32);
void snapVecToGrid(sead::Vector3f*, const sead::Vector3f&, f32, const sead::Vector3f&);
void snapVecToGrid(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                   const sead::Vector3f&);
void limitVectorOppositeDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void scaleVectorDirection(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void scaleVectorExceptDirection(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void snapVecToDirAxisY(sead::Vector3f*, const sead::Vector3f&, s32);
bool calcDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
bool calcDirH(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
bool calcDirOnPlane(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                    const sead::Vector3f&);
f32 mapRangeLogarithmic(f32, f32, f32, f32, f32, f32);
void calcDirFromLongitudeLatitude(sead::Vector3f*, f32, f32);
void calcLongitudeLatitudeFromDir(f32*, f32*, const sead::Vector3f&);

u32 getMaxAbsElementIndex(const sead::Vector3i&);
f32 getMaxAbsElementValue(const sead::Vector3f&);
s32 getMaxAbsElementValue(const sead::Vector3i&);
u32 getMinAbsElementIndex(const sead::Vector3f&);
u32 getMinAbsElementIndex(const sead::Vector3i&);
f32 getMinAbsElementValue(const sead::Vector3f&);
s32 getMinAbsElementValue(const sead::Vector3i&);

Axis calcNearVecFromAxis2(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&);
Axis calcNearVecFromAxis3(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&, const sead::Vector3f&);
void calcDirVerticalAny(sead::Vector3f*, const sead::Vector3f&);
void calcDirSlide(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
Axis calcNearVecFromAxis3(sead::Vector3f*, const sead::Vector3f&, const sead::Quatf&);
void calcQuatLocalAxisAll(const sead::Quatf&, sead::Vector3f*, sead::Vector3f*, sead::Vector3f*);
void addRandomVector(sead::Vector3f*, const sead::Vector3f&, f32);
void turnRandomVector(sead::Vector3f*, const sead::Vector3f&, f32);
void makeAxisFrontUp(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                     const sead::Vector3f&);
void makeAxisFrontSide(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                       const sead::Vector3f&);
void makeAxisUpFront(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                     const sead::Vector3f&);
void makeAxisUpSide(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void makeQuatFromTwoAxis(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&, s32, s32);
void makeQuatFrontUp(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);
void makeQuatFrontSide(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);
void makeQuatFrontNoSupport(sead::Quatf*, const sead::Vector3f&);
void makeQuatUpFront(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);
void makeQuatUpSide(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);
void makeQuatUpNoSupport(sead::Quatf*, const sead::Vector3f&);
void makeQuatSideUp(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);
void makeQuatSideFront(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);
void makeQuatSideNoSupport(sead::Quatf*, const sead::Vector3f&);
void makeQuatFromToQuat(sead::Quatf*, const sead::Quatf&, const sead::Quatf&);
void makeQuatRotationRate(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&, f32);
bool makeQuatRotationLimit(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&, f32);
void makeQuatAxisRotation(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&, f32);
void makeQuatRotateDegree(sead::Quatf*, const sead::Vector3f&, f32);
void slerpQuat(sead::Quatf*, const sead::Quatf&, const sead::Quatf&, f32);
void calcQuatSide(sead::Vector3f* out, const sead::Quatf& quat);
void calcQuatUp(sead::Vector3f* out, const sead::Quatf& quat);
void calcQuatGravity(sead::Vector3f* out, const sead::Quatf& quat);
void calcQuatFront(sead::Vector3f* out, const sead::Quatf& quat);
f32 calcQuatUpY(const sead::Quatf&);
f32 calcQuatFrontY(const sead::Quatf&);
void calcQuatLocalAxis(sead::Vector3f*, const sead::Quatf&, s32);
void calcQuatLocalSignAxis(sead::Vector3f*, const sead::Quatf&, s32);
void calcQuatRotateDegree(sead::Vector3f*, const sead::Quatf&);
void calcQuatRotateRadian(sead::Vector3f*, const sead::Quatf&);
void calcQuatRotateAxisAndDegree(sead::Vector3f*, f32*, const sead::Quatf&);
void calcQuatRotateAxisAndDegree(sead::Vector3f*, f32*, const sead::Quatf&, const sead::Quatf&);
void rotateQuatRadian(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
void makeQuatXDegree(sead::Quatf*, f32);
void makeQuatYDegree(sead::Quatf*, f32);
void makeQuatZDegree(sead::Quatf*, f32);
void rotateQuatXDirDegree(sead::Quatf*, const sead::Quatf&, f32);
void rotateQuatYDirDegree(sead::Quatf*, const sead::Quatf&, f32);
void rotateQuatZDirDegree(sead::Quatf*, const sead::Quatf&, f32);
void rotateQuatLocalDirDegree(sead::Quatf*, const sead::Quatf&, s32, f32);
void rotateQuatMoment(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&);
void rotateQuatMomentDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&);
void rotateQuatRollBall(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&,
                        const sead::Vector3f&, f32);
void calcMomentRollBall(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
bool turnQuat(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, const sead::Vector3f&, f32);
bool turnQuatXDirRadian(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatYDirRadian(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatZDirRadian(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
void turnQuatXDirRate(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
void turnQuatYDirRate(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
void turnQuatZDirRate(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
void tiltQuatDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, const sead::Vector3f&,
                    f32);
void tiltQuatXDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
void tiltQuatYDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
void tiltQuatZDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatWithAxisDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&,
                            const sead::Vector3f&, const sead::Vector3f&, f32);
bool turnQuatXDirWithYDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatXDirWithZDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatYDirWithZDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatYDirWithXDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatZDirWithXDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatZDirWithYDirDegree(sead::Quatf*, const sead::Quatf&, const sead::Vector3f&, f32);
bool turnQuatZDirToTargetWithAxis(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&, f32);
bool turnQuatFrontToDirDegreeH(sead::Quatf*, const sead::Vector3f&, f32);
void rotateQuatAndTransDegree(sead::Quatf*, sead::Vector3f*, const sead::Quatf&,
                              const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                              f32);
bool turnVecToVecDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void turnVecToVecRate(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
bool turnVecToVecCos(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32,
                     const sead::Vector3f&, f32);
bool turnVecToVecCosOnPlane(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                            const sead::Vector3f&, f32);
bool turnVecToVecCosOnPlane(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32);
void rotateVectorCenterDegree(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                              const sead::Vector3f&, f32);
void rotateVectorDegreeX(sead::Vector3f*, f32);
void rotateVectorDegreeY(sead::Vector3f*, f32);
void rotateVectorDegreeZ(sead::Vector3f*, f32);
void rotateVectorQuat(sead::Vector3f*, const sead::Quatf&);
f32 calcAreaTriangle(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void createBoundingBox(const sead::Vector3f*, u32, sead::Vector3f*, sead::Vector3f*);
void updateBoundingBox(sead::Vector3f, sead::Vector3f*, sead::Vector3f*);
f32 calcDistanceToFarthestBoundingBoxVertex(const sead::Vector3f&, const sead::Vector3f&,
                                            const sead::Vector3f&);
void calcSphereMargeSpheres(sead::Vector3f*, f32*, const sead::Vector3f&, f32,
                            const sead::Vector3f&, f32);
bool calcCrossLinePoint(sead::Vector2f*, const sead::Vector2f&, const sead::Vector2f&,
                        const sead::Vector2f&, const sead::Vector2f&);
f32 calcSquaredDistanceSegmentToSegment(const sead::Vector3f&, const sead::Vector3f&,
                                        const sead::Vector3f&, const sead::Vector3f&,
                                        sead::Vector3f*, sead::Vector3f*);
f32 calcSquaredDistancePointToSegment(const sead::Vector3f&, const sead::Vector3f&,
                                      const sead::Vector3f&);
f32 calcDistancePointToSegment(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void calcPerpendicFootToLineInside(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                                   const sead::Vector3f&);
void calcClosestSegmentPoint(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                             const sead::Vector3f&);
f32 calcCylinderRadiusDot(const sead::Vector3f&, const sead::Vector3f&, f32);
bool checkHitSemilinePlane(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                           const sead::Vector3f&, const sead::Vector3f&);
bool checkHitSegmentPlane(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&, const sead::Vector3f&, bool);
bool checkHitLinePlane(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                       const sead::Vector3f&, const sead::Vector3f&);
bool checkHitSegmentSphere(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                           sead::Vector3f*, sead::Vector3f*);
bool checkHitSegmentSphereNearDepth(const sead::Vector3f&, const sead::Vector3f&,
                                    const sead::Vector3f&, f32, sead::Vector3f*, sead::Vector3f*);
bool checkInCylinder(const sead::Vector3f&, const sead::Vector3f&, f32, const sead::Vector3f&, f32);
bool checkHitSegmentCylinder(const sead::Vector3f&, f32, const sead::Vector3f&, f32,
                             const sead::Vector3f&, const sead::Vector3f&, sead::Vector3f*,
                             sead::Vector3f*);
bool checkHitHalfLineSphere(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                            f32);

}  // namespace al

namespace Intersect {

bool calcX(sead::Vector3f*, f32, const sead::Vector3f&, const sead::Vector3f&,
           const sead::Vector3f&, const sead::Vector3f&);
bool calcY(sead::Vector3f*, f32, const sead::Vector3f&, const sead::Vector3f&,
           const sead::Vector3f&, const sead::Vector3f&);
bool calcZ(sead::Vector3f*, f32, const sead::Vector3f&, const sead::Vector3f&,
           const sead::Vector3f&, const sead::Vector3f&);

}  // namespace Intersect

namespace al {

bool checkHitSegmentBox(const sead::Vector3f&, const sead::Vector3f&, const sead::BoundBox3f&,
                        sead::Vector3f*);
bool checkHitPointCone(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&, f32,
                       f32);
bool checkHitSphereCone(const sead::Vector3f&, f32, const sead::Vector3f&, const sead::Vector3f&,
                        f32, f32);
bool checkHitSphereSpotLight(const sead::Vector3f&, f32, const sead::Vector3f&,
                             const sead::Vector3f&, f32, f32);
bool calcBoundingSphereSpotLight(sead::Vector3f*, f32*, const sead::Vector3f&,
                                 const sead::Vector3f&, f32, f32);
void calcBoundingSphereBox3f(sead::Vector3f*, f32*, const sead::BoundBox3f&);
void calcArrowAabb(sead::BoundBox3f*, const sead::Vector3f&, const sead::Vector3f&);
bool isNearCollideSphereAabb(const sead::Vector3f&, f32, const sead::BoundBox3f&);
void calcBoxFacePoint(sead::Vector3f*, const sead::BoundBox3f&, s32);
void calcBoxFacePoint(sead::Vector3f*, const sead::BoundBox3f&, s32, const sead::Matrix34f&);
void calcBoxFacePoint(sead::Vector3f*, const sead::BoundBox3f&, s32, const sead::Quatf&,
                      const sead::Vector3f&);
void calcFittingBoxPoseEqualAxisAll(sead::Quatf*, const sead::Quatf&, const sead::Quatf&);
void calcFittingBoxPoseEqualAxisNone(sead::Quatf*, const sead::Quatf&, const sead::Quatf&);
void calcFittingBoxPoseEqualAxisTwo(sead::Quatf*, const sead::Quatf&, const sead::Quatf&, s32);
void calcFittingBoxPose(sead::Quatf*, const sead::BoundBox3f&, const sead::Quatf&,
                        const sead::Quatf&);
f32 calcSquaredDistanceToObb(const sead::Vector3f&, const sead::Matrix34f&, const sead::Vector3f&);
f32 calcDistanceToObb(const sead::Vector3f&, const sead::Matrix34f&, const sead::Vector3f&);
f32 calcSquaredDistanceToObb(const sead::Vector3f&, const sead::Matrix34f&, const sead::Vector3f&,
                             const sead::BoundBox3f&);
f32 calcDistanceToObb(const sead::Vector3f&, const sead::Matrix34f&, const sead::Vector3f&,
                      const sead::BoundBox3f&);
f32 calcSquaredDistanceToObb(const sead::Vector3f&, const sead::Matrix34f&);
f32 calcDistanceToObb(const sead::Vector3f&, const sead::Matrix34f&);
void calcObbCorners(sead::Vector3f*, const sead::Matrix34f&, const sead::BoundBox3f&);
bool calcTriangleInteriorAngleDegree(f32*, f32*, f32*, f32, f32, f32);
bool calcBetweenTwoLinkMtx(sead::Matrix34f*, sead::Matrix34f*, sead::Matrix34f*,
                           const sead::Vector3f&, const sead::Vector3f&, f32, f32);
bool calcBetweenTwoLinkPos(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, f32, f32,
                           const sead::Vector3f&);
bool calcReflectionVector(sead::Vector3f*, const sead::Vector3f&, f32, f32);
void calcReverseVector(sead::Vector3f*, const sead::Vector3f&, f32);
void calcParabolicFunctionParam(f32* gravity, f32* initialVelY, f32 maxHeight,
                                f32 verticalDistance);
f32 calcConvergeVibrationValue(f32, f32, f32, f32, f32);
bool calcSphericalPolarCoordPY(sead::Vector2f*, const sead::Vector3f&, const sead::Vector3f&,
                               const sead::Vector3f&);
void calcBezierPoint(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                     const sead::Vector3f&, const sead::Vector3f&, f32);
f32 calcSpringDumperForce(f32, f32, f32, f32);
f32 convertSpringEnergyToSpeed(f32, f32, f32);
const char* axisIndexToString(s32);
void visitCellsOverlapped(const sead::Vector3f&, const sead::Vector3f&, f32,
                          const VisitCellCallBack&);
f32 calcMultValueToDestination(u32, f32, f32);
f32 getHaltonSequence(u32, u32);
f32 calcFractal(f32, f32, u32, f32, f32, f32, bool);
f32 calcMultiFractal(f32, f32, f32, u32, f32, f32, f32, bool);
f32 calcNormalDistribution(f32, f32, f32);
bool calcVecViewInput(sead::Vector3f*, const sead::Vector2f&, const sead::Vector3f&,
                      const sead::Matrix34f*);
bool calcDirViewInput(sead::Vector3f*, const sead::Vector2f&, const sead::Vector3f&,
                      const sead::Matrix34f*);
bool calcVecViewInput2D(sead::Vector3f*, const sead::Vector2f&, const sead::Vector3f&,
                        const sead::Matrix34f*);
bool calcDirViewInput2D(sead::Vector3f*, const sead::Vector2f&, const sead::Vector3f&,
                        const sead::Matrix34f*);
void calcBendPosAndFront(sead::Vector3f*, sead::Vector3f*, sead::Vector3f&, f32,
                         const sead::Vector3f&, const sead::Vector3f&, f32);
void calcCirclePointPicking(sead::Vector2f*, f32, f32);
void pickUniformPointsOnCircleHammersley(sead::Vector2f*, u32, u32);
void calcDiskPointPicking(sead::Vector2f*, f32, f32);
void pickUniformPointsOnDiskHammersley(sead::Vector2f*, u32, u32);
void pickUniformPointOnDisk(sead::Vector2f*);
void calcSpherePointPicking(sead::Vector3f*, f32, f32);
void pickUniformPointOnSphere(sead::Vector3f*);
void makeBayerMatrix(s32*, s32);
u16 f32ToF16(f32);
f32 f16ToF32(u16);

}  // namespace al
