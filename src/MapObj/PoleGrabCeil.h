#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
struct ActorInitInfo;
class HitSensor;
class KeyMoveFollowTarget;
class MtxConnector;
class SensorMsg;
}  // namespace al

class PoleGrabCeil : public al::LiveActor {
public:
    PoleGrabCeil(const char* name);
    void init(const al::ActorInitInfo& info) override;
    void initAfterPlacement() override;
    void control() override;
    void exeWait();
    void exeGrab();
    void exeGrabEnd();
    void attackSensor(al::HitSensor* self, al::HitSensor* other) override;
    bool receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                    al::HitSensor* self) override;

private:
    sead::Vector3f mSnapSensorPos = {0.0f, 0.0f, 0.0f};
    al::MtxConnector* mMtxConnector = nullptr;
    al::KeyMoveFollowTarget* mKeyMoveFollowTarget = nullptr;
    bool mIsConnectPose = false;
    bool mIsCollisionCheck = false;
    sead::Matrix34f mSurfaceMatrix = sead::Matrix34f::ident;
};

static_assert(sizeof(PoleGrabCeil) == 0x160);
