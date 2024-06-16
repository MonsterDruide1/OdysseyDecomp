#pragma once

#include <math/seadMatrix.h>

namespace al {
class ActorInitInfo;
class BreakModel;
class HitSensor;
class HostSyncTowerCamera;
class LiveActor;
class CollisionObj;
class PartsModel;
class KeyMoveCameraObj;

CollisionObj* createCollisionObj(const LiveActor*, const ActorInitInfo&, const char*, HitSensor*,
                                 const char*, const char*);
CollisionObj* createCollisionObjMtx(const LiveActor*, const ActorInitInfo&, const char*, HitSensor*,
                                    const sead::Matrix34f*, const char*);
PartsModel* createPartsModel(LiveActor*, const ActorInitInfo&, const char*, const char*,
                             const sead::Matrix34f*);
PartsModel* createPartsModelFile(LiveActor*, const ActorInitInfo&, const char*, const char*,
                                 const char*);
PartsModel* createPartsModelFileSuffix(LiveActor*, const ActorInitInfo&, const char*, const char*,
                                       const char*, const char*);
PartsModel* createSimplePartsModel(LiveActor*, const ActorInitInfo&, const char*, const char*,
                                   const char*);
PartsModel* createSimplePartsModelSuffix(LiveActor*, const ActorInitInfo&, const char*, const char*,
                                         const char*, const char*);
PartsModel* createPartsModelSuffix(LiveActor*, const ActorInitInfo&, const char*, const char*,
                                   const char*, const sead::Matrix34f*);
PartsModel* createPartsModelJoint(LiveActor*, const ActorInitInfo&, const char*, const char*,
                                  const char*);
PartsModel* createPartsModelSuffixJoint(LiveActor*, const ActorInitInfo&, const char*, const char*,
                                        const char*, const char*);
void appearBreakModelRandomRotateY(LiveActor*);
bool updateSyncHostVisible(bool*, LiveActor*, const LiveActor*, bool);
bool isTraceModelRandomRotate(const LiveActor*);
BreakModel* createBreakModel(const LiveActor*, const ActorInitInfo&, const char*, const char*,
                             const char*, const sead::Matrix34f*, const char*);
HostSyncTowerCamera* createHostSyncTowerCamera(LiveActor*, const ActorInitInfo&);
void activateHostSyncTowerCamera(HostSyncTowerCamera*);
void deactivateHostSyncTowerCamera(HostSyncTowerCamera*);
void updateHostSyncTowerCamera(HostSyncTowerCamera*);
KeyMoveCameraObj* tryCreateLinksKeyMoveCameraObj(const ActorInitInfo&, const char*);
KeyMoveCameraObj* tryCreateLinksKeyMoveCameraObjWithInterpole(const ActorInitInfo&, const char*);
}  // namespace al
