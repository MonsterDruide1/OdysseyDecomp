#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
struct ActorInitInfo;
class HitSensor;
}  // namespace al

class KoopaCap;
class KoopaNumberCounter;
class KoopaItemHolder;
class KoopaCameraCtrl;
class Peach;

namespace KoopaFunction {
void initActorKoopa(al::LiveActor*, const al::ActorInitInfo&);
void invalidateTailSensors(al::LiveActor*);
void initActorKoopaDemoModel(al::LiveActor*, const al::ActorInitInfo&);
void initAndCreateDemoModel(const al::ActorInitInfo&);
void initAndCreateLinksPeach(const al::ActorInitInfo&);
void initAndCreateDemoPeachRing(const al::ActorInitInfo&);
void initAndCreateWeaponCapGroup(const al::ActorInitInfo&, KoopaItemHolder*);
void initAndCreateDamageBallGroup(const al::ActorInitInfo&, KoopaItemHolder*,
                                  const sead::Matrix34f*);
void initAndCreateDamageBallBombGroup(const al::ActorInitInfo&, KoopaItemHolder*,
                                      const sead::Matrix34f*);
void initAndCreateDemoExecutorLv1(al::LiveActor*, const al::ActorInitInfo&, KoopaCameraCtrl*,
                                  al::LiveActor*, KoopaCap*, Peach*);
void initAndCreateDemoExecutorLv2(al::LiveActor*, const al::ActorInitInfo&, KoopaCameraCtrl*,
                                  al::LiveActor*, KoopaCap*, Peach*);
void initAndCreateDemoEventChurch(al::LiveActor*, const al::ActorInitInfo&);
const al::LiveActor* tryGetDemoKoopaSubActor(const al::LiveActor*);
const al::LiveActor* tryGetDemoPeachSubActor(const al::LiveActor*);
void validateTailSensors(al::LiveActor*);
bool isTailSensor(const al::HitSensor*);
void calcAnimPost(al::LiveActor*, KoopaCap*);
void startCapOnAnim(al::LiveActor*);
void startCapOffAnim(al::LiveActor*);
void updateFallVelocityToGravity(al::LiveActor*);
void updateOnGroundVelocity(al::LiveActor*);
void addVelocityToGravityFitCurvedSurface(al::LiveActor*, f32);
void turnKoopaToTrans(al::LiveActor*, const sead::Vector3f*);
void turnKoopaToPlayer(al::LiveActor*);
void initCapJointSideRotator(al::LiveActor*, f32*);
f32 getCapBlowDownParam();
void updateCapBlowDownSideDegree(f32*, const al::LiveActor*);
f32 getCapGravityTurnRate();
f32 getCapPushLimit();
s32 getLevel1();
s32 getLevel2();
s32 getLevel3();
bool isLevel1(const KoopaNumberCounter*);
bool isLevel2(const KoopaNumberCounter*);
bool isLevelGreater2(const KoopaNumberCounter*);
bool isLevel3(const KoopaNumberCounter*);
void onSwitchBattleKeepOn(al::LiveActor*);
void offSwitchBattleKeepOn(al::LiveActor*);
void onSwitchGraphicsLevelByHp(al::LiveActor*, s32);
void onSwitchGraphicsLevelBattleEnd(al::LiveActor*);
void createAndInitLinksKoopaDemoModelSkyWorld(const al::ActorInitInfo&, const char*, s32);
void startHitReactionPunchHitInvincible(al::LiveActor*);
}  // namespace KoopaFunction
