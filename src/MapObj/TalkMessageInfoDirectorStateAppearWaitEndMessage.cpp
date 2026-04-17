#include "MapObj/TalkMessageInfoDirectorStateAppearWaitEndMessage.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

namespace {
NERVE_IMPL(TalkMessageInfoDirectorStateAppearWaitEndMessage, Appear);
NERVE_IMPL(TalkMessageInfoDirectorStateAppearWaitEndMessage, Wait);
NERVE_IMPL(TalkMessageInfoDirectorStateAppearWaitEndMessage, End);

NERVES_MAKE_NOSTRUCT(TalkMessageInfoDirectorStateAppearWaitEndMessage, Appear, Wait, End);
}  // namespace

TalkMessageInfoDirectorStateAppearWaitEndMessage::TalkMessageInfoDirectorStateAppearWaitEndMessage(
    TalkMessageInfoDirector* director, al::SimpleLayoutAppearWaitEnd* layout)
    : al::HostStateBase<TalkMessageInfoDirector>("メッセージ表示", director), mLayout(layout) {
    initNerve(&Appear, 0);
}

void TalkMessageInfoDirectorStateAppearWaitEndMessage::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Appear);
}

void TalkMessageInfoDirectorStateAppearWaitEndMessage::kill() {
    al::NerveStateBase::kill();
    mLayout->kill();
}

void TalkMessageInfoDirectorStateAppearWaitEndMessage::exeAppear() {
    if (al::isFirstStep(this)) {
        mLayout->appear();
        al::startAction(mLayout, "Appear");
    }

    if (al::isActionEnd(mLayout))
        al::setNerve(this, &Wait);
}

void TalkMessageInfoDirectorStateAppearWaitEndMessage::exeWait() {}

void TalkMessageInfoDirectorStateAppearWaitEndMessage::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(mLayout, "End");

    if (al::isActionEnd(mLayout))
        kill();
}

void TalkMessageInfoDirectorStateAppearWaitEndMessage::end() {
    al::setNerve(this, &End);
}

bool TalkMessageInfoDirectorStateAppearWaitEndMessage::isActiveCapMessage() const {
    return !isDead() && mLayout->isAlive();
}
