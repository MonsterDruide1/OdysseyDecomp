#pragma once

#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class MtxConnector;
class SensorMsg;
}  // namespace al

class BalloonIcon;

class ShopMark : public al::LiveActor {
public:
    ShopMark(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void startClipped() override;
    void endClipped() override;
    void control() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    BalloonIcon* mBalloonIcon = nullptr;
    al::MtxConnector* mMtxConnector = nullptr;
    sead::Matrix34f mBalloonMtx = sead::Matrix34f::ident;
};

static_assert(sizeof(ShopMark) == 0x148);
