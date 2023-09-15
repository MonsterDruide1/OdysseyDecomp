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

void makeMtxRotateTrans(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxFromTwoAxis(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, s32, s32);
void makeMtxFrontUp(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxFrontSide(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxUpFront(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxUpSide(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxSideUp(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxSideFront(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxFrontNoSupport(sead::Matrix34f*, const sead::Vector3f&);
void makeMtxFrontNoSupportPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxUpNoSupport(sead::Matrix34f*, const sead::Vector3f&);
void makeMtxUpNoSupportPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxSideNoSupport(sead::Matrix34f*, const sead::Vector3f&);
void makeMtxSideNoSupportPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxQuatPos(sead::Matrix34f*, const sead::Quatf&, const sead::Vector3f&);
void makeMtxQuatScalePos(sead::Matrix34f*, const sead::Quatf&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxQuatScalePos(sead::Matrix44f*, const sead::Quatf&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxFrontUpPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxFrontSidePos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxUpFrontPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxUpSidePos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxSideUpPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxSideFrontPos(sead::Matrix34f*, const sead::Vector3f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxFollowTarget(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxProj(sead::Matrix44f*, const sead::Vector2f&, const sead::Vector3f&, const sead::Vector3f&);
void makeMtxProjFromQuatPoseUp(sead::Matrix44f*, const sead::Quatf&, const sead::Vector2f&, const sead::Vector3f&);
void makeMtxProjFromQuatPoseFront(sead::Matrix44f*, const sead::Quatf&, const sead::Vector2f&, const sead::Vector3f&);
void makeMtxProjFromQuatPoseSide(sead::Matrix44f*, const sead::Quatf&, const sead::Vector2f&, const sead::Vector3f&);
void makeMtxProjFromUp(sead::Matrix44f*, const sead::Vector2f&, const sead::Vector3f&);

void rotateMtxXDirDegree(sead::Matrix34f*, const sead::Matrix34f&, f32);
void rotateMtxYDirDegree(sead::Matrix34f*, const sead::Matrix34f&, f32);
void rotateMtxZDirDegree(sead::Matrix34f*, const sead::Matrix34f&, f32);
void rotateMtxCenterPosXDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);
void rotateMtxCenterPosAxisDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, const sead::Vector3f&, f32);
void rotateMtxCenterPosYDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);
void rotateMtxCenterPosZDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);
void rotateMtxCenterPosQuat(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, const sead::Quatf&);

bool turnMtxXDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);
bool turnMtxYDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);
bool turnMtxZDirDegree(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&, f32);

void calcCameraPosFromViewMtx(sead::Vector3f*, const sead::Matrix34f&);
void calcMtxMul(sead::Vector3f*, const sead::Matrix34f&, const sead::Vector3f&);
void calcMtxMul(sead::Vector3f*, const Matrix43f&, const sead::Vector3f&);
void calcMtxScale(sead::Vector3f*, const sead::Matrix34f&);
void calcMtxScale(sead::Vector3f*, const Matrix43f&);
void normalizeMtxScale(sead::Matrix34f*, const sead::Matrix34f&);
bool tryNormalizeMtxScaleOrIdentity(sead::Matrix34f*, const sead::Matrix34f&);
void calcMtxLocalTrans(sead::Vector3f*, const sead::Matrix34f&, const sead::Vector3f&);
void calcTransLocalOffsetByMtx(sead::Vector3f*, const sead::Matrix34f&, const sead::Vector3f&);
void preScaleMtx(sead::Matrix34f*, const sead::Vector3f&);
void addTransMtxLocalOffset(sead::Matrix34f*, const sead::Matrix34f&, const sead::Vector3f&);

void blendMtx(sead::Matrix34f*, const sead::Matrix34f&, const sead::Matrix34f&, f32);
void blendMtxRotate(sead::Matrix34f*, const sead::Matrix34f&, const sead::Matrix34f&, f32);
void blendMtxTrans(sead::Matrix34f*, const sead::Matrix34f&, const sead::Matrix34f&, f32);

float calcMtxLocalDirAngleOnPlaneToTarget(const sead::Matrix34f*, const sead::Vector3f&, s32, s32);
float calcMtxLocalDirAngleOnPlaneToDir(const sead::Matrix34f*, const sead::Vector3f&, s32, s32);
bool calcRotAxisOrZero(sead::Vector3f*, const sead::Matrix34f&);
void calcMxtInvertOrtho(sead::Matrix34f*, const sead::Matrix34f&);
void calcNearFarByInvProjection(f32*, f32*, const sead::Matrix44f&);
void calcMovedInertiaTensor(sead::Matrix33f*, const sead::Matrix33f&, const sead::Vector3f&, f32);
void calcInertiaTensorByMovedTensorAndCenter(sead::Matrix33f*, const sead::Matrix33f&, const sead::Vector3f&, f32);
void calcInertiaTensorSphere(sead::Matrix33f*, f32, f32);
void calcInertiaTensorBox(sead::Matrix33f*, const sead::Vector3f&, f32);

void makeMtx34f(sead::Matrix34f*, const nn::util::neon::MatrixColumnMajor4x3fType&);
void makeMtx44f(sead::Matrix44f*, const nn::util::neon::MatrixColumnMajor4x4fType&);

}  // namespace al
