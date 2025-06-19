#pragma once

#include <basis/seadTypes.h>

namespace al {
class ActionAnimCtrl;
class ActionBgmCtrl;
class ActionEffectCtrl;
class ActionFlagCtrl;
class ActionPadAndCameraCtrl;
class ActionSeCtrl;
class ActionScreenEffectCtrl;
class ActorResource;
class LiveActor;
class NerveActionCtrl;

class ActorActionKeeper {
public:
    static ActorActionKeeper* tryCreate(LiveActor* actor, const ActorResource* actorRes,
                                        const char* string0, const char* string1);

    ActorActionKeeper(LiveActor* parentActor, const char* actorName, ActionAnimCtrl* animCtrl,
                      NerveActionCtrl* nrvActionCtrl, ActionFlagCtrl* flagCtrl,
                      ActionEffectCtrl* effectCtrl, ActionSeCtrl* seCtrl, ActionBgmCtrl* bgmCtrl,
                      ActionPadAndCameraCtrl* padAndCamCtrl,
                      ActionScreenEffectCtrl* screenEffectCtrl);

    bool startAction(const char* name);
    void tryStartActionNoAnim(const char* string);
    void init();
    void updatePost();
    void updatePrev();

    ActionAnimCtrl* getAnimCtrl() const { return mAnimCtrl; }

    ActionSeCtrl* getSeCtrl() const { return mSeCtrl; }

    ActionPadAndCameraCtrl* getPadAndCameraCtrl() const { return mPadAndCameraCtrl; }

private:
    LiveActor* mParentActor;
    const char* mActorName;
    bool mIsActionRunning = false;
    ActionAnimCtrl* mAnimCtrl;
    NerveActionCtrl* mNerveActionCtrl;
    ActionFlagCtrl* mFlagCtrl;
    ActionEffectCtrl* mEffectCtrl;
    ActionSeCtrl* mSeCtrl;
    ActionBgmCtrl* mBgmCtrl;
    ActionPadAndCameraCtrl* mPadAndCameraCtrl;
    ActionScreenEffectCtrl* mScreenEffectCtrl;
};

static_assert(sizeof(ActorActionKeeper) == 0x58);
}  // namespace al
