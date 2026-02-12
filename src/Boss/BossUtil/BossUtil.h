#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class BarrierField;

namespace rs {
BarrierField* tryCreateBarrierField(const al::ActorInitInfo&);
void updateEyeMove(al::LiveActor*, const sead::Vector3f&, f32, const char*);
void resetEyeMove(al::LiveActor*, const char*);
void startBossBattle(const al::LiveActor*, s32);
void endBossBattle(const al::LiveActor*, s32);
s32 getBossBattleDeadCount(const al::LiveActor*, s32);
bool isAlreadyShowDemoBossBattleStart(const al::LiveActor*, s32, s32);
void saveShowDemoBossBattleStart(const al::LiveActor*, s32, s32);
bool isAlreadyShowDemoBossBattleEndKoopaLv2(const al::LiveActor*);
void saveShowDemoBossBattleEndKoopaLv2(const al::LiveActor*);
bool isAlreadyShowDemoMoonBasementCollapse(const al::LiveActor*);
void saveShowDemoMoonBasementCollapse(const al::LiveActor*);
bool isAlreadyDeadGK(const al::LiveActor*, s32, s32);
void onAlreadyDeadGK(const al::LiveActor*, s32, s32);
}  // namespace rs
