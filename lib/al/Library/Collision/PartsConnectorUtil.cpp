#include "Library/Collision/PartsConnectorUtil.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/Collision/CollisionPartsTriangle.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/Collision/PartsMtxConnector.h"
#include "Library/Collision/SensorConnector.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Screen/ScreenPointTarget.h"
#include "Project/HitSensor/HitSensor.h"

namespace al {

inline bool IsConnectToCollision(const ActorInitInfo& initInfo) {
    bool isConnectToCollision = false;

    if (!al::tryGetArg(&isConnectToCollision, initInfo, "IsConnectToCollision"))
        return false;

    return isConnectToCollision;
}

MtxConnector* createMtxConnector(const LiveActor* actor) {
    return new MtxConnector(getQuat(actor), getTrans(actor));
}

MtxConnector* createMtxConnector(const LiveActor* actor, const sead::Quatf& quat) {
    return new MtxConnector(quat, getTrans(actor));
}

MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& initInfo) {
    return IsConnectToCollision(initInfo) ? createMtxConnector(actor) : nullptr;
}

MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& initInfo,
                                    const sead::Quatf& quat) {
    return IsConnectToCollision(initInfo) ? createMtxConnector(actor, quat) : nullptr;
}

CollisionPartsConnector* createCollisionPartsConnector(const LiveActor* actor,
                                                       const sead::Quatf& quat) {
    CollisionPartsConnector* connector = new CollisionPartsConnector();
    connector->setBaseQuatTrans(quat, getTrans(actor));
    return connector;
}

CollisionPartsConnector* tryCreateCollisionPartsConnector(const LiveActor* actor,
                                                          const ActorInitInfo& initInfo) {
    return tryCreateCollisionPartsConnector(actor, initInfo, getQuat(actor));
}

CollisionPartsConnector* tryCreateCollisionPartsConnector(const LiveActor* actor,
                                                          const ActorInitInfo& initInfo,
                                                          const sead::Quatf& quat) {
    return IsConnectToCollision(initInfo) ? createCollisionPartsConnector(actor, quat) : nullptr;
}

bool isMtxConnectorConnecting(const MtxConnector* connector) {
    return connector->isConnecting();
}

void disconnectMtxConnector(MtxConnector* connector) {
    return connector->clear();
}

CollisionParts* attachMtxConnectorToCollision(MtxConnector* connector, const LiveActor* actor,
                                              const sead::Vector3f& pos,
                                              const sead::Vector3f& dir) {
    CollisionPartsFilterActor partsFilter = CollisionPartsFilterActor(actor);
    CollisionParts* parts = alCollisionUtil::getStrikeArrowCollisionParts(actor, nullptr, pos, dir,
                                                                          &partsFilter, nullptr);
    if (!parts)
        return nullptr;

    attachMtxConnectorToCollisionParts(connector, parts);
    return parts;
}

CollisionParts* attachMtxConnectorToCollision(MtxConnector* connector, const LiveActor* actor,
                                              bool isAttachToGround) {
    return attachMtxConnectorToCollision(connector, actor, 50.0f,
                                         isAttachToGround ? -150.0f : 150.0f);
}

CollisionParts* attachMtxConnectorToCollision(MtxConnector* connector, const LiveActor* actor,
                                              f32 checkOffUp, f32 checkDistance) {
    sead::Vector3f upDir;
    calcUpDir(&upDir, actor);

    sead::Vector3f dir = upDir * -checkDistance;
    sead::Vector3f pos = getTrans(actor) + upDir * checkOffUp;
    return attachMtxConnectorToCollision(connector, actor, pos, dir);
}

void attachMtxConnectorToCollisionParts(MtxConnector* connector, const CollisionParts* parts) {
    connector->init(&parts->getBaseMtx(), parts->getBaseInvMtx());
}

void setConnectorBaseQuatTrans(const sead::Quatf& quat, const sead::Vector3f& trans,
                               MtxConnector* connector) {
    connector->setBaseQuatTrans(quat, trans);
}

void setConnectorBaseQuatTrans(LiveActor* actor, MtxConnector* connector) {
    connector->setBaseQuatTrans(getQuat(actor), getTrans(actor));
}

void connectPoseQT(LiveActor* actor, const MtxConnector* connector) {
    calcConnectQT(getQuatPtr(actor), getTransPtr(actor), connector);
}

void connectPoseQT(LiveActor* actor, const MtxConnector* connector, const sead::Quatf& quat,
                   const sead::Vector3f& trans) {
    calcConnectQT(getQuatPtr(actor), getTransPtr(actor), connector, quat, trans);
}

void connectPoseTrans(LiveActor* actor, const MtxConnector* connector,
                      const sead::Vector3f& trans) {
    calcConnectTrans(getTransPtr(actor), connector, trans);
}

void connectPoseMtx(LiveActor* actor, const MtxConnector* connector, const sead::Matrix34f& mtx) {
    sead::Matrix34f poseMtx;
    calcConnectMtx(&poseMtx, connector, mtx);
    normalize(&poseMtx);
    updatePoseMtx(actor, &poseMtx);
}

void calcConnectTrans(sead::Vector3f* outTrans, const MtxConnector* connector,
                      const sead::Vector3f& trans) {
    connector->multTrans(outTrans, trans);
}

void calcConnectTrans(sead::Vector3f* outTrans, const MtxConnector* connector) {
    calcConnectTrans(outTrans, connector, connector->getBaseTrans());
}

void calcConnectDir(sead::Vector3f* outDir, const MtxConnector* connector,
                    const sead::Vector3f& dir) {
    connector->multVec(outDir, dir);
    normalize(outDir);
}

void calcConnectQT(sead::Quatf* outQuat, sead::Vector3f* outTrans, const MtxConnector* connector,
                   const sead::Quatf& poseQuat, const sead::Vector3f& poseTrans) {
    connector->multQT(outQuat, outTrans, poseQuat, poseTrans);
}

void calcConnectQT(sead::Quatf* outQuat, sead::Vector3f* outTrans, const MtxConnector* connector) {
    connector->multQT(outQuat, outTrans, nullptr);
}

void calcConnectMtx(sead::Matrix34f* outMtx, const MtxConnector* connector,
                    const sead::Matrix34f& mtx) {
    connector->multMtx(outMtx, mtx);
}

void calcConnectMtx(sead::Matrix34f* outMtx, const MtxConnector* connector, const sead::Quatf& quat,
                    const sead::Vector3f& trans) {
    sead::Matrix34f mtx;
    mtx.makeQT(quat, trans);
    calcConnectMtx(outMtx, connector, mtx);
}

void attachMtxConnectorToCollisionRT(MtxConnector* connector, const LiveActor* actor,
                                     bool isFacingUp, bool useStrikeArrowPos) {
    sead::Vector3f facingDir;
    calcUpDir(&facingDir, actor);
    if (!isFacingUp)
        facingDir = -facingDir;

    sead::Vector3f dir = facingDir * 150.0f;
    sead::Vector3f pos = getTrans(actor) - facingDir * 50.0f;

    CollisionPartsFilterActor partsFilter = CollisionPartsFilterActor(actor);
    sead::Vector3f arrowPos;
    CollisionParts* parts = alCollisionUtil::getStrikeArrowCollisionParts(
        actor, &arrowPos, pos, dir, &partsFilter, nullptr);
    if (!parts)
        return;

    const sead::Vector3f& rotate = getRotate(actor);
    sead::Matrix34f mtx;
    mtx.makeRT({sead::Mathf::deg2rad(rotate.x), sead::Mathf::deg2rad(rotate.y),
                sead::Mathf::deg2rad(rotate.z)},
               useStrikeArrowPos ? arrowPos : getTrans(actor));
    connector->init(&parts->getBaseMtx(), parts->getBaseInvMtx() * mtx);
}

void attachMtxConnectorToCollisionQT(MtxConnector* connector, const LiveActor* actor,
                                     bool isFacingUp, bool useStrikeArrowPos) {
    sead::Vector3f facingDir;
    calcUpDir(&facingDir, actor);
    if (!isFacingUp)
        facingDir = -facingDir;

    sead::Vector3f dir = facingDir * 150.0f;
    sead::Vector3f pos = getTrans(actor) - facingDir * 50.0f;

    CollisionPartsFilterActor partsFilter = CollisionPartsFilterActor(actor);
    sead::Vector3f arrowPos;
    CollisionParts* parts = alCollisionUtil::getStrikeArrowCollisionParts(
        actor, &arrowPos, pos, dir, &partsFilter, nullptr);
    if (!parts)
        return;

    sead::Matrix34f mtx;
    mtx.makeQT(getQuat(actor), useStrikeArrowPos ? arrowPos : getTrans(actor));
    connector->init(&parts->getBaseMtx(), parts->getBaseInvMtx() * mtx);
}

void attachMtxConnectorToJoint(MtxConnector* connector, const LiveActor* actor,
                               const char* jointName) {
    attachMtxConnectorToMtxPtr(connector, getJointMtxPtr(actor, jointName));
}

void attachMtxConnectorToJoint(MtxConnector* connector, const LiveActor* actor,
                               const char* jointName, const sead::Vector3f& rotation,
                               const sead::Vector3f& trans) {
    attachMtxConnectorToMtxPtr(connector, getJointMtxPtr(actor, jointName), rotation, trans);
}

void attachMtxConnectorToMtxPtr(MtxConnector* connector, const sead::Matrix34f* mtx,
                                const sead::Vector3f& rotation, const sead::Vector3f& trans) {
    sead::Quatf quat;
    quat.setRPY(sead::Mathf::deg2rad(rotation.x), sead::Mathf::deg2rad(rotation.y),
                sead::Mathf::deg2rad(rotation.z));
    connector->setBaseQuatTrans(quat, trans);
    attachMtxConnectorToMtxPtr(connector, mtx);
}

void attachMtxConnectorToActor(MtxConnector* connector, const LiveActor* actor) {
    attachMtxConnectorToMtxPtr(connector, actor->getBaseMtx());
}

void attachMtxConnectorToActor(MtxConnector* connector, const LiveActor* actor,
                               const sead::Vector3f& rotation, const sead::Vector3f& trans) {
    attachMtxConnectorToMtxPtr(connector, actor->getBaseMtx(), rotation, trans);
}

void attachMtxConnectorToMtxPtr(MtxConnector* connector, const sead::Matrix34f* mtx) {
    connector->init(mtx, sead::Matrix34f::ident);
}

void attachMtxConnectorToSensor(MtxConnector* connector, HitSensor* hitSensor,
                                const sead::Matrix34f& mtx) {
    const sead::Matrix34f* followMtx = hitSensor->getFollowMtx();
    if (followMtx) {
        sead::Matrix34f newMtx;
        newMtx.setInverse(*followMtx);
        newMtx = newMtx * mtx;
        connector->init(followMtx, newMtx);
        return;
    }

    if (hitSensor->getParentActor()->getBaseMtx()) {
        const sead::Matrix34f* baseMtx = hitSensor->getParentActor()->getBaseMtx();
        if (baseMtx) {
            sead::Matrix34f newMtx;
            newMtx.setInverse(*baseMtx);
            newMtx = newMtx * mtx;
            connector->init(baseMtx, newMtx);
        }
    }
}

void attachSensorConnectorToSensor(SensorConnector* connector, HitSensor* hitSensor,
                                   const sead::Matrix34f& mtx) {
    const sead::Matrix34f* followMtx = hitSensor->getFollowMtx();
    if (followMtx) {
        sead::Matrix34f newMtx;
        newMtx.setInverse(*followMtx);
        newMtx = newMtx * mtx;
        connector->init(followMtx, newMtx, hitSensor);
        return;
    }

    if (hitSensor->getParentActor()->getBaseMtx()) {
        const sead::Matrix34f* baseMtx = hitSensor->getParentActor()->getBaseMtx();
        if (baseMtx) {
            sead::Matrix34f newMtx;
            newMtx.setInverse(*baseMtx);
            newMtx = newMtx * mtx;
            connector->init(baseMtx, newMtx, hitSensor);
        }
    }
}

void attachMtxConnectorToScreenPointTarget(MtxConnector* connector,
                                           ScreenPointTarget* screenPointTarget,
                                           const sead::Matrix34f& mtx) {
    const sead::Matrix34f* followMtx = screenPointTarget->getJointMtx();
    if (followMtx) {
        sead::Matrix34f newMtx;
        newMtx.setInverse(*followMtx);
        newMtx = newMtx * mtx;
        connector->init(followMtx, newMtx);
        return;
    }

    if (screenPointTarget->getActor()->getBaseMtx()) {
        const sead::Matrix34f* baseMtx = screenPointTarget->getActor()->getBaseMtx();
        if (baseMtx) {
            sead::Matrix34f newMtx;
            newMtx.setInverse(*baseMtx);
            newMtx = newMtx * mtx;
            connector->init(baseMtx, newMtx);
        }
    }
}

void attachCollisionPartsConnector(CollisionPartsConnector* partsConnector,
                                   const CollisionParts* parts) {
    partsConnector->init(&parts->getBaseMtx(), parts->getBaseInvMtx(), parts);
}

void attachCollisionPartsConnectorToGround(CollisionPartsConnector* partsConnector,
                                           const LiveActor* actor) {
    // Same as attachMtxConnectorToCollision(connector, actor, true)
    CollisionPartsFilterActor partsFilter = CollisionPartsFilterActor(actor);
    sead::Vector3f upDir{0.0f, 0.0f, 0.0f};
    calcUpDir(&upDir, actor);
    CollisionParts* parts = alCollisionUtil::getStrikeArrowCollisionParts(
        actor, nullptr, getTrans(actor) + upDir * 50.0f, upDir * -150.0f, &partsFilter, nullptr);
    attachCollisionPartsConnector(partsConnector, parts);
}

void attachToHitTriangle(CollisionPartsConnector* partsConnector, const Triangle& triangle,
                         const sead::Matrix34f& mtx) {
    const CollisionParts* collisionParts = triangle.getCollisionParts();
    sead::Matrix34f partsMtx = collisionParts->getBaseInvMtx() * mtx;
    partsConnector->init(&collisionParts->getBaseMtx(), partsMtx, collisionParts);
}

void attachToHitInfo(CollisionPartsConnector* partsConnector, const HitInfo& hitInfo,
                     const sead::Matrix34f& mtx) {
    attachToHitTriangle(partsConnector, hitInfo.triangle, mtx);
}

void attachToHitInfoNrmToMinusZ(CollisionPartsConnector* partsConnector, const HitInfo& hitInfo) {
    sead::Matrix34f mtx;
    makeMtxFrontNoSupportPos(&mtx, -hitInfo.triangle.getFaceNormal(), hitInfo.collisionHitPos);
    attachToHitInfo(partsConnector, hitInfo, mtx);
}

void calcConnectInfo(const MtxConnector* connector, sead::Vector3f* outTrans, sead::Quatf* outQuat,
                     sead::Vector3f* outScale, const sead::Vector3f& vecA,
                     const sead::Vector3f& vecB) {
    connector->calcConnectInfo(outTrans, outQuat, outScale, vecA, vecB);
}

void connectPoseQTUsingConnectInfo(LiveActor* actor, const MtxConnector* connector) {
    if (!connector->isConnecting())
        return;

    sead::Vector3f trans;
    sead::Quatf quat;
    connector->calcConnectInfo(&trans, &quat, nullptr, sead::Vector3f::zero, sead::Vector3f::zero);
    setTrans(actor, trans);
    setQuat(actor, quat);
}

const sead::Quatf& getConnectBaseQuat(const MtxConnector* connector) {
    return connector->getBaseQuat();
}

const sead::Vector3f& getConnectBaseTrans(const MtxConnector* connector) {
    return connector->getBaseTrans();
}

}  // namespace al
