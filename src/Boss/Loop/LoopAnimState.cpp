#include "Boss/Loop/LoopAnimState.h"

#include "Library/Base/StringUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/LiveActorFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(LoopAnimState, Start)
NERVE_IMPL(LoopAnimState, End)
NERVE_IMPL(LoopAnimState, Loop)

NERVES_MAKE_NOSTRUCT(LoopAnimState, Start, End, Loop)
}  // namespace

LoopAnimState::LoopAnimState(al::LiveActor* actor, const char* name, bool isSyncSubActor)
    : al::ActorStateBase("ループアニメ状態", actor), mName(name), mIsSyncSubActor(isSyncSubActor) {
    initNerve(&Start, 0);
}

void LoopAnimState::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Start);
}

void LoopAnimState::end() {
    if (al::isNerve(this, &End))
        return;
    al::setNerve(this, &End);
}

void LoopAnimState::exeStart() {
    if (al::isFirstStep(this)) {
        al::StringTmp<64> animName("%s%s", mName, mStartSuffix);
        startAction(animName.cstr());
    }

    if (al::isActionEnd(mActor))
        al::setNerve(this, &Loop);
}

void LoopAnimState::startAction(const char* actionName) {
    al::startAction(mActor, actionName);
    if (mIsSyncSubActor) {
        s32 numSubActors = al::getSubActorNum(mActor);
        for (s32 i = 0; i < numSubActors; i++)
            al::tryStartAction(al::getSubActor(mActor, i), actionName);
    }
}

void LoopAnimState::exeLoop() {
    if (al::isFirstStep(this)) {
        al::StringTmp<64> animName("%s%s", mName, mLoopSuffix);
        startAction(animName.cstr());
    }
}

void LoopAnimState::exeEnd() {
    if (al::isFirstStep(this)) {
        al::StringTmp<64> animName("%s%s", mName, mEndSuffix);
        startAction(animName.cstr());
    }

    if (al::isActionEnd(mActor))
        kill();
}
