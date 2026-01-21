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

CollisionObj* createCollisionObj(const LiveActor* parent, const ActorInitInfo& info,
                                 const char* collisionFileName, HitSensor* hitSensor,
                                 const char* jointMtxName, const char* suffix);
CollisionObj* createCollisionObjMtx(const LiveActor* parent, const ActorInitInfo& info,
                                    const char* collisionFileName, HitSensor* hitSensor,
                                    const sead::Matrix34f* jointMtx, const char* suffix);

PartsModel* createPartsModel(LiveActor* parent, const ActorInitInfo& info, const char* name,
                             const char* arcName, const sead::Matrix34f* jointMtx);
PartsModel* createPartsModelFile(LiveActor* parent, const ActorInitInfo& info, const char* name,
                                 const char* arcName, const char* suffix);
PartsModel* createPartsModelFileSuffix(LiveActor* parent, const ActorInitInfo& info,
                                       const char* name, const char* arcName, const char* arcSuffix,
                                       const char* suffix);
PartsModel* createSimplePartsModel(LiveActor* parent, const ActorInitInfo& info, const char* name,
                                   const char* arcName, const char* suffix);
PartsModel* createSimplePartsModelSuffix(LiveActor* parent, const ActorInitInfo& info,
                                         const char* name, const char* arcName,
                                         const char* arcSuffix, const char* suffix);
PartsModel* createPartsModelSuffix(LiveActor* parent, const ActorInitInfo& info, const char* name,
                                   const char* arcName, const char* suffix,
                                   const sead::Matrix34f* jointMtx);
PartsModel* createPartsModelJoint(LiveActor* parent, const ActorInitInfo& info, const char* name,
                                  const char* arcName, const char* jointMtxName);
PartsModel* createPartsModelSuffixJoint(LiveActor* parent, const ActorInitInfo& info,
                                        const char* name, const char* arcName,
                                        const char* arcSuffix, const char* jointMtxName);
void appearBreakModelRandomRotateY(LiveActor* actor);
bool updateSyncHostVisible(bool* isChildHidden, LiveActor* actor, const LiveActor* host, bool unk);
bool isTraceModelRandomRotate(const LiveActor* actor);
BreakModel* createBreakModel(const LiveActor* parent, const ActorInitInfo& info,
                             const char* objName, const char* modelName, const char* fileSuffixName,
                             const sead::Matrix34f* rootMtx, const char* breakActionName);
HostSyncTowerCamera* createHostSyncTowerCamera(LiveActor* actor, const ActorInitInfo& info);
void activateHostSyncTowerCamera(HostSyncTowerCamera* camera);
void deactivateHostSyncTowerCamera(HostSyncTowerCamera* camera);
void updateHostSyncTowerCamera(HostSyncTowerCamera* camera);
KeyMoveCameraObj* tryCreateLinksKeyMoveCameraObj(const ActorInitInfo& info, const char* childName);
KeyMoveCameraObj* tryCreateLinksKeyMoveCameraObjWithInterpole(const ActorInitInfo& info,
                                                              const char* childName);
}  // namespace al
