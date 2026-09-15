#include "MapObj/TalkMessageInfoDirectorStateAppearMessage.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Play/Layout/SimpleLayoutAppearWaitEnd.h"

#include "Layout/ProjectReplaceTagProcessor.h"
#include "System/GameDataUtil.h"

namespace {
NERVE_IMPL(TalkMessageInfoDirectorStateAppearMessage, Appear)
NERVE_IMPL(TalkMessageInfoDirectorStateAppearMessage, AppearWait)
NERVE_IMPL(TalkMessageInfoDirectorStateAppearMessage, FollowText)
NERVE_IMPL(TalkMessageInfoDirectorStateAppearMessage, Wait)
NERVE_IMPL(TalkMessageInfoDirectorStateAppearMessage, End)

NERVES_MAKE_NOSTRUCT(TalkMessageInfoDirectorStateAppearMessage, Appear, AppearWait, FollowText,
                     Wait, End)
}  // namespace

TalkMessageInfoDirectorStateAppearMessage::TalkMessageInfoDirectorStateAppearMessage(
    TalkMessageInfoDirector* director, al::SimpleLayoutAppearWaitEnd* layout)
    : al::HostStateBase<TalkMessageInfoDirector>("メッセージ表示", director), mLayout(layout) {}

void TalkMessageInfoDirectorStateAppearMessage::init() {
    initNerve(&Appear, 0);
}

void TalkMessageInfoDirectorStateAppearMessage::appear() {
    s32 delay = mDelay;
    al::NerveStateBase::appear();

    if (delay >= 1)
        al::setNerve(this, &AppearWait);
    else
        al::setNerve(this, &Appear);
}

void TalkMessageInfoDirectorStateAppearMessage::kill() {
    mLayout->kill();
    al::NerveStateBase::kill();
}

void TalkMessageInfoDirectorStateAppearMessage::exeAppearWait() {
    if (al::isGreaterEqualStep(this, mDelay))
        al::setNerve(this, &Appear);
}

void TalkMessageInfoDirectorStateAppearMessage::exeAppear() {
    if (al::isFirstStep(this)) {
        mLayout->appear();
        al::startAction(mLayout, "Appear");
    }

    if (al::isActionEnd(mLayout))
        al::setNerve(this, &FollowText);
}

void TalkMessageInfoDirectorStateAppearMessage::exeFollowText() {
    if (al::isFirstStep(this) && !al::isActionPlaying(mLayout, "Wait"))
        al::startAction(mLayout, "Wait");

    if (al::isEndTextPaneAnim(mLayout, false))
        al::setNerve(this, &Wait);
}

void TalkMessageInfoDirectorStateAppearMessage::exeWait() {
    if (al::isGreaterEqualStep(this, mWaitFrame)) {
        ProjectReplaceTagProcessor replaceTagProcessor(mLayout);

        if (al::tryChangeNextPage(mLayout, nullptr, &replaceTagProcessor))
            al::setNerve(this, &FollowText);
        else
            al::setNerve(this, &End);
    }
}

void TalkMessageInfoDirectorStateAppearMessage::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(mLayout, "End");

    if (al::isActionEnd(mLayout))
        kill();
}

void TalkMessageInfoDirectorStateAppearMessage::setWaitFrame(s32 waitFrame) {
    const al::LayoutActor* layout = mLayout;
    mWaitFrame = waitFrame;

    if (rs::isKidsMode(layout))
        mWaitFrame *= 2;
}

void TalkMessageInfoDirectorStateAppearMessage::setDelay(s32 delay) {
    mDelay = delay;
}

void TalkMessageInfoDirectorStateAppearMessage::setLabelName(const char* labelName) {
    mLabelName = al::StringTmp<128>(labelName);
}

const char* TalkMessageInfoDirectorStateAppearMessage::getLabelName() const {
    return mLabelName.cstr();
}

bool TalkMessageInfoDirectorStateAppearMessage::isActiveCapMessage(const char* labelName) const {
    if (!isDead()) {
        bool isActive;

        if (al::isNerve(this, &Appear))
            isActive = true;
        else if (al::isNerve(this, &FollowText))
            isActive = true;
        else if (al::isNerve(this, &Wait))
            isActive = true;
        else
            isActive = al::isNerve(this, &End);

        if (labelName) {
            if ((isActive & al::isEqualString(labelName, mLabelName.cstr())) != 0)
                return true;
        } else if (isActive) {
            return true;
        }
    }

    return false;
}

bool TalkMessageInfoDirectorStateAppearMessage::isDelayCapMessage(const char* labelName) const {
    if (isDead())
        return false;

    if (labelName) {
        if (al::isEqualString(labelName, mLabelName.cstr()) && al::isNerve(this, &AppearWait))
            return true;
    } else if (al::isNerve(this, &AppearWait)) {
        return true;
    }

    return false;
}
