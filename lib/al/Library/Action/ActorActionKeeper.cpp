#include "Library/Action/ActorActionKeeper.h"

#include "Library/Base/HashCodeUtil.h"
#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
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
ActorActionKeeper::ActorActionKeeper(LiveActor* parentActor, const char* actorName,
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

    ActionAnimCtrl* newAnimCtrl;
    NerveActionCtrl* newNerveCtrl;
    ActionFlagCtrl* newFlagCtrl;
    ActionEffectCtrl* newEffectCtrl;
    ActionSeCtrl* newSeCtrl;
    ActionBgmCtrl* newBgmCtrl;
    ActionPadAndCameraCtrl* newPadAndCameraCtrl;
    ActionScreenEffectCtrl* newScreenEffectCtrl;

    newAnimCtrl = ActionAnimCtrl::tryCreate(actor, actorRes, name, string1);
    newNerveCtrl = actor->getNerveKeeper() ? actor->getNerveKeeper()->getActionCtrl() : nullptr;
    newFlagCtrl = ActionFlagCtrl::tryCreate(actor, string1);
    newEffectCtrl = ActionEffectCtrl::tryCreate(actor);
    newSeCtrl = ActionSeCtrl::tryCreate(actor->getAudioKeeper());
    newBgmCtrl = ActionBgmCtrl::tryCreate(actor->getAudioKeeper());
    newPadAndCameraCtrl =
        ActionPadAndCameraCtrl::tryCreate(actor, actorRes, getTransPtr(actor), string1);
    newScreenEffectCtrl = ActionScreenEffectCtrl::tryCreate(actor, string1);

    if (newAnimCtrl || newFlagCtrl || newEffectCtrl || newSeCtrl || newBgmCtrl ||
        newPadAndCameraCtrl || newScreenEffectCtrl) {
        return new ActorActionKeeper(actor, name, newAnimCtrl, newNerveCtrl, newFlagCtrl,
                                     newEffectCtrl, newSeCtrl, newBgmCtrl, newPadAndCameraCtrl,
                                     newScreenEffectCtrl);
    } else {
        return nullptr;
    }
}

void ActorActionKeeper::init() {
    if (mFlagCtrl)
        mFlagCtrl->initPost();
}

bool ActorActionKeeper::startAction(const char* action) {
    mIsActionRunning = true;
    if (!mNerveActionCtrl)
        tryStartActionNoAnim(action);

    return mAnimCtrl && mAnimCtrl->start(action);
}

void ActorActionKeeper::tryStartActionNoAnim(const char* action) {
    if (mFlagCtrl)
        mFlagCtrl->start(action);
    if (mEffectCtrl)
        mEffectCtrl->startAction(action);
    if (mSeCtrl)
        mSeCtrl->startAction(action);
    if (mBgmCtrl)
        mBgmCtrl->startAction(action);
    if (mPadAndCameraCtrl)
        mPadAndCameraCtrl->startAction(action);
    if (mScreenEffectCtrl)
        mScreenEffectCtrl->startAction(action);
}

void ActorActionKeeper::updatePost() {
    if (!mEffectCtrl && !mSeCtrl && !mBgmCtrl && !mPadAndCameraCtrl && !mScreenEffectCtrl) {
        mIsActionRunning = false;
        return;
    }

    if (mNerveActionCtrl && isNewNerve(mParentActor)) {
        mIsActionRunning = false;
        return;
    }

    f32 frame;
    f32 frameRateMax;
    f32 frameRate;
    bool isRunning;

    LiveActor* actor = mParentActor;
    if (mNerveActionCtrl) {
        frame = actor->getNerveKeeper()->getCurrentStep() - 1;
        frameRate = 1.0f;
        frameRateMax = -1.0f;
        isRunning = true;
    } else {
        bool isActionRunning = mIsActionRunning;
        if (getActionName(actor)) {
            frame = getActionFrame(actor);
            frameRate = getActionFrameRate(actor);
            frameRateMax = getActionFrameMax(actor);
            isRunning = isActionRunning || isActionOneTime(actor);
        } else {
            frame = getActionFrame(actor);
            frameRate = getActionFrameRate(actor);
            frameRateMax = -1.0f;
            isRunning = true;
        }
    }

    bool isStop = !isRunning;
    if (mFlagCtrl)
        mFlagCtrl->update(frame, frameRateMax, frameRate, isStop);
    if (mEffectCtrl)
        mEffectCtrl->update(frame, frameRateMax, frameRate, isStop);
    if (mSeCtrl)
        mSeCtrl->update(frame, frameRateMax, frameRate, isStop);
    if (mBgmCtrl)
        mBgmCtrl->update(frame, frameRateMax, frameRate, isStop);
    if (mPadAndCameraCtrl)
        mPadAndCameraCtrl->update(frame, frameRateMax, frameRate, isStop);
    if (mScreenEffectCtrl)
        mScreenEffectCtrl->update(frame, frameRateMax, frameRate, isStop);
    mIsActionRunning = false;
}

void ActorActionKeeper::updatePrev() {}
}  // namespace al
