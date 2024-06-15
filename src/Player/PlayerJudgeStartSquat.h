#pragma once

#include "Library/HostIO/HioNode.h"

#include "Player/IJudge.h"

class PlayerInput;
class PlayerCounterForceRun;
class PlayerCarryKeeper;

class PlayerJudgeStartSquat : public al::HioNode, public IJudge {
public:
    PlayerJudgeStartSquat(const PlayerInput*, const PlayerCounterForceRun*,
                          const PlayerCarryKeeper*);
    void reset();
    void update();
    bool judge() const;

private:
    const PlayerInput* mInput;
    const PlayerCounterForceRun* mCounterForceRun;
    const PlayerCarryKeeper* mCarryKeeper;
};
static_assert(sizeof(PlayerJudgeStartSquat) == 0x20);
