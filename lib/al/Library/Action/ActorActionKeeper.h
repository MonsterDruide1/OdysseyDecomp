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
    static ActorActionKeeper* tryCreate(LiveActor*, const ActorResource*, const char*, const char*);

    ActorActionKeeper(LiveActor*, const char*, ActionAnimCtrl*, NerveActionCtrl*, ActionFlagCtrl*,
                      ActionEffectCtrl*, ActionSeCtrl*, ActionBgmCtrl*, ActionPadAndCameraCtrl*,
                      ActionScreenEffectCtrl*);

    bool startAction(const char* actionName);
    void tryStartActionNoAnim(const char* actionName);
    void init();
    void updatePost();
    void updatePrev();

    ActionAnimCtrl* getAnimCtrl() const { return mAnimCtrl; }

    ActionSeCtrl* getSeCtrl() const { return mSeCtrl; }

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
