#pragma once

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class BreakModel;
class CollisionObj;
}  // namespace al

class IMoonBasementGateParts {
public:
    virtual void validate() = 0;
    virtual void moveSlideToTarget(const sead::Vector3f& targetTrans) = 0;
};

class MoonBasementGateParts : public al::LiveActor, public IMoonBasementGateParts {
public:
    MoonBasementGateParts(const char* name);

    void initParts(const al::ActorInitInfo& info, const char* modelName, const char* breakModelName,
                   const sead::Vector3f& trans, bool isStartInvalid);

    void kill() override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

    void validate() override;
    void moveSlideToTarget(const sead::Vector3f& targetTrans) override;

    void setPrevParts(IMoonBasementGateParts* prevParts) { mPrevParts = prevParts; }

    void exeInvalid();
    void exeWait();
    void exeBreakDelay();

private:
    al::CollisionObj* mCollisionObj = nullptr;
    al::BreakModel* mBreakModel = nullptr;
    IMoonBasementGateParts* mPrevParts = nullptr;
};

static_assert(sizeof(MoonBasementGateParts) == 0x128);
