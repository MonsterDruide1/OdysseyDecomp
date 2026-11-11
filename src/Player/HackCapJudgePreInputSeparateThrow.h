#pragma once

#include "Player/IJudge.h"

class PlayerInput;
class PlayerSeparateCapFlag;

// Minimal: only the IJudge base is established so far (used by
// HackCap::tryChangeSeparateThrow via rs::resetJudge). The original passes
// the judge pointer to resetJudge with no offset adjustment, so IJudge is
// the first base. Size 0x20 verified from PlayerActorHakoniwa::initPlayer
// (round 18); members unknown.
class HackCapJudgePreInputSeparateThrow : public IJudge {
public:
    HackCapJudgePreInputSeparateThrow(const PlayerInput*, const PlayerSeparateCapFlag*);
    // Trivial out-of-line dtor, defined in the framework (FwPlayer.cpp).
    // (Not the vtable anchor — reset() is; see FwPlayer.cpp.) If the
    // decomp ever defines it, delete the framework definition.
    ~HackCapJudgePreInputSeparateThrow();

    void reset() override;
    void update() override;
    bool judge() const override;

private:
    u8 _00[0x20 - sizeof(IJudge)];
};

static_assert(sizeof(HackCapJudgePreInputSeparateThrow) == 0x20);
