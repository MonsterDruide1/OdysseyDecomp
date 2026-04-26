#include "Event/EventActorStateDemoTalk.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/NpcEventFlowUtil.h"

namespace {

NERVE_IMPL(EventActorStateDemoTalk, Wait);
NERVE_IMPL(EventActorStateDemoTalk, Talk);
NERVE_IMPL(EventActorStateDemoTalk, TalkAfter);

NERVES_MAKE_NOSTRUCT(EventActorStateDemoTalk, Wait, Talk, TalkAfter);

}  // namespace

EventActorStateDemoTalk::EventActorStateDemoTalk(al::LiveActor* actor)
    : al::ActorStateBase("イベントデモ中の会話", actor) {
    mMessageName = "Talk";
    initNerve(&Wait, 0);
}

void EventActorStateDemoTalk::appear() {
    al::NerveStateBase::appear();
    al::startAction(mActor, mMessageName);

    if (rs::isPlayingTextPaneAnimEventTalkMessage(mActor)) {
        al::setSklAnimBlendWeightDouble(mActor, 0.0f, 1.0f);
        al::setNerve(this, &Talk);
    } else {
        al::setSklAnimBlendWeightDouble(mActor, 1.0f, 0.0f);
        al::setNerve(this, &Wait);
    }
}

void EventActorStateDemoTalk::exeWait() {
    al::LiveActor* actor = mActor;
    f32 blendWeight = al::getSklAnimBlendWeight(actor, 0);
    blendWeight += 0.125f;
    if (blendWeight > 1.0f)
        blendWeight = 1.0f;
    al::setSklAnimBlendWeightDouble(actor, blendWeight, 1.0f - blendWeight);

    if (rs::isPlayingTextPaneAnimEventTalkMessage(mActor))
        al::setNerve(this, &Talk);
}

void EventActorStateDemoTalk::exeTalk() {
    al::LiveActor* actor = mActor;
    f32 blendWeight = al::getSklAnimBlendWeight(actor, 1);
    blendWeight += 0.125f;
    f32 one = 1.0f;
    if (blendWeight > 1.0f)
        blendWeight = 1.0f;
    al::setSklAnimBlendWeightDouble(actor, one - blendWeight, blendWeight);

    if (!rs::isPlayingTextPaneAnimEventTalkMessage(mActor))
        al::setNerve(this, &TalkAfter);
}

void EventActorStateDemoTalk::exeTalkAfter() {
    al::LiveActor* actor = mActor;
    f32 blendWeight = al::getSklAnimBlendWeight(actor, 1);
    blendWeight += 0.125f;
    f32 one = 1.0f;
    if (blendWeight > 1.0f)
        blendWeight = 1.0f;
    al::setSklAnimBlendWeightDouble(actor, one - blendWeight, blendWeight);

    if (al::isGreaterEqualStep(this, 45)) {
        if (mIsInitialized) {
            al::LiveActor* actor2 = mActor;
            const char* actionName = al::getActionName(actor2);
            f32 frameMax = al::getActionFrameMax(actor2, actionName);
            f32 frame = al::getActionFrame(mActor);

            if (frameMax <= frame + one)
                al::setSklAnimBlendWeightDouble(mActor, 1.0f);
            else
                return;
        }
        al::setNerve(this, &Wait);
    }
}

ParamEventActorStateDemoTalkGK::ParamEventActorStateDemoTalkGK() {
    mParam1 = 0.125f;
    mParam2 = 0;
}

ParamEventActorStateDemoTalkGK::ParamEventActorStateDemoTalkGK(f32 param1, s32 param2)
    : mParam1(param1), mParam2(param2) {}
