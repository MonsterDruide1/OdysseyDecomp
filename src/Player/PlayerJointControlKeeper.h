#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class HitSensor;
class LiveActor;
}  // namespace al
class PlayerConst;

class PlayerJointControlKeeper {
public:
    PlayerJointControlKeeper(al::LiveActor*, const PlayerConst*, bool, bool, bool);

    void update();
    void updateLookAt();
    void updateSwimPose();
    void updateCenter();
    void updateGroundPose();
    void updateCapThrow();
    void updatePoleClimb();
    void updateGrab();
    void updatePartsDynamics();
    void calcLookAtEyePos(sead::Vector3f*) const;
    void resetPartsDynamics();
    void updateLookAtTarget(al::HitSensor*);
    void calcGroundPoseUp(sead::Vector3f*) const;
    f32 getHeadTiltRate() const;

private:
    void* filler[0xF0 / 8];
};

static_assert(sizeof(PlayerJointControlKeeper) == 0xF0);
