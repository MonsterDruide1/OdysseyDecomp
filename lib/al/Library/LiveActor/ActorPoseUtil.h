#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;

void initActorPoseT(LiveActor* actor, const sead::Vector3f& trans);
void setTrans(LiveActor* actor, const sead::Vector3f& trans);
void initActorPoseTR(LiveActor* actor, const sead::Vector3f& trans, const sead::Vector3f& rotate);
void setRotate(LiveActor* actor, const sead::Vector3f& rotate);
void makeMtxSRT(sead::Matrix34f* mtx, const LiveActor* actor);
void makeMtxRT(sead::Matrix34f* mtx, const LiveActor* actor);
void makeMtxR(sead::Matrix34f* mtx, const LiveActor* actor);
void calcAnimFrontGravityPos(LiveActor* actor, const sead::Vector3f& front);
const sead::Vector3f& getGravity(const LiveActor* actor);
const sead::Vector3f& getTrans(const LiveActor* actor);
const sead::Vector3f& getScale(const LiveActor* actor);
void copyPose(LiveActor* actor, const LiveActor* target);
void updatePoseTrans(LiveActor* actor, const sead::Vector3f& trans);
void updatePoseRotate(LiveActor* actor, const sead::Vector3f& rotate);
void updatePoseQuat(LiveActor* actor, const sead::Quatf& quat);
void updatePoseMtx(LiveActor* actor, const sead::Matrix34f* mtx);
void calcSideDir(sead::Vector3f* side, const LiveActor* actor);
void calcLeftDir(sead::Vector3f* left, const LiveActor* actor);
void calcRightDir(sead::Vector3f* right, const LiveActor* actor);
void calcUpDir(sead::Vector3f* up, const LiveActor* actor);
void calcDownDir(sead::Vector3f* down, const LiveActor* actor);
void calcFrontDir(sead::Vector3f* front, const LiveActor* actor);
void calcBackDir(sead::Vector3f* back, const LiveActor* actor);
void calcPoseDir(sead::Vector3f* side, sead::Vector3f* up, sead::Vector3f* front,
                 const LiveActor* actor);
void calcQuat(sead::Quatf* quat, const LiveActor* actor);
f32 calcDistanceSignLocalXDir(const LiveActor* actor, const sead::Vector3f& pos);
f32 calcDistanceSignLocalYDir(const LiveActor* actor, const sead::Vector3f& pos);
f32 calcDistanceSignLocalZDir(const LiveActor* actor, const sead::Vector3f& pos);
f32 calcDistanceLocalXDir(const LiveActor* actor, const sead::Vector3f& pos);
f32 calcDistanceLocalYDir(const LiveActor* actor, const sead::Vector3f& pos);
f32 calcDistanceLocalZDir(const LiveActor* actor, const sead::Vector3f& pos);
sead::Vector3f* getTransPtr(LiveActor* actor);
void setTrans(LiveActor* actor, f32 x, f32 y, f32 z);
void setTransX(LiveActor* actor, f32 x);
void setTransY(LiveActor* actor, f32 y);
void setTransZ(LiveActor* actor, f32 z);
const sead::Vector3f& getRotate(const LiveActor* actor);
sead::Vector3f* getRotatePtr(LiveActor* actor);
void setRotate(LiveActor* actor, f32 x, f32 y, f32 z);
void setRotateX(LiveActor* actor, f32 x);
void setRotateY(LiveActor* actor, f32 y);
void setRotateZ(LiveActor* actor, f32 z);
sead::Vector3f* getScalePtr(LiveActor* actor);
sead::Vector3f* tryGetScalePtr(LiveActor* actor);
f32 getScaleX(const LiveActor* actor);
f32 getScaleY(const LiveActor* actor);
f32 getScaleZ(const LiveActor* actor);
void setScale(LiveActor* actor, const sead::Vector3f& scale);
void setScale(LiveActor* actor, f32 x, f32 y, f32 z);
void setScaleAll(LiveActor* actor, f32 scale);
void setScaleX(LiveActor* actor, f32 x);
void setScaleY(LiveActor* actor, f32 y);
void setScaleZ(LiveActor* actor, f32 z);
bool isFrontTarget(const LiveActor* actor, const sead::Vector3f& pos);
bool isFrontDir(const LiveActor* actor, const sead::Vector3f& dir);
bool isBackTarget(const LiveActor* actor, const sead::Vector3f& pos);
bool isBackDir(const LiveActor* actor, const sead::Vector3f& dir);
bool isLeftTarget(const LiveActor* actor, const sead::Vector3f& pos);
bool isLeftDir(const LiveActor* actor, const sead::Vector3f& dir);
bool isRightTarget(const LiveActor* actor, const sead::Vector3f& pos);
bool isRightDir(const LiveActor* actor, const sead::Vector3f& dir);
bool isUpTarget(const LiveActor* actor, const sead::Vector3f& pos);
bool isUpDir(const LiveActor* actor, const sead::Vector3f& dir);
bool isDownTarget(const LiveActor* actor, const sead::Vector3f& pos);
bool isDownDir(const LiveActor* actor, const sead::Vector3f& dir);
const sead::Quatf& getQuat(const LiveActor* actor);
sead::Quatf* getQuatPtr(LiveActor* actor);
sead::Quatf* tryGetQuatPtr(LiveActor* actor);
void setQuat(LiveActor* actor, const sead::Quatf& quat);
sead::Vector3f* getGravityPtr(const LiveActor* actor);
void setGravity(const LiveActor* actor, const sead::Vector3f& gravity);
const sead::Vector3f& getFront(const LiveActor* actor);
sead::Vector3f* getFrontPtr(LiveActor* actor);
void setFront(LiveActor* actor, const sead::Vector3f& front);
const sead::Vector3f& getUp(const LiveActor* actor);
sead::Vector3f* getUpPtr(LiveActor* actor);
void setUp(LiveActor* actor, const sead::Vector3f& up);
void multVecPoseNoTrans(sead::Vector3f* posOut, const LiveActor* actor,
                        const sead::Vector3f& posIn);
void multVecPose(sead::Vector3f* posOut, const LiveActor* actor, const sead::Vector3f& posIn);
void multVecPoseAndScale(sead::Vector3f* posOut, const LiveActor* actor,
                         const sead::Vector3f& posIn);
void multVecInvPose(sead::Vector3f* posOut, const LiveActor* actor, const sead::Vector3f& posIn);
void multVecInvQuat(sead::Vector3f* posOut, const LiveActor* actor, const sead::Vector3f& posIn);
void multMtxInvPose(sead::Matrix34f* mtxOut, const LiveActor* actor, const sead::Matrix34f& mtxIn);
void calcTransLocalOffset(sead::Vector3f* transOut, const LiveActor* actor,
                          const sead::Vector3f& transIn);

}  // namespace al

namespace alActorPoseFunction {

void calcBaseMtx(sead::Matrix34f* mtx, const al::LiveActor* actor);
void updatePoseTRMSV(al::LiveActor* actor);

}  // namespace alActorPoseFunction
