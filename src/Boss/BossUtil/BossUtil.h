#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadEnum.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class BarrierField;

namespace rs {

SEAD_ENUM(BossType, cStacker, cCapThrower, cBombTail, cFireBlower, 
                    cBossKnuckle, cBossForest, cKoopa, cMofumofu, 
                    cGiantWanderBoss, cBossRaid, cGolemClimb, 
                    cBossMagma, cBreeda);

BarrierField* tryCreateBarrierField(const al::ActorInitInfo& info);
void updateEyeMove(al::LiveActor* actor, const sead::Vector3f& target, f32 maxAngle,
                   const char* animName);
void resetEyeMove(al::LiveActor* actor, const char* animName);
void startBossBattle(const al::LiveActor* actor, s32 bossType);
void endBossBattle(const al::LiveActor* actor, s32 bossType);
s32 getBossBattleDeadCount(const al::LiveActor* actor, s32 bossType);
bool isAlreadyShowDemoBossBattleStart(const al::LiveActor* actor, s32 bossType, s32 level);
void saveShowDemoBossBattleStart(const al::LiveActor* actor, s32 bossType, s32 level);
bool isAlreadyShowDemoBossBattleEndKoopaLv2(const al::LiveActor* actor);
void saveShowDemoBossBattleEndKoopaLv2(const al::LiveActor* actor);
bool isAlreadyShowDemoMoonBasementCollapse(const al::LiveActor* actor);
void saveShowDemoMoonBasementCollapse(const al::LiveActor* actor);
bool isAlreadyDeadGK(const al::LiveActor* actor, s32 bossType, s32 level);
void onAlreadyDeadGK(const al::LiveActor* actor, s32 bossType, s32 level);

}  // namespace rs
