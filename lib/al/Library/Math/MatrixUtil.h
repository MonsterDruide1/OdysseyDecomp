#pragma once

#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace nn::util::neon {
struct MatrixColumnMajor4x3fType;
struct MatrixColumnMajor4x4fType;
}  // namespace nn::util::neon

namespace al {

template <typename T>
struct Matrix43 {
    T m[3][4];
};

using Matrix43f = Matrix43<f32>;

void makeMtxRotateTrans(sead::Matrix34f* outMtx, const sead::Vector3f& rotate,
                        const sead::Vector3f& trans);
void makeMtxFromTwoAxis(sead::Matrix34f* outMtx, const sead::Vector3f& vectorA,
                        const sead::Vector3f& vectorB, s32 axisA, s32 axisB);
void makeMtxFrontUp(sead::Matrix34f* outMtx, const sead::Vector3f& front, const sead::Vector3f& up);
void makeMtxFrontSide(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                      const sead::Vector3f& side);
void makeMtxUpFront(sead::Matrix34f* outMtx, const sead::Vector3f& up, const sead::Vector3f& front);
void makeMtxUpSide(sead::Matrix34f* outMtx, const sead::Vector3f& up, const sead::Vector3f& side);
void makeMtxSideUp(sead::Matrix34f* outMtx, const sead::Vector3f& side, const sead::Vector3f& up);
void makeMtxSideFront(sead::Matrix34f* outMtx, const sead::Vector3f& side,
                      const sead::Vector3f& front);
void makeMtxFrontNoSupport(sead::Matrix34f* outMtx, const sead::Vector3f& front);
void makeMtxFrontNoSupportPos(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                              const sead::Vector3f& pos);
void makeMtxUpNoSupport(sead::Matrix34f* outMtx, const sead::Vector3f& up);
void makeMtxUpNoSupportPos(sead::Matrix34f* outMtx, const sead::Vector3f& up,
                           const sead::Vector3f& pos);
void makeMtxSideNoSupport(sead::Matrix34f* outMtx, const sead::Vector3f& side);
void makeMtxSideNoSupportPos(sead::Matrix34f* outMtx, const sead::Vector3f& side,
                             const sead::Vector3f& pos);
void makeMtxQuatPos(sead::Matrix34f* outMtx, const sead::Quatf& quat, const sead::Vector3f& pos);
void makeMtxQuatScalePos(sead::Matrix34f* outMtx, const sead::Quatf& quat,
                         const sead::Vector3f& scale, const sead::Vector3f& pos);
void makeMtxQuatScalePos(sead::Matrix44f* outMtx, const sead::Quatf& quat,
                         const sead::Vector3f& scale, const sead::Vector3f& pos);
void makeMtxFrontUpPos(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                       const sead::Vector3f& up, const sead::Vector3f& pos);
void makeMtxFrontSidePos(sead::Matrix34f* outMtx, const sead::Vector3f& front,
                         const sead::Vector3f& side, const sead::Vector3f& pos);
void makeMtxUpFrontPos(sead::Matrix34f* outMtx, const sead::Vector3f& up,
                       const sead::Vector3f& front, const sead::Vector3f& pos);
void makeMtxUpSidePos(sead::Matrix34f* outMtx, const sead::Vector3f& up, const sead::Vector3f& side,
                      const sead::Vector3f& pos);
void makeMtxSideUpPos(sead::Matrix34f* outMtx, const sead::Vector3f& side, const sead::Vector3f& up,
                      const sead::Vector3f& pos);
void makeMtxSideFrontPos(sead::Matrix34f* outMtx, const sead::Vector3f& side,
                         const sead::Vector3f& front, const sead::Vector3f& pos);
void makeMtxFollowTarget(sead::Matrix34f* outMtx, const sead::Matrix34f& baseMtx,
                         const sead::Vector3f& trans, const sead::Vector3f& rotate);
void makeMtxProj(sead::Matrix44f*, const sead::Vector2f&, const sead::Vector3f&,
                 const sead::Vector3f&);
void makeMtxProjFromQuatPoseUp(sead::Matrix44f*, const sead::Quatf&, const sead::Vector2f&,
                               const sead::Vector3f&);
void makeMtxProjFromQuatPoseFront(sead::Matrix44f*, const sead::Quatf&, const sead::Vector2f&,
                                  const sead::Vector3f&);
void makeMtxProjFromQuatPoseSide(sead::Matrix44f*, const sead::Quatf&, const sead::Vector2f&,
                                 const sead::Vector3f&);
void makeMtxProjFromUp(sead::Matrix44f*, const sead::Vector2f&, const sead::Vector3f&);

void rotateMtxXDirDegree(sead::Matrix34f* outMtx, const sead::Matrix34f& baseMtx, f32 angle);
void rotateMtxYDirDegree(sead::Matrix34f* outMtx, const sead::Matrix34f& baseMtx, f32 angle);
void rotateMtxZDirDegree(sead::Matrix34f* outMtx, const sead::Matrix34f& baseMtx, f32 angle);
void rotateMtxCenterPosXDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&,
                                  f32);
void rotateMtxCenterPosAxisDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&,
                                  const sead::Vector3f&, f32);
void rotateMtxCenterPosYDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&,
                                  f32);
void rotateMtxCenterPosZDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&,
                                  f32);
void rotateMtxCenterPosQuat(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&,
                            const sead::Quatf&);

bool turnMtxXDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);
bool turnMtxYDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);
bool turnMtxZDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);

void calcCameraPosFromViewMtx(sead::Vector3f*, const sead::Matrix34f&);
void calcMtxMul(sead::Vector3f*, const sead::Matrix34f&, const sead::Vector3f&);
void calcMtxMul(sead::Vector3f*, const Matrix43f&, const sead::Vector3f&);
void calcMtxScale(sead::Vector3f* outMtx, const sead::Matrix34f& mtx);
void calcMtxScale(sead::Vector3f* outMtx, const Matrix43f& mtx);
void normalizeMtxScale(sead::Matrix34f* outMtx, const sead::Matrix34f& mtx);
bool tryNormalizeMtxScaleOrIdentity(sead::Matrix34f* outMtx, const sead::Matrix34f& mtx);
void calcMtxLocalTrans(sead::Vector3f*, const sead::Matrix34f&, const sead::Vector3f&);
void calcTransLocalOffsetByMtx(sead::Vector3f*, const sead::Matrix34f&, const sead::Vector3f&);
void preScaleMtx(sead::Matrix34f* outMtx, const sead::Vector3f& scale);
void addTransMtxLocalOffset(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&);

void blendMtx(sead::Matrix34f*, const sead::Matrix34f&, const sead::Matrix34f&, f32);
void blendMtxRotate(sead::Matrix34f*, const sead::Matrix34f&, const sead::Matrix34f&, f32);
void blendMtxTrans(sead::Matrix34f*, const sead::Matrix34f&, const sead::Matrix34f&, f32);

f32 calcMtxLocalDirAngleOnPlaneToTarget(const sead::Matrix34f*, const sead::Vector3f&, s32, s32);
f32 calcMtxLocalDirAngleOnPlaneToDir(const sead::Matrix34f*, const sead::Vector3f&, s32, s32);
bool calcRotAxisOrZero(sead::Vector3f*, const sead::Matrix34f&);
void calcMxtInvertOrtho(sead::Matrix34f*, const sead::Matrix34f&);
void calcNearFarByInvProjection(f32*, f32*, const sead::Matrix44f&);
void calcMovedInertiaTensor(sead::Matrix33f*, const sead::Matrix33f&, const sead::Vector3f&, f32);
void calcInertiaTensorByMovedTensorAndCenter(sead::Matrix33f*, const sead::Matrix33f&,
                                             const sead::Vector3f&, f32);
void calcInertiaTensorSphere(sead::Matrix33f*, f32, f32);
void calcInertiaTensorBox(sead::Matrix33f*, const sead::Vector3f&, f32);

void makeMtx34f(sead::Matrix34f*, const nn::util::neon::MatrixColumnMajor4x3fType&);
void makeMtx44f(sead::Matrix44f*, const nn::util::neon::MatrixColumnMajor4x4fType&);

}  // namespace al
