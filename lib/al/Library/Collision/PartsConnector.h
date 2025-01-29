#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class MtxConnector;
class LiveActor;
class ActorInitInfo;

MtxConnector* createMtxConnector(const LiveActor* actor);
MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& info);
void attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor, bool);
void attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor,
                                   const sead::Vector3f&, const sead::Vector3f&);
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
}  // namespace al
