#include "Event/EventActorStateGiveShine.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/ItemUtil.h"

namespace {
NERVE_IMPL(EventActorStateGiveShine, Start);
NERVE_IMPL(EventActorStateGiveShine, Wait);

NERVES_MAKE_STRUCT(EventActorStateGiveShine, Start, Wait);
}  // namespace

EventActorStateGiveShine::EventActorStateGiveShine(al::LiveActor* actor)
    : al::ActorStateBase("シャイン放出状態", actor) {
    initNerve(&NrvEventActorStateGiveShine.Start, 0);
}

void EventActorStateGiveShine::start(Shine* shine, const sead::Vector3f& trans,
                                     const char* actionName) {
    mShine = shine;
    mShineAppearTrans.set(trans);

    if (actionName)
        al::startAction(mActor, actionName);

    if (actionName)
        al::setNerve(this, &NrvEventActorStateGiveShine.Start);
    else
        al::setNerve(this, &NrvEventActorStateGiveShine.Wait);
}

void EventActorStateGiveShine::exeStart() {
    if (al::isActionEnd(mActor))
        al::setNerve(this, &NrvEventActorStateGiveShine.Wait);
}

void EventActorStateGiveShine::exeWait() {
    if (al::isFirstStep(this))
        rs::appearPopupShineWithoutDemo(mShine, mShineAppearTrans);

    if (rs::isEndAppearShine(mShine))
        kill();
}
