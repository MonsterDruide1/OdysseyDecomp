#pragma once

#include "Library/LiveActor/LiveActor.h"

namespace al {
class MtxConnector;
}
class IUsePlayerPuppet;

class ShineTowerBackDoor : public al::LiveActor {
public:
    ShineTowerBackDoor(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void attachToHostJoint(const al::LiveActor* actor, const char* jointName);
    bool isEnter() const;

    void exeWait();
    void exeEnter();
    void exeReaction();
    void exeReactionWait();

private:
    al::MtxConnector* mMtxConnector = nullptr;
    bool _118 = false;
    s32 mBindTimer = 0;
    IUsePlayerPuppet* mPlayerPuppet = nullptr;
};
