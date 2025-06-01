#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class CollisionParts;
class CollisionPartsConnector;
class LiveActor;
class MtxConnector;

CollisionPartsConnector* createCollisionPartsConnector(const LiveActor* actor,
                                                       const sead::Quatf& quat);
MtxConnector* createMtxConnector(const LiveActor* actor);
MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& info);
void attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor, bool);
void attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor,
                                   const sead::Vector3f&, const sead::Vector3f&);
void attachMtxConnectorToCollisionParts(MtxConnector* mtxConnector,
                                        const CollisionParts* collisionParts);
CollisionParts* attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor,
                                              f32, f32);
void connectPoseQT(LiveActor* actor, const MtxConnector* mtxConnector, const sead::Quatf& quat,
                   const sead::Vector3f& trans);
void connectPoseQT(LiveActor* actor, const MtxConnector* mtxConnector);
void attachMtxConnectorToJoint(MtxConnector* mtxConnector, const LiveActor* actor,
                               const char* jointName);
void attachMtxConnectorToJoint(MtxConnector* mtxConnector, const LiveActor* actor,
                               const char* jointName, const sead::Vector3f& quatInitVec,
                               const sead::Vector3f& trans);
void disconnectMtxConnector(MtxConnector* mtxConnector);
bool isMtxConnectorConnecting(const MtxConnector* mtxConnector);
void calcConnectTrans(sead::Vector3f* connectTrans, const MtxConnector* mtxConnector);
void setConnectorBaseQuatTrans(const sead::Quatf& quat, const sead::Vector3f& trans,
                               MtxConnector* mtxConnector);

}  // namespace al
