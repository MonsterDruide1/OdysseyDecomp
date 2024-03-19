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

        LiveActor *mParentActor;
        const char *mActorName;
        s8 mByteUnkn = 0;
        ActionAnimCtrl *mAnimCtrl;
        NerveActionCtrl *mNrvActionCtrl;
        ActionFlagCtrl *mFlagCtrl;
        ActionEffectCtrl *mEffectCtrl;
        ActionSeCtrl *mSeCtrl;
        ActionBgmCtrl *mBgmCtrl;
        ActionPadAndCameraCtrl *mPadAndCamCtrl;
        ActionScreenEffectCtrl *mScreenEffectCtrl;
    };

    static_assert(sizeof(ActorActionKeeper) == 0x58, "");
}
