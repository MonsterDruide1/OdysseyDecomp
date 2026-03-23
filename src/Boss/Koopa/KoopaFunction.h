#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class LiveActorGroup;
struct ActorInitInfo;
class HitSensor;
struct EnemyStateBlowDownParam;
}  // namespace al

class KoopaCap;
class KoopaNumberCounter;
class KoopaItemHolder;
class KoopaCameraCtrl;
class KoopaDemoExecutor;
class KoopaDemoDummy;
class Peach;

namespace KoopaFunction {
void initActorKoopa(al::LiveActor*, const al::ActorInitInfo&);
void invalidateTailSensors(al::LiveActor*);
void initActorKoopaDemoModel(al::LiveActor*, const al::ActorInitInfo&);
al::LiveActor* initAndCreateDemoModel(const al::ActorInitInfo&);
Peach* initAndCreateLinksPeach(const al::ActorInitInfo&);
al::LiveActor* initAndCreateDemoPeachRing(const al::ActorInitInfo&);
al::LiveActorGroup* initAndCreateWeaponCapGroup(const al::ActorInitInfo&, KoopaItemHolder*);
al::LiveActorGroup* initAndCreateDamageBallGroup(const al::ActorInitInfo&, KoopaItemHolder*,
                                                 const sead::Matrix34f*);
al::LiveActorGroup* initAndCreateDamageBallBombGroup(const al::ActorInitInfo&, KoopaItemHolder*,
                                                     const sead::Matrix34f*);
KoopaDemoExecutor* initAndCreateDemoExecutorLv1(al::LiveActor*, const al::ActorInitInfo&,
                                                KoopaCameraCtrl*, al::LiveActor*, KoopaCap*,
                                                Peach*);
KoopaDemoExecutor* initAndCreateDemoExecutorLv2(al::LiveActor*, const al::ActorInitInfo&,
                                                KoopaCameraCtrl*, al::LiveActor*, KoopaCap*,
                                                Peach*);
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
const al::EnemyStateBlowDownParam& getCapBlowDownParam();
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
KoopaDemoDummy* createAndInitLinksKoopaDemoModelSkyWorld(const al::ActorInitInfo&, const char*,
                                                         s32);
void startHitReactionPunchHitInvincible(al::LiveActor*);
}  // namespace KoopaFunction
