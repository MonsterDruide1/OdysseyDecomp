#pragma once

namespace al {
class IUseSceneObjHolder;
class LiveActor;
}  // namespace al

class PlayerInputFunction {
public:
    static bool isTriggerAction(const al::LiveActor*, s32);
    static bool isHoldAction(const al::LiveActor*, s32);
    static bool isReleaseAction(const al::LiveActor*, s32);

    static bool isTriggerJump(const al::LiveActor*, s32);
    static bool isHoldJump(const al::LiveActor*, s32);
    static bool isReleaseJump(const al::LiveActor*, s32);

    static bool isTriggerSubAction(const al::LiveActor*, s32);
    static bool isHoldSubAction(const al::LiveActor*, s32);

    static bool isTriggerTalk(const al::LiveActor*, s32);
    static bool isTriggerStartWorldWarp(const al::LiveActor*, s32);
    static bool isTriggerCancelWorldWarp(const al::LiveActor*, s32);
};

namespace rs {

bool isSeparatePlay(const al::IUseSceneObjHolder*);

}
