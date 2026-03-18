#pragma once

namespace al {
class LiveActor;
}

class KoopaCap;

class KoopaFlag {
public:
    bool tryStartPunchHitFirst();

    bool _0 = false;
    bool _1 = false;
    bool _2 = false;
    bool _3 = false;
    bool _4 = false;
    bool _5 = false;
    bool _6 = false;
    bool _7 = true;
};

class KoopaNumberCounter {
public:
    void startPunchHitAction(al::LiveActor* actor) const;
    void recoverPunchHitCount(KoopaCap* cap);
};

namespace KoopaFunction {
void updateOnGroundVelocity(al::LiveActor* actor);
}
