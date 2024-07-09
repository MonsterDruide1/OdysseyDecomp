#pragma once

namespace al {
class MtxConnector;
class LiveActor;
class ActorInitInfo;

MtxConnector* tryCreateMtxConnector(const LiveActor* actor, const ActorInitInfo& info);
void attachMtxConnectorToCollision(MtxConnector* mtxConnector, const LiveActor* actor, bool);
void connectPoseQT(LiveActor* actor, const MtxConnector* mtxConnector);
}  // namespace al
