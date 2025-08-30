#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class CollisionParts;
class CollisionPartsConnector;
struct HitInfo;
class HitSensor;
class LiveActor;
class MtxConnector;
class ScreenPointTarget;
class SensorConnector;
class Triangle;

MtxConnector* createMtxConnector(const LiveActor* actor);
MtxConnector* createMtxConnector(const LiveActor* actor, const sead::Quatf& quat);
MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& initInfo);
MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& initInfo,
                                    const sead::Quatf& quat);
CollisionPartsConnector* createCollisionPartsConnector(const LiveActor* actor,
                                                       const sead::Quatf& quat);
CollisionPartsConnector* tryCreateCollisionPartsConnector(const LiveActor* actor,
                                                          const ActorInitInfo& initInfo);
CollisionPartsConnector* tryCreateCollisionPartsConnector(const LiveActor* actor,
                                                          const ActorInitInfo& initInfo,
                                                          const sead::Quatf& quat);
bool isMtxConnectorConnecting(const MtxConnector* connector);
void disconnectMtxConnector(MtxConnector* connector);
CollisionParts* attachMtxConnectorToCollision(MtxConnector* connector, const LiveActor* actor,
                                              const sead::Vector3f& pos, const sead::Vector3f& dir);
CollisionParts* attachMtxConnectorToCollision(MtxConnector* connector, const LiveActor* actor,
                                              bool isAttachToGround);
CollisionParts* attachMtxConnectorToCollision(MtxConnector* connector, const LiveActor* actor,
                                              f32 checkOffUp, f32 checkDistance);
void attachMtxConnectorToCollisionParts(MtxConnector* connector, const CollisionParts* parts);
void setConnectorBaseQuatTrans(const sead::Quatf& quat, const sead::Vector3f& trans,
                               MtxConnector* connector);
void setConnectorBaseQuatTrans(LiveActor* actor, MtxConnector* connector);
void connectPoseQT(LiveActor* actor, const MtxConnector* connector);
void connectPoseQT(LiveActor* actor, const MtxConnector* connector, const sead::Quatf& quat,
                   const sead::Vector3f& trans);
void connectPoseTrans(LiveActor* actor, const MtxConnector* connector, const sead::Vector3f& trans);
void connectPoseMtx(LiveActor* actor, const MtxConnector* connector, const sead::Matrix34f& mtx);
void calcConnectTrans(sead::Vector3f* outTrans, const MtxConnector* connector,
                      const sead::Vector3f& trans);
void calcConnectTrans(sead::Vector3f* outTrans, const MtxConnector* connector);
void calcConnectDir(sead::Vector3f* outDir, const MtxConnector* connector,
                    const sead::Vector3f& dir);
void calcConnectQT(sead::Quatf* outQuat, sead::Vector3f* outTrans, const MtxConnector* connector,
                   const sead::Quatf& poseQuat, const sead::Vector3f& poseTrans);
void calcConnectQT(sead::Quatf* outQuat, sead::Vector3f* outTrans, const MtxConnector* connector);
void calcConnectMtx(sead::Matrix34f* outMtx, const MtxConnector* connector,
                    const sead::Matrix34f& mtx);
void calcConnectMtx(sead::Matrix34f* outMtx, const MtxConnector* connector, const sead::Quatf& quat,
                    const sead::Vector3f& trans);
void attachMtxConnectorToCollisionRT(MtxConnector* connector, const LiveActor* actor,
                                     bool isFacingUp, bool useStrikeArrowPos);
void attachMtxConnectorToCollisionQT(MtxConnector* connector, const LiveActor* actor,
                                     bool isFacingUp, bool useStrikeArrowPos);
void attachMtxConnectorToJoint(MtxConnector* connector, const LiveActor* actor,
                               const char* jointName);
void attachMtxConnectorToJoint(MtxConnector* connector, const LiveActor* actor,
                               const char* jointName, const sead::Vector3f& rotation,
                               const sead::Vector3f& trans);
void attachMtxConnectorToMtxPtr(MtxConnector* connector, const sead::Matrix34f* mtx,
                                const sead::Vector3f& rotation, const sead::Vector3f& trans);
void attachMtxConnectorToActor(MtxConnector* connector, const LiveActor* actor);
void attachMtxConnectorToActor(MtxConnector* connector, const LiveActor* actor,
                               const sead::Vector3f& rotation, const sead::Vector3f& trans);
void attachMtxConnectorToMtxPtr(MtxConnector* connector, const sead::Matrix34f* mtx);
void attachMtxConnectorToSensor(MtxConnector* connector, HitSensor* hitSensor,
                                const sead::Matrix34f& mtx);
void attachSensorConnectorToSensor(SensorConnector* connector, HitSensor* hitSensor,
                                   const sead::Matrix34f& mtx);
void attachMtxConnectorToScreenPointTarget(MtxConnector* connector,
                                           ScreenPointTarget* screenPointTarget,
                                           const sead::Matrix34f& mtx);
void attachCollisionPartsConnector(CollisionPartsConnector* partsConnector,
                                   const CollisionParts* parts);
void attachCollisionPartsConnectorToGround(CollisionPartsConnector* partsConnector,
                                           const LiveActor* actor);
void attachToHitTriangle(CollisionPartsConnector* partsConnector, const Triangle& triangle,
                         const sead::Matrix34f& mtx);
void attachToHitInfo(CollisionPartsConnector* partsConnector, const HitInfo& hitInfo,
                     const sead::Matrix34f& mtx);
void attachToHitInfoNrmToMinusZ(CollisionPartsConnector* partsConnector, const HitInfo& hitInfo);
// TODO: Rename vecA and vecB
void calcConnectInfo(const MtxConnector* connector, sead::Vector3f* outTrans, sead::Quatf* outQuat,
                     sead::Vector3f* outScale, const sead::Vector3f& vecA,
                     const sead::Vector3f& vecB);
void connectPoseQTUsingConnectInfo(LiveActor* actor, const MtxConnector* connector);
const sead::Quatf& getConnectBaseQuat(const MtxConnector* connector);
const sead::Vector3f& getConnectBaseTrans(const MtxConnector* connector);

}  // namespace al
