#pragma once

namespace al {
class LiveActor;
}

class KoopaFlag {
public:
    KoopaFlag();
    bool tryStartPunchHitFirst();
    bool trySkipJumpSign();

    bool hasHitEnd() const { return mHasHitEnd; }

    void setHasHipDropLandEnd() { _2 = true; }

    void setHasHitEnd() { mHasHitEnd = true; }

private:
    bool _0 = false;
    bool _1 = false;
    bool _2 = false;
    bool mHasHitEnd = false;
    bool _4 = false;
    bool _5 = false;
    bool _6 = false;
    bool _7 = true;
};

static_assert(sizeof(KoopaFlag) == 0x8);
