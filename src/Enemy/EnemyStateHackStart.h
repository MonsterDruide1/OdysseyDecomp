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
    EnemyStateHackStartParam(const char* actionName, const char* visAnimName,
                             const char* mtpAnimName, bool hasSubActors,
                             bool isUpdateSubActorShadowMap);

    const char* actionName;
    const char* visAnimName;
    const char* mtpAnimName;
    bool hasSubActors;
    bool isUpdateSubActorShadowMap;
};

class EnemyStateHackStart : public al::ActorStateBase {
public:
    EnemyStateHackStart(al::LiveActor* actor, const EnemyStateHackStartParam* param,
                        PlayerHackStartShaderParam* shaderParam);

    IUsePlayerHack* tryStart(const al::SensorMsg* msg, al::HitSensor* other, al::HitSensor* self);
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
void startHackSwitchShadow(al::LiveActor* actor, const EnemyStateHackStartParam* param);
void endHackSwitchShadow(al::LiveActor* actor, const EnemyStateHackStartParam* param);
}  // namespace EnemyStateHackFunction
