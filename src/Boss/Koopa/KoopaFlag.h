#pragma once

namespace al {
class LiveActor;
}

class KoopaFlag {
public:
    KoopaFlag();
    bool tryStartPunchHitFirst();
    bool trySkipJumpSign();

    inline bool hasHitEnd() const { return mHasHitEnd; }

    inline void setHasHitEnd(bool v) { mHasHitEnd = v; }

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
