#include "Event/EventActorStateDemoTalkGK.h"

#include <math/seadMathCalcCommon.h>

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/NpcEventFlowUtil.h"

namespace {
NERVE_IMPL(EventActorStateDemoTalkGK, Wait)
NERVE_IMPL(EventActorStateDemoTalkGK, Talk)
NERVE_IMPL(EventActorStateDemoTalkGK, TalkAfter)

NERVES_MAKE_NOSTRUCT(EventActorStateDemoTalkGK, Wait, Talk, TalkAfter)

constexpr ParamEventActorStateDemoTalkGK cDefaultParam;
}  // namespace

EventActorStateDemoTalkGK::EventActorStateDemoTalkGK(al::LiveActor* actor,
                                                     const ParamEventActorStateDemoTalkGK* param)
    : al::ActorStateBase("", actor), mParam(param == nullptr ? &cDefaultParam : param) {
    initNerve(&Wait, 0);
}

void EventActorStateDemoTalkGK::appear() {
    al::NerveStateBase::appear();
    al::startAction(mActor, mActionName);
    if (rs::isPlayingTextPaneAnimEventTalkMessage(mActor)) {
        al::setSklAnimBlendWeightDouble(mActor, 0.0f, 1.0f);
        mBlendWeight = 1.0f;
        al::setNerve(this, &Talk);
    } else {
        mBlendWeight = 0.0f;
        al::setSklAnimBlendWeightDouble(mActor, 1.0f, 0.0f);
        al::setNerve(this, &Wait);
    }
}

void EventActorStateDemoTalkGK::exeWait() {
    al::isFirstStep(this);
    chaseWaitAnimWeightOne();
    if (rs::isPlayingTextPaneAnimEventTalkMessage(mActor))
        al::setNerve(this, &Talk);
}

void EventActorStateDemoTalkGK::chaseWaitAnimWeightOne() {
    mBlendWeight = sead::Mathf::clampMin(mBlendWeight - mParam->blendStep, 0.0f);
    f32 weight = al::easeByType(mBlendWeight, mParam->easeType);
    al::setSklAnimBlendWeightDouble(mActor, 1.0f - weight, weight);
}

void EventActorStateDemoTalkGK::exeTalk() {
    al::isFirstStep(this);
    chaseTalkAnimWeightOne();
    if (!rs::isPlayingTextPaneAnimEventTalkMessage(mActor))
        al::setNerve(this, &TalkAfter);
}

void EventActorStateDemoTalkGK::chaseTalkAnimWeightOne() {
    mBlendWeight = sead::Mathf::clampMax(mBlendWeight + mParam->blendStep, 1.0f);
    f32 weight = al::easeByType(mBlendWeight, mParam->easeType);
    al::setSklAnimBlendWeightDouble(mActor, 1.0f - weight, weight);
}

void EventActorStateDemoTalkGK::exeTalkAfter() {
    chaseTalkAnimWeightOne();
    if (!al::isGreaterEqualStep(this, 45))
        return;

    if (mIsWaitActionEnd) {
        al::LiveActor* actor = mActor;
        f32 frameMax = al::getActionFrameMax(actor, al::getActionName(actor));
        if (!(frameMax <= al::getActionFrame(mActor) + 1.0f))
            return;

        mBlendWeight = 0.0f;
        al::setSklAnimBlendWeightDouble(mActor, 1.0f);
    } else if (rs::isPlayingTextPaneAnimEventTalkMessage(mActor)) {
        al::setNerve(this, &Talk);
        return;
    }

    al::setNerve(this, &Wait);
}
