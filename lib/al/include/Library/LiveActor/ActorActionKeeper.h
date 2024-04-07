#pragma once

// TODO: includes ActorActionKeeper.h

namespace al {
    class ActorActionKeeper {
    public:
        al::LiveActor *mParentActor; // 0x0
        char *mActorName; // 0x8
        undefined ukn[8]; // 0x10
        al::ActionAnimCtrl *mAnimCtrl; // 0x18
        al::NerveActionCtrl *mNrvActionCtrl; // 0x20
        al::ActionFlagCtrl *mFlagCtrl; // 0x28
        al::ActionEffectCtrl *mEffectCtrl; // 0x30
        al::ActionSeCtrl *mSeCtrl; // 0x38
        al::ActionBgmCtrl *mBgmCtrl; // 0x40
        al::ActionPadAndCameraCtrl *mPadAndCamCtrl; // 0x48
        al::ActionScreenEffectCtrl *mScreenEffectCtrl; // 0x50

        ActorActionKeeper(
            al::LiveActor *parentActor,
            char const *actorName,
            al::ActionAnimCtrl *animCtrl,
            al::NerveActionCtrl *nrvActionCtrl,
            al::ActionFlagCtrl *flagCtrl,
            al::ActionEffectCtrl *effectCtrl,
            al::ActionSeCtrl *seCtrl,
            al::ActionBgmCtrl *bgmCtrl,
            al::ActionPadAndCameraCtrl *padAndCamCtrl,
            al::ActionScreenEffectCtrl *screenEffectCtrl
        );

        void init();
        void startAction(char const *actionName);
        void tryStartActionNoAnim(char const *actionName);
        void updatePost();
        void updatePrev();

        static al::ActorActionKeeper* tryCreate(
            al::LiveActor *actor,
            al::ActorResource const *actorResource,
            char const *uknStr1,
            char const *uknStr2
        );
    };

    static_assert(sizeof(ActorActionKeeper) == 0x58, "");
}
