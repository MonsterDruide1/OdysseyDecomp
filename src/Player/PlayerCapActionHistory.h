#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
}
class PlayerConst;
class PlayerTrigger;
class IUsePlayerCollision;
class PlayerCounterAfterCapCatch;

class PlayerCapActionHistory {
public:
    PlayerCapActionHistory(const al::LiveActor* player, const PlayerConst* pConst,
                           const PlayerTrigger* trigger, const IUsePlayerCollision* collider);
    void update();
    void clearLandLimit();
    void clearLimitHeight();
    void clearCapJump();
    void clearLandLimitStandAngle();
    void clearWallAirLimit();
    void recordLimitHeight();
    bool isOverLimitHeight() const;

public:
    const al::LiveActor* mPlayer;
    const PlayerConst* mConst;
    const PlayerTrigger* mTrigger;
    const IUsePlayerCollision* mCollision;
    PlayerCounterAfterCapCatch* mCounterAfterCapCatch;
    bool mIsLimitHeight = false;
    sead::Vector3f mHeightLimit = sead::Vector3f::zero;
    bool mIsCapJumpPossible = true;
    // seems to be always set to `true`
    bool _39 = true;
};
