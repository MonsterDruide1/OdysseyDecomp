#pragma once

#include <math/seadQuat.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

#include "MapObj/MoonBasementGateParts.h"

namespace al {
class BreakModel;
class CameraTicket;
class CollisionObj;
}  // namespace al

class MoonBasementGate : public al::LiveActor, public IMoonBasementGateParts {
public:
    MoonBasementGate(const char* name);

    void init(const al::ActorInitInfo& info) override;
    void kill() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void validate() override;
    void moveSlideToTarget(const sead::Vector3f& targetTrans) override;

    void exeInvalid();
    void exeWait();
    void exeBreak();
    void exeBreakAfter();
    void exeClimaxCamera();

private:
    s32 mBreakCount = 0;
    al::BreakModel* mBreakModel = nullptr;
    al::CollisionObj* mCollisionObj = nullptr;
    MoonBasementGateParts** mParts = nullptr;
    al::CameraTicket* mClimaxCameraTicket = nullptr;
    sead::Quatf mClimaxAfterKoopaQuat = sead::Quatf::unit;
    sead::Vector3f mClimaxAfterKoopaTrans = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(MoonBasementGate) == 0x158);
