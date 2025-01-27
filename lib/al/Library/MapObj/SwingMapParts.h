#pragma once

#include "Library/LiveActor/LiveActor.h"
#include "Library/Math/Axis.h"

namespace al {
class SwingMovement;

class SwingMapParts : public LiveActor {
public:
    SwingMapParts(const char* name);

    void init(const ActorInitInfo& info) override;
    void initAfterPlacement() override;
    bool receiveMsg(const SensorMsg* message, HitSensor* other, HitSensor* self) override;
    void control() override;

    void start();

    void exeStandBy();
    void exeMoveRight();
    void exeMoveLeft();
    void exeStop();

private:
    sead::Quatf mQuat = sead::Quatf::unit;
    SwingMovement* mSwingMovement = nullptr;
    Axis mRotateAxis = Axis::None;
    bool mIsFloorTouchStart = false;
    sead::Vector3f mDepthShadowLength = {0.0f, 0.0f, 0.0f};
};

static_assert(sizeof(SwingMapParts) == 0x138);
}  // namespace al
