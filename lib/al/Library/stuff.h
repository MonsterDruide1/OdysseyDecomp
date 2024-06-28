#pragma once

#include <math/seadVector.h>
#include "Library/LiveActor/LiveActor.h"
#include "Library/Player/PlayerHolder.h"
#include "Player/PlayerActorBase.h"
#include "math/seadQuat.h"

namespace al {
class MtxConnector;

s32 getPadAccelerationDeviceNum(s32);

void separateVelocityHV(sead::Vector3f*, sead::Vector3f*, al::LiveActor*);
void alongVectorNormalH(sead::Vector3f* a1, const sead::Vector3f& a2, const sead::Vector3f& a3,
                        const sead::Vector3f& a4);

bool isNearZero(const sead::Vector2f&, f32);

bool makeQuatRotationLimit(sead::Quatf*, sead::Vector3f const&, sead::Vector3f const&, float);
void makeQuatAxisRotation(sead::Quat<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,float);

s32 converge(s32, s32, s32);
f32 converge(f32, f32, f32);

bool isInRange(f32, f32, f32);

void calcDirVerticalAny(sead::Vector3f*, const sead::Vector3f&);

void makeQuatUpFront(sead::Quatf*, const sead::Vector3f&, const sead::Vector3f&);

bool isFloorPolygonCos(const sead::Vector3f&, const sead::Vector3f&, f32);
bool isFloorPolygon(const sead::Vector3f&, const sead::Vector3f&);

bool isInAreaObj(al::IUseAreaObj const*,char const*);
bool isInAreaObj(al::IUseAreaObj const*,char const*,sead::Vector3<float> const&);
al::PadRumbleKeeper* getPlayerPadRumbleKeeper(al::LiveActor const*,int);

al::MtxConnector* createCollisionPartsConnector(al::LiveActor const*,sead::Quat<float> const&);
void disconnectMtxConnector(al::MtxConnector*);

bool checkHitLinePlane(sead::Vector3<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&);

void followRotateFrontAxisUp(al::LiveActor *,al::CollisionParts const*);
bool turnVecToVecCosOnPlane(sead::Vector3<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,float);
bool turnVecToVecCosOnPlane(sead::Vector3<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&,float);
bool turnVecToVecCos(sead::Vector3<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&,float,sead::Vector3<float> const&,float);
bool addVectorLimit(sead::Vector3<float> *,sead::Vector3<float> const&,float);

bool turnQuatYDirRadian(sead::Quat<float> *,sead::Quat<float> const&,sead::Vector3<float> const&,float);
bool turnQuat(sead::Quat<float> *,sead::Quat<float> const&,sead::Vector3<float> const&,sead::Vector3<float> const&,float);
void turnRandomVector(sead::Vector3<float> *,sead::Vector3<float> const&,float);

void limitVectorOppositeDir(sead::Vector3<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&,float);
void calcDirSlide(sead::Vector3<float> *,sead::Vector3<float> const&,sead::Vector3<float> const&);

bool isWallPolygon(const sead::Vector3f&, const sead::Vector3f&);

bool isActiveCameraInterpole(al::IUseCamera const*,int);

al::LiveActor* getPlayerActor(al::LiveActor const*,int);

}  // namespace al

namespace alPadRumbleFunction {

void startPadRumbleLoopNo3D(al::LiveActor const*,char const*,sead::Vector3<float> const*,int);
void stopPadRumbleLoop(al::LiveActor const*,char const*,sead::Vector3<float> const*,int);

}
