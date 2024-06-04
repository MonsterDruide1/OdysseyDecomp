#pragma once
#include <math/seadMatrix.h>

namespace al {
struct ActorInitInfo;
class BreakModel;
class HitSensor;
class HostSyncTowerCamera;
class LiveActor;
class CollisionObj;
class PartsModel;
class KeyMoveCameraObj;

al::CollisionObj* createCollisionObj(const al::LiveActor*, const al::ActorInitInfo&, const char*,
                                     al::HitSensor*, const char*, const char*);
al::CollisionObj* createCollisionObjMtx(const al::LiveActor*, const al::ActorInitInfo&, const char*,
                                        al::HitSensor*, const sead::Matrix34f*, const char*);
al::PartsModel* createPartsModel(al::LiveActor*, const al::ActorInitInfo&, const char*, const char*,
                                 const sead::Matrix34f*);
al::PartsModel* createPartsModelFile(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                     const char*, const char*);
al::PartsModel* createPartsModelFileSuffix(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                           const char*, const char*, const char*);
al::PartsModel* createSimplePartsModel(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                       const char*, const char*);
al::PartsModel* createSimplePartsModelSuffix(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                             const char*, const char*, const char*);
al::PartsModel* createPartsModelSuffix(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                       const char*, const char*, const sead::Matrix34f*);
al::PartsModel* createPartsModelJoint(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                      const char*, const char*);
al::PartsModel* createPartsModelSuffixJoint(al::LiveActor*, const al::ActorInitInfo&, const char*,
                                            const char*, const char*, const char*);
void appearBreakModelRandomRotateY(al::LiveActor*);
bool updateSyncHostVisible(bool*, al::LiveActor*, const al::LiveActor*, bool);
bool isTraceModelRandomRotate(const al::LiveActor*);
al::BreakModel* createBreakModel(const al::LiveActor*, const al::ActorInitInfo&, const char*,
                                 const char*, const char*, const sead::Matrix34f*, const char*);
al::HostSyncTowerCamera* createHostSyncTowerCamera(al::LiveActor*, const al::ActorInitInfo&);
void activateHostSyncTowerCamera(al::HostSyncTowerCamera*);
void deactivateHostSyncTowerCamera(al::HostSyncTowerCamera*);
void updateHostSyncTowerCamera(al::HostSyncTowerCamera*);
al::KeyMoveCameraObj* tryCreateLinksKeyMoveCameraObj(const al::ActorInitInfo&, const char*);
al::KeyMoveCameraObj* tryCreateLinksKeyMoveCameraObjWithInterpole(const al::ActorInitInfo&,
                                                                  const char*);
}  // namespace al