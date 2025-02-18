#include "Library/LiveActor/ActorPoseUtil.h"

#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"

namespace al {
class LiveActor;

void initActorPoseT(LiveActor* actor, const sead::Vector3f& trans) {
    actor->initPoseKeeper(new ActorPoseKeeperTRSV());
    setTrans(actor, trans);
}

void setTrans(LiveActor* actor, const sead::Vector3f& trans) {
    *getTransPtr(actor) = trans;
}

void initActorPoseTR(LiveActor* actor, const sead::Vector3f& trans, const sead::Vector3f& rotate) {
    actor->initPoseKeeper(new ActorPoseKeeperTRSV());
    setTrans(actor, trans);
    setRotate(actor, rotate);
}

void setRotate(LiveActor* actor, const sead::Vector3f& rotate) {
    getRotatePtr(actor)->set(rotate);
}

void makeMtxSRT(sead::Matrix34f* mtx, const LiveActor* actor) {
    ActorPoseKeeperBase* poseKeeper = actor->getPoseKeeper();
    poseKeeper->calcBaseMtx(mtx);
    preScaleMtx(mtx, poseKeeper->getScale());
}

void makeMtxRT(sead::Matrix34f* mtx, const LiveActor* actor) {
    actor->getPoseKeeper()->calcBaseMtx(mtx);
}

void makeMtxR(sead::Matrix34f* mtx, const LiveActor* actor) {
    makeMtxRT(mtx, actor);
    mtx->setBase(3, sead::Vector3f::zero);
}

void calcAnimFrontGravityPos(LiveActor* actor, const sead::Vector3f& front) {
    sead::Vector3f up = -getGravity(actor);
    if (actor->getBaseMtx()) {
        const sead::Matrix34f* mtx = actor->getBaseMtx();
        sead::Vector3f side;
        side.setCross(front, up);
        if (isNearZero(side, 0.001f)) {
            sead::Vector3f side;
            mtx->getBase(side, 0);
            up.setCross(front, side);
        }
    }

    sead::Matrix34f mtx;
    makeMtxFrontUpPos(&mtx, front, up, getTrans(actor));
    setBaseMtxAndCalcAnim(actor, mtx, getScale(actor));
}

const sead::Vector3f& getGravity(const LiveActor* actor) {
    return actor->getPoseKeeper()->getGravity();
}

const sead::Vector3f& getTrans(const LiveActor* actor) {
    return actor->getPoseKeeper()->getTrans();
}

const sead::Vector3f& getScale(const LiveActor* actor) {
    return actor->getPoseKeeper()->getScale();
}

void copyPose(LiveActor* actor, const LiveActor* target) {
    actor->getPoseKeeper()->copyPose(target->getPoseKeeper());
}

void updatePoseTrans(LiveActor* actor, const sead::Vector3f& trans) {
    actor->getPoseKeeper()->updatePoseTrans(trans);
}

void updatePoseRotate(LiveActor* actor, const sead::Vector3f& rotate) {
    actor->getPoseKeeper()->updatePoseRotate(rotate);
}

void updatePoseQuat(LiveActor* actor, const sead::Quatf& quat) {
    actor->getPoseKeeper()->updatePoseQuat(quat);
}

void updatePoseMtx(LiveActor* actor, const sead::Matrix34f* mtx) {
    actor->getPoseKeeper()->updatePoseMtx(mtx);
}

void calcSideDir(sead::Vector3f* side, const LiveActor* actor) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    mtx.getBase(*side, 0);
}

void calcLeftDir(sead::Vector3f* left, const LiveActor* actor) {
    return calcSideDir(left, actor);
}

void calcRightDir(sead::Vector3f* right, const LiveActor* actor) {
    sead::Vector3f left;
    calcLeftDir(&left, actor);
    right->x = -left.x;
    right->y = -left.y;
    right->z = -left.z;
}

void calcUpDir(sead::Vector3f* up, const LiveActor* actor) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    mtx.getBase(*up, 1);
}

void calcDownDir(sead::Vector3f* down, const LiveActor* actor) {
    sead::Vector3f up;
    calcUpDir(&up, actor);
    down->x = -up.x;
    down->y = -up.y;
    down->z = -up.z;
}

void calcFrontDir(sead::Vector3f* front, const LiveActor* actor) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    mtx.getBase(*front, 2);
}

void calcBackDir(sead::Vector3f* back, const LiveActor* actor) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    back->x = -front.x;
    back->y = -front.y;
    back->z = -front.z;
}

void calcPoseDir(sead::Vector3f* side, sead::Vector3f* up, sead::Vector3f* front,
                 const LiveActor* actor) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    if (side)
        mtx.getBase(*side, 0);
    if (up)
        mtx.getBase(*up, 1);
    if (front)
        mtx.getBase(*front, 2);
}

void calcQuat(sead::Quatf* quat, const LiveActor* actor) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    mtx.toQuat(*quat);
}

f32 calcDistanceSignLocalXDir(const LiveActor* actor, const sead::Vector3f& pos) {
    sead::Vector3f left;
    calcLeftDir(&left, actor);
    return left.dot(pos - getTrans(actor));
}

f32 calcDistanceSignLocalYDir(const LiveActor* actor, const sead::Vector3f& pos) {
    sead::Vector3f up;
    calcUpDir(&up, actor);
    return up.dot(pos - getTrans(actor));
}

f32 calcDistanceSignLocalZDir(const LiveActor* actor, const sead::Vector3f& pos) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    return front.dot(pos - getTrans(actor));
}

f32 calcDistanceLocalXDir(const LiveActor* actor, const sead::Vector3f& pos) {
    return sead::Mathf::abs(calcDistanceSignLocalXDir(actor, pos));
}

f32 calcDistanceLocalYDir(const LiveActor* actor, const sead::Vector3f& pos) {
    return sead::Mathf::abs(calcDistanceSignLocalYDir(actor, pos));
}

f32 calcDistanceLocalZDir(const LiveActor* actor, const sead::Vector3f& pos) {
    return sead::Mathf::abs(calcDistanceSignLocalZDir(actor, pos));
}

sead::Vector3f* getTransPtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getTransPtr();
}

void setTrans(LiveActor* actor, f32 x, f32 y, f32 z) {
    setTrans(actor, {x, y, z});
}

void setTransX(LiveActor* actor, f32 x) {
    getTransPtr(actor)->x = x;
}

void setTransY(LiveActor* actor, f32 y) {
    getTransPtr(actor)->y = y;
}

void setTransZ(LiveActor* actor, f32 z) {
    getTransPtr(actor)->z = z;
}

const sead::Vector3f& getRotate(const LiveActor* actor) {
    return actor->getPoseKeeper()->getRotate();
}

sead::Vector3f* getRotatePtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getRotatePtr();
}

void setRotate(LiveActor* actor, f32 x, f32 y, f32 z) {
    setRotate(actor, {x, y, z});
}

void setRotateX(LiveActor* actor, f32 x) {
    getRotatePtr(actor)->x = x;
}

void setRotateY(LiveActor* actor, f32 y) {
    getRotatePtr(actor)->y = y;
}

void setRotateZ(LiveActor* actor, f32 z) {
    getRotatePtr(actor)->z = z;
}

sead::Vector3f* getScalePtr(LiveActor* actor) {
    return tryGetScalePtr(actor);
}

sead::Vector3f* tryGetScalePtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getScalePtr();
}

f32 getScaleX(const LiveActor* actor) {
    return getScale(actor).x;
}

f32 getScaleY(const LiveActor* actor) {
    return getScale(actor).y;
}

f32 getScaleZ(const LiveActor* actor) {
    return getScale(actor).z;
}

void setScale(LiveActor* actor, const sead::Vector3f& scale) {
    *getScalePtr(actor) = scale;
}

void setScale(LiveActor* actor, f32 x, f32 y, f32 z) {
    setScale(actor, {x, y, z});
}

void setScaleAll(LiveActor* actor, f32 scale) {
    setScale(actor, {scale, scale, scale});
}

void setScaleX(LiveActor* actor, f32 x) {
    getScalePtr(actor)->x = x;
}

void setScaleY(LiveActor* actor, f32 y) {
    getScalePtr(actor)->y = y;
}

void setScaleZ(LiveActor* actor, f32 z) {
    getScalePtr(actor)->z = z;
}

bool isFrontTarget(const LiveActor* actor, const sead::Vector3f& pos) {
    return isFrontDir(actor, pos - getTrans(actor));
}

bool isFrontDir(const LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f front;
    calcFrontDir(&front, actor);
    return front.dot(dir) > 0.0f;
}

bool isBackTarget(const LiveActor* actor, const sead::Vector3f& pos) {
    return isBackDir(actor, pos - getTrans(actor));
}

bool isBackDir(const LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f back;
    calcBackDir(&back, actor);
    return back.dot(dir) > 0.0f;
}

bool isLeftTarget(const LiveActor* actor, const sead::Vector3f& pos) {
    return isLeftDir(actor, pos - getTrans(actor));
}

bool isLeftDir(const LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f left;
    calcLeftDir(&left, actor);
    return left.dot(dir) > 0.0f;
}

bool isRightTarget(const LiveActor* actor, const sead::Vector3f& pos) {
    return isRightDir(actor, pos - getTrans(actor));
}

bool isRightDir(const LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f right;
    calcRightDir(&right, actor);
    return right.dot(dir) > 0.0f;
}

bool isUpTarget(const LiveActor* actor, const sead::Vector3f& pos) {
    return isUpDir(actor, pos - getTrans(actor));
}

bool isUpDir(const LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f up;
    calcUpDir(&up, actor);
    return up.dot(dir) > 0.0f;
}

bool isDownTarget(const LiveActor* actor, const sead::Vector3f& pos) {
    return isDownDir(actor, pos - getTrans(actor));
}

bool isDownDir(const LiveActor* actor, const sead::Vector3f& dir) {
    sead::Vector3f down;
    calcDownDir(&down, actor);
    return down.dot(dir) > 0.0f;
}

const sead::Quatf& getQuat(const LiveActor* actor) {
    return actor->getPoseKeeper()->getQuat();
}

sead::Quatf* getQuatPtr(LiveActor* actor) {
    return tryGetQuatPtr(actor);
}

sead::Quatf* tryGetQuatPtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getQuatPtr();
}

void setQuat(LiveActor* actor, const sead::Quatf& quat) {
    getQuatPtr(actor)->set(quat);
}

sead::Vector3f* getGravityPtr(const LiveActor* actor) {
    return actor->getPoseKeeper()->getGravityPtr();
}

void setGravity(const LiveActor* actor, const sead::Vector3f& gravity) {
    getGravityPtr(actor)->set(gravity);
}

const sead::Vector3f& getFront(const LiveActor* actor) {
    return actor->getPoseKeeper()->getFront();
}

sead::Vector3f* getFrontPtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getFrontPtr();
}

void setFront(LiveActor* actor, const sead::Vector3f& front) {
    getFrontPtr(actor)->set(front);
}

const sead::Vector3f& getUp(const LiveActor* actor) {
    return actor->getPoseKeeper()->getUp();
}

sead::Vector3f* getUpPtr(LiveActor* actor) {
    return actor->getPoseKeeper()->getUpPtr();
}

void setUp(LiveActor* actor, const sead::Vector3f& up) {
    getUpPtr(actor)->set(up);
}

void multVecPoseNoTrans(sead::Vector3f* posOut, const LiveActor* actor,
                        const sead::Vector3f& posIn) {
    sead::Matrix34f mtx;
    makeMtxR(&mtx, actor);
    posOut->setMul(mtx, posIn);
}

void multVecPose(sead::Vector3f* posOut, const LiveActor* actor, const sead::Vector3f& posIn) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    posOut->setMul(mtx, posIn);
}

void multVecPoseAndScale(sead::Vector3f* posOut, const LiveActor* actor,
                         const sead::Vector3f& posIn) {
    sead::Matrix34f mtx;
    makeMtxSRT(&mtx, actor);
    posOut->setMul(mtx, posIn);
}

void multVecInvPose(sead::Vector3f* posOut, const LiveActor* actor, const sead::Vector3f& posIn) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    mtx.invert();
    posOut->setMul(mtx, posIn);
}

// TODO: requires Quatf::rotate to be implemented
// void multVecInvQuat(sead::Vector3f* posOut, const LiveActor* actor, const sead::Vector3f& posIn);

void multMtxInvPose(sead::Matrix34f* mtxOut, const LiveActor* actor, const sead::Matrix34f& mtxIn) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    mtx.invert();
    mtxOut->setMul(mtx, mtxIn);
}

void calcTransLocalOffset(sead::Vector3f* transOut, const LiveActor* actor,
                          const sead::Vector3f& transIn) {
    sead::Matrix34f mtx;
    makeMtxRT(&mtx, actor);
    calcTransLocalOffsetByMtx(transOut, mtx, transIn);
}

}  // namespace al

namespace alActorPoseFunction {

void calcBaseMtx(sead::Matrix34f* mtx, const al::LiveActor* actor) {
    actor->getPoseKeeper()->calcBaseMtx(mtx);
}

void updatePoseTRMSV(al::LiveActor* actor) {
    al::ActorPoseKeeperBase* poseKeeper = actor->getPoseKeeper();
    poseKeeper->updatePoseRotate(poseKeeper->getRotate());
}

}  // namespace alActorPoseFunction
