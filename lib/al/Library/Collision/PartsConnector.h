#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class MtxConnector;
class LiveActor;
class ActorInitInfo;

MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& info);
void attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor, bool);
void connectPoseQT(LiveActor* actor, const MtxConnector* mtxConnector, const sead::Quatf& quat,
                   const sead::Vector3f& vec);
void connectPoseQT(LiveActor* actor, const MtxConnector* mtxConnector);
void attachMtxConnectorToJoint(MtxConnector*, const LiveActor*, const char*);
void attachMtxConnectorToJoint(MtxConnector*, const LiveActor*, const char*, const sead::Vector3f&,
                               const sead::Vector3f&);
void disconnectMtxConnector(MtxConnector*);
bool isMtxConnectorConnecting(const MtxConnector*);
}  // namespace al
