#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class HitSensor;
}

class PlayerEyeSensorHitHolder {
public:
    PlayerEyeSensorHitHolder(al::LiveActor* actor);

    void update();
    bool isHit() const;
    const sead::Vector3f& getHitPos() const;

private:
    al::LiveActor* mActor;
    bool mIsHit;
    char filler[3];
    sead::Vector3f mHitPos;
    char filler2[8];
};

static_assert(sizeof(PlayerEyeSensorHitHolder) == 0x20);
