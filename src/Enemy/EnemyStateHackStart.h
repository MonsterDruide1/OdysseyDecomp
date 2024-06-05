#pragma once

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SensorMsg;
class HitSensor;
}  // namespace al

class IUsePlayerHack;
class PlayerHackStartShaderCtrl;
struct PlayerHackStartShaderParam;

struct EnemyStateHackStartParam {
    const char* mActionName;
    const char* mVisAnimName;
    const char* mMtpAnimName;
    bool mHasSubActors;
    bool mUpdateSubActorShadowMap;
    EnemyStateHackStartParam(const char*, const char*, const char*, bool, bool);
};

class EnemyStateHackStart : public al::ActorStateBase {
public:
    EnemyStateHackStart(al::LiveActor*, const EnemyStateHackStartParam*,
                        PlayerHackStartShaderParam*);
    IUsePlayerHack* tryStart(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    void kill();
    bool isHackStart() const;
    f32 calcHackStartNerveRate() const;
    void exeDiveIn();
    void exeHackStart();

private:
    IUsePlayerHack* mHackActor = nullptr;
    PlayerHackStartShaderCtrl* mPlayerHackStartShaderCtrl = nullptr;
    const EnemyStateHackStartParam* mParam;
};

namespace EnemyStateHackFunction {
void startHackSwitchShadow(al::LiveActor*, const EnemyStateHackStartParam*);
void endHackSwitchShadow(al::LiveActor*, const EnemyStateHackStartParam*);
}  // namespace EnemyStateHackFunction
