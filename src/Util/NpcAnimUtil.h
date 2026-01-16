#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class IUseNerve;
class LiveActor;
class MtxConnector;
class SensorMsg;
struct ActorInitInfo;
}  // namespace al
class ActorStateReactionBase;
class NpcJointLookAtController;
class NpcStateReactionParam;
class PlayerEyeSensorHitHolder;
class TalkNpcParam;

namespace rs {
bool tryApplyNpcMaterialAnimPresetFromPlacementInfo(al::LiveActor*, const al::ActorInitInfo&,
                                                    const TalkNpcParam*);
void setNpcMaterialAnimFromPlacementInfo(al::LiveActor*, const al::ActorInitInfo&);
bool tryUpdateNpcEyeLineAnim(al::LiveActor*, const TalkNpcParam*);
bool tryUpdateNpcEyeLineAnimToTarget(al::LiveActor*, const TalkNpcParam*, const sead::Vector3f&,
                                     bool);
void resetNpcEyeLineAnim(al::LiveActor*);
bool tryUpdateNpcFacialAnim(al::LiveActor*, const TalkNpcParam*);
void trySwitchDepthToSelfShadow(al::LiveActor*);
bool tryReceiveMsgPlayerDisregard(const al::SensorMsg*, al::HitSensor*, const TalkNpcParam*);
void attackSensorNpcCommon(al::HitSensor*, al::HitSensor*);
bool receiveMsgNpcCommonNoReaction(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
void calcNpcPushVecBetweenSensors(sead::Vector3f*, al::HitSensor*, al::HitSensor*);
NpcJointLookAtController* createAndAppendNpcJointLookAtController(al::LiveActor*,
                                                                  const TalkNpcParam*);
NpcJointLookAtController* tryCreateAndAppendNpcJointLookAtController(al::LiveActor*,
                                                                     const TalkNpcParam*);
NpcJointLookAtController* tryCreateAndAppendNpcJointLookAtController(al::LiveActor*,
                                                                     const TalkNpcParam*, f32);
void setPlayerEyeSensorHitHolder(NpcJointLookAtController*, const PlayerEyeSensorHitHolder*);
s32 getNpcJointLookAtControlJointNum(const TalkNpcParam*);
void updateNpcJointLookAtController(NpcJointLookAtController*);
void cancelUpdateNpcJointLookAtController(NpcJointLookAtController*);
void updateNpcJointLookAtControllerLookAtDistance(NpcJointLookAtController*, f32);
void invalidateNpcJointLookAtController(NpcJointLookAtController*);
void validateNpcJointLookAtController(NpcJointLookAtController*);
void requestLookAtTargetTrans(NpcJointLookAtController*, const sead::Vector3f&);
void initCapWorldNpcTail(al::LiveActor*);
bool tryStartForestManFlowerAnim(al::LiveActor*);
bool tryUpdateMaterialCodeByFloorCollisionOnArrow(al::LiveActor*);
void tryAttachConnectorToCollisionTFSV(al::LiveActor*, al::MtxConnector*, sead::Quatf*);
void tryConnectToCollisionTFSV(al::LiveActor*, const al::MtxConnector*, sead::Quatf*);
al::LiveActor* tryGetSubActorCityMayorFace(const al::LiveActor*);
void syncActionCityMayorFace(al::LiveActor*);
void syncMtsAnimCityMayorFace(al::LiveActor*);
bool isExistFaceAnim(al::LiveActor*, const char*);
void animateCityMayorFace(al::LiveActor*, const char*, f32);
ActorStateReactionBase* createNpcStateReaction(al::LiveActor*, const TalkNpcParam*,
                                               const NpcStateReactionParam*);
bool isInvalidTrampleSensor(const al::HitSensor*, const TalkNpcParam*);
}  // namespace rs

namespace TalkNpcFunction {
bool tryGetHackingEventHackType(s32*, const al::ActorInitInfo&);
}

namespace BirdFunction {
void tryUpdateFlyAwayDisappearDitherAlpha(al::LiveActor* actor, const al::IUseNerve* user,
                                          s32 start_step, s32 end_step);
}
