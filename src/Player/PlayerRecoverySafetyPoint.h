#pragma once

#include <math/seadVector.h>
#include "Library/LiveActor/ActorInitInfo.h"
#include "Player/HackCap.h"
#include "Util/IUseDimension.h"

class PlayerRecoverySafetyPoint {
public:
    PlayerRecoverySafetyPoint(al::LiveActor const*,HackCap const*,al::ActorInitInfo const&,IUseDimension const*,al::CollisionPartsFilterBase *,al::HitSensor *);
    void updateRecoveryBubble();
    sead::Vector3f* getSafetyPoint() const;
    sead::Vector3f* getSafetyPointGravity() const;
    sead::Vector3f* getSafetyPointArea() const;
};
