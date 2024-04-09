#include "Library/Action/ActorActionKeeper.h"

#include "Library/Base/Base.h"
#include "Library/Base/String.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveExecutor.h"
#include "Library/Nerve/NerveKeeper.h"
#include "Library/Nerve/NerveUtil.h"
#include "Project/Action/ActionAnimCtrl.h"
#include "Project/Action/ActionBgmCtrl.h"
#include "Project/Action/ActionEffectCtrl.h"
#include "Project/Action/ActionFlagCtrl.h"
#include "Project/Action/ActionPadAndCameraCtrl.h"
#include "Project/Action/ActionScreenEffectCtrl.h"
#include "Project/Action/ActionSeCtrl.h"

namespace al {
ActorActionKeeper::ActorActionKeeper(LiveActor* parentActor, char const* actorName,
                                     ActionAnimCtrl* animCtrl, NerveActionCtrl* nrvActionCtrl,
                                     ActionFlagCtrl* flagCtrl, ActionEffectCtrl* effectCtrl,
                                     ActionSeCtrl* seCtrl, ActionBgmCtrl* bgmCtrl,
                                     ActionPadAndCameraCtrl* padAndCamCtrl,
                                     ActionScreenEffectCtrl* screenEffectCtrl)

    : mParentActor(parentActor), mActorName(actorName), mAnimCtrl(animCtrl),
      mNerveActionCtrl(nrvActionCtrl), mFlagCtrl(flagCtrl), mEffectCtrl(effectCtrl),
      mSeCtrl(seCtrl), mBgmCtrl(bgmCtrl), mPadAndCameraCtrl(padAndCamCtrl),
      mScreenEffectCtrl(screenEffectCtrl) {}

ActorActionKeeper* ActorActionKeeper::tryCreate(LiveActor* actor, const ActorResource* actorRes,
                                                const char* string0, const char* string1) {
    if (!actor->getModelKeeper()) {
        if (!actor->getNerveKeeper())
            return nullptr;
        if (!actor->getNerveKeeper()->getActionCtrl())
            return nullptr;
    }

    const char* name = createStringIfInStack(getBaseName(string0));

    ActionAnimCtrl* newAnimCtrl = ActionAnimCtrl::tryCreate(actor, actorRes, name, string1);
    NerveActionCtrl* newNerveCtrl =
        actor->getNerveKeeper() ? actor->getNerveKeeper()->getActionCtrl() : nullptr;

    ActionFlagCtrl* newFlagCtrl = ActionFlagCtrl::tryCreate(actor, string1);
    ActionEffectCtrl* newEffectCtrl = ActionEffectCtrl::tryCreate(actor);
    ActionSeCtrl* newSeCtrl = ActionSeCtrl::tryCreate(actor->getAudioKeeper());
    ActionBgmCtrl* newBgmCtrl = ActionBgmCtrl::tryCreate(actor->getAudioKeeper());
    ActionPadAndCameraCtrl* newPadAndCameraCtrl =
        ActionPadAndCameraCtrl::tryCreate(actor, actorRes, getTransPtr(actor), string1);

    ActionScreenEffectCtrl* newScreenEffectCtrl = ActionScreenEffectCtrl::tryCreate(actor, string1);
    ActorActionKeeper* newMe = nullptr;

    if (newAnimCtrl || newFlagCtrl || newEffectCtrl || newSeCtrl || newBgmCtrl ||
        newPadAndCameraCtrl || newScreenEffectCtrl) {
        newMe = new ActorActionKeeper(actor, name, newAnimCtrl, newNerveCtrl, newFlagCtrl,
                                      newEffectCtrl, newSeCtrl, newBgmCtrl, newPadAndCameraCtrl,
                                      newScreenEffectCtrl);
    }
    return newMe;
}

void ActorActionKeeper::init() {
    if (mFlagCtrl) {
        mFlagCtrl->initPost();
    }
}
bool ActorActionKeeper::startAction(char const* name) {
    mIsActionRunning = true;
    if (!mNerveActionCtrl)
        tryStartActionNoAnim(name);
    if (mAnimCtrl)
        return mAnimCtrl->start(name);
    return false;
}
void ActorActionKeeper::tryStartActionNoAnim(const char* string) {
    if (mFlagCtrl)
        mFlagCtrl->start(string);
    if (mEffectCtrl)
        mEffectCtrl->startAction(string);
    if (mSeCtrl)
        mSeCtrl->startAction(string);
    if (mBgmCtrl)
        mBgmCtrl->startAction(string);
    if (mPadAndCameraCtrl)
        mPadAndCameraCtrl->startAction(string);
    if (mScreenEffectCtrl)
        mScreenEffectCtrl->startAction(string);
}
void ActorActionKeeper::updatePrev() {}
}  // namespace al
