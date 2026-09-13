#include "Demo/SimpleDemoState.h"

#include "Library/Demo/DemoActorFunction.h"
#include "Library/Demo/DemoActorHolder.h"
#include "Library/Demo/DemoFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Util/DemoUtil.h"
#include "Util/PlayerDemoUtil.h"

namespace {
NERVE_IMPL(SimpleDemoState, Demo);

NERVES_MAKE_NOSTRUCT(SimpleDemoState, Demo);
}  // namespace

SimpleDemoState::SimpleDemoState(al::LiveActor* actor, const al::ActorInitInfo& info,
                                 const char* demoName, const sead::Matrix34f* baseMtx)
    : al::ActorStateBase("シンプルデモ状態", actor) {
    if (baseMtx)
        mDemoActorHolder =
            al::alDemoFunction::createDemoActorHolder(demoName, info, baseMtx, 1, false);
    else
        mDemoActorHolder =
            al::alDemoFunction::createDemoActorHolder(demoName, info, nullptr, 1, false);
    initNerve(&Demo, 0);
}

void SimpleDemoState::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Demo);
}

void SimpleDemoState::exeDemo() {
    if (al::isFirstStep(this)) {
        if (!requestStartDemo() || !(mIsDemoStarted = startDemoProc())) {
            if (mIsDemoStarted) {
                al::setNerve(this, &Demo);
                return;
            }
            kill();
            return;
        }
    }

    mDemoActorHolder->updateSequence();
    if (mDemoActorHolder->isEndSequence()) {
        endDemoProcBeforeRequest();
        requestEndDemo();
        endDemoProc();
    }
}

bool SimpleDemoState::tryStartDemo() {
    bool isStart = false;
    if (requestStartDemo())
        isStart = mIsDemoStarted = startDemoProc();
    return isStart;
}

bool SimpleDemoState::tryEndDemo() {
    endDemoProcBeforeRequest();
    requestEndDemo();
    endDemoProc();
    return true;
}

bool SimpleDemoState::requestStartDemo() {
    return rs::requestStartDemoNormal(mActor, false);
}

void SimpleDemoState::requestEndDemo() {
    rs::requestEndDemoNormal(mActor);
}

bool SimpleDemoState::startDemoProc() {
    al::addDemoActorFromDemoActorHolder(mActor, mDemoActorHolder);
    if (!al::isInvalidClipping(mActor)) {
        mIsInvalidateClipping = true;
        al::invalidateClipping(mActor);
    }
    mDemoActorHolder->startSequence();
    return true;
}

void SimpleDemoState::endDemoProc() {
    mDemoActorHolder->kill();
    if (mIsInvalidateClipping)
        al::validateClipping(mActor);
    al::NerveStateBase::kill();
}

void SimpleDemoState::endDemoProcBeforeRequest() {}

void SimpleDemoStateSkip::updateOnlyDemoGraphics() {
    al::DemoActorHolder* holder = mDemoActorHolder;
    if (holder && holder->isActiveSequence())
        holder->updateGraphics();
}

bool SimpleDemoStateSkip::requestStartDemo() {
    if (mIsUsePlayer) {
        if (!rs::requestStartDemoWithPlayerCinemaFrame(mActor, true))
            return false;
    } else {
        if (!rs::requestStartDemoNormalWithCinemaFrame(mActor))
            return false;
    }

    rs::requestValidateDemoSkip(mDemoSkipRequester, mActor);
    if (mIsHideDemoPlayer)
        rs::hideDemoPlayerAndStartDemoResetAction(mActor);
    return true;
}

void SimpleDemoStateSkip::requestEndDemo() {
    if (!mIsUsePlayer)
        return rs::requestEndDemoNormalWithCinemaFrame(mActor);

    rs::startActionDemoPlayer(mActor, "Wait");
    rs::showDemoPlayer(mActor);
    rs::requestEndDemoWithPlayerCinemaFrame(mActor);
}
