#pragma once

#include <basis/seadTypes.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class CollisionObj;
class HitSensor;
class MtxConnector;
class SensorMsg;
class SimpleAudioUser;
}  // namespace al

class KuriboTowerSwitch : public al::LiveActor {
public:
    KuriboTowerSwitch(const char* name);
    void init(const al::ActorInitInfo& initInfo) override;
    void initAfterPlacement() override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;
    bool isReactionNerve() const;
    void exeDisable();
    void exeOffWait();
    void exeReactionLand();
    void exeOn();
    void exeOnWait();

private:
    al::MtxConnector* mMtxConnector = nullptr;
    s32 mNum = 10;
    al::SimpleAudioUser* mAudioUser = nullptr;
    al::CollisionObj* mCollisionObj = nullptr;
    bool _128 = true;
    bool _129 = false;
    u32 mDisplayNum = 4;
};

static_assert(sizeof(KuriboTowerSwitch) == 0x130);
