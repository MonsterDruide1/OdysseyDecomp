#pragma once

namespace al {
class LiveActor;
}

class PlayerInputFunction {
public:
    static bool isTriggerAction(const al::LiveActor*, int);
    static bool isHoldAction(const al::LiveActor*, int);
    static bool isReleaseAction(const al::LiveActor*, int);

    static bool isTriggerJump(const al::LiveActor*, int);
    static bool isHoldJump(const al::LiveActor*, int);
    static bool isReleaseJump(const al::LiveActor*, int);

    static bool isTriggerSubAction(const al::LiveActor*, int);
    static bool isHoldSubAction(const al::LiveActor*, int);

    static bool isTriggerTalk(const al::LiveActor*, int);
    static bool isTriggerStartWorldWarp(const al::LiveActor*, int);
    static bool isTriggerCancelWorldWarp(const al::LiveActor*, int);
};
