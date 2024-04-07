#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {

void separateScalarAndDirection(float*, sead::Vector2f*, const sead::Vector2f&);
void separateScalarAndDirection(float*, sead::Vector3f*, const sead::Vector3f&);
void limitVectorSeparateHV(sead::Vector3f*, const sead::Vector3f&, float, float);
void parallelizeVec(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void calcVectorSeparateHV(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float,
                          float);
void limitVectorParallelVertical(sead::Vector3f*, const sead::Vector3f&, float, float);
void separateVectorParallelVertical(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                                    const sead::Vector3f&);
void addVectorLimit(sead::Vector3f*, const sead::Vector3f&, float);
void alongVectorNormalH(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                        const sead::Vector3f&);
void calcDistanceVecToPlane(const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&,
                            const sead::Vector3f&);
void limitPlanePos(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                   const sead::Vector3f&);
void limitCylinderInPos(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                        const sead::Vector3f&, float);
void limitCylinderInDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                        const sead::Vector3f&, const sead::Vector3f&);
void limitCylinderInPos(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);
void limitCylinderInDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                        const sead::Vector3f&);
void roundOffVec(sead::Vector3f*, const sead::Vector3f&);
void roundOffVec(sead::Vector3f*);
void roundOffVec(sead::Vector2f*, const sead::Vector2f&);
void roundOffVec(sead::Vector2f*);
void snapToGrid(float, float, float);
void snapVecToGrid(sead::Vector3f*, const sead::Vector3f&, float, const sead::Vector3f&);
void snapVecToGrid(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                   const sead::Vector3f&);
void limitVectorOppositeDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);
void scaleVectorDirection(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&, float);
void scaleVectorExceptDirection(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                                float);
void snapVecToDirAxisY(sead::Vector3f*, const sead::Vector3f&, int);
void calcDir(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void calcDirH(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void calcDirOnPlane(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                    const sead::Vector3f&);
void mapRangeLogarithmic(float, float, float, float, float, float);
void calcDirFromLongitudeLatitude(sead::Vector3f*, float, float);
void calcLongitudeLatitudeFromDir(float*, float*, const sead::Vector3f&);
void getMaxAbsElementIndex(const sead::Vector3i&);
void getMaxAbsElementValue(const sead::Vector3f&);
void getMaxAbsElementValue(const sead::Vector3i&);
void getMinAbsElementIndex(const sead::Vector3f&);
void getMinAbsElementIndex(const sead::Vector3i&);
void getMinAbsElementValue(const sead::Vector3f&);
void getMinAbsElementValue(const sead::Vector3i&);
void calcNearVecFromAxis2(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&);
void calcNearVecFromAxis3(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&,
                          const sead::Vector3f&, const sead::Vector3f&);
void calcDirVerticalAny(sead::Vector3f*, const sead::Vector3f&);
void calcDirSlide(sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);
void calcNearVecFromAxis3(sead::Vector3f*, const sead::Vector3f&, const sead::Quatf&);
void calcQuatLocalAxisAll(const sead::Quatf&, sead::Vector3f*, sead::Vector3f*, sead::Vector3f*);
void addRandomVector(sead::Vector3f*, const sead::Vector3f&, float);
void turnRandomVector(sead::Vector3f*, const sead::Vector3f&, float);
void makeAxisFrontUp(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                     const sead::Vector3f&);
void makeAxisFrontSide(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                       const sead::Vector3f&);
void makeAxisUpFront(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&,
                     const sead::Vector3f&);
void makeAxisUpSide(sead::Vector3f*, sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&);

}  // namespace al
