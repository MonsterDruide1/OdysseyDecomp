#pragma once

#include <basis/seadTypes.h>

#include "Library/Obj/PartsModel.h"

namespace al {
class LiveActor;
struct ActorInitInfo;
class HitSensor;
}  // namespace al

class GolemClimbWeakPoint : public al::PartsModel {
public:
    GolemClimbWeakPoint(al::LiveActor*, const al::ActorInitInfo&, const char*, const char*,
                        const char*, const char*, const char*, bool, bool);
    void appear() override;
    void kill() override;
    void exeWait();
    void exePanic();
    void exeDamage();
    void exeBreak();
    void exeBlowDown();
    void exeDemo();
    void setWait();
    void setPanic();
    void receiveMsgThrust(al::HitSensor*, al::HitSensor*);
    void receiveMsgHipDrop(al::HitSensor*, al::HitSensor*);
    void startBreak();
    void startDemo(const char*);
    bool isBreak() const;
};
