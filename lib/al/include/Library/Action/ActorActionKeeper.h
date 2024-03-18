#pragma once

#include <basis/seadTypes.h>

namespace al {
    class LiveActor;
    class ActionAnimCtrl;
    class NerveActionCtrl;
    class ActionFlagCtrl;
    class ActionEffectCtrl;
    class ActionSeCtrl;
    class ActionBgmCtrl;
    class ActionPadAndCameraCtrl;
    class ActionScreenEffectCtrl;
    class ActorResource;

    class ActorActionKeeper {
    public:
        ActorActionKeeper(
            LiveActor*, char const*, ActionAnimCtrl*, NerveActionCtrl*,
            ActionFlagCtrl*, ActionEffectCtrl*, ActionSeCtrl*,
            ActionBgmCtrl*, ActionPadAndCameraCtrl*, ActionScreenEffectCtrl*
        );

        void init();
        void startAction(char const *actionName);
        void tryStartActionNoAnim(char const *actionName);
        void updatePost();
        void updatePrev();

        static ActorActionKeeper* tryCreate(
            LiveActor*, ActorResource const*,
            char const*, char const*
        );

        LiveActor *mParentActor; // 0x0
        const char *mActorName; // 0x8
        s8 mByteUnkn = 0; // 0x10
        ActionAnimCtrl *mAnimCtrl; // 0x18
        NerveActionCtrl *mNrvActionCtrl; // 0x20
        ActionFlagCtrl *mFlagCtrl; // 0x28
        ActionEffectCtrl *mEffectCtrl; // 0x30
        ActionSeCtrl *mSeCtrl; // 0x38
        ActionBgmCtrl *mBgmCtrl; // 0x40
        ActionPadAndCameraCtrl *mPadAndCamCtrl; // 0x48
        ActionScreenEffectCtrl *mScreenEffectCtrl; // 0x50
    };

    static_assert(sizeof(ActorActionKeeper) == 0x58, "");
}
