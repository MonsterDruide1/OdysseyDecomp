#pragma once

#include <basis/seadTypes.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class HitSensor;
class LiveActor;
class SensorMsg;
}  // namespace al

class IUsePlayerHack;
class PlayerHackStartShaderCtrl;
struct PlayerHackStartShaderParam;

struct EnemyStateHackStartParam {
    EnemyStateHackStartParam(const char*, const char*, const char*, bool, bool);

    const char* actionName;
    const char* visAnimName;
    const char* mtpAnimName;
    bool hasSubActors;
    bool updateSubActorShadowMap;
};

class EnemyStateHackStart : public al::ActorStateBase {
public:
    EnemyStateHackStart(al::LiveActor*, const EnemyStateHackStartParam*,
                        PlayerHackStartShaderParam*);

    IUsePlayerHack* tryStart(const al::SensorMsg*, al::HitSensor*, al::HitSensor*);
    void kill() override;
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
