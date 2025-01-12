#include "Layout/MiniGameLayout.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Layout/KidsModeLayoutAccessor.h"

namespace {
NERVE_IMPL(MiniGameLayout, Appear);
NERVE_IMPL(MiniGameLayout, End);
NERVE_IMPL(MiniGameLayout, Wait);

NERVES_MAKE_NOSTRUCT(MiniGameLayout, Wait, End, Appear);
}  // namespace

MiniGameLayout::MiniGameLayout(const char* name, const al::LayoutInitInfo& info)
    : al::LayoutActor(name) {
    al::initLayoutActor(this, info, "MiniGame", nullptr);
    initNerve(&Appear, 0);
    kill();
}

void MiniGameLayout::appear() {
    al::LayoutActor::appear();
    rs::setKidsModeLayoutDisable(this);
}

void MiniGameLayout::kill() {
    al::LayoutActor::kill();
    rs::setKidsModeLayoutEnable(this);
}

void MiniGameLayout::startJumprope() {
    al::setPaneString(this, "TxtTitle",
                      al::getSystemMessageString(this, "MiniGame", "TitleJumprope"), 0);
    al::startAction(this, "Jumprope", "State");
    al::startAction(this, "NewRecordWait", "NewRecord");
    al::setNerve(this, &Appear);
    appear();
}

void MiniGameLayout::startRace() {
    al::setPaneString(this, "TxtTitle",
                      al::getSystemMessageString(this, "MiniGame", "TitleRaceManRace"), 0);
    al::startAction(this, "Runrace", "State");
    al::startAction(this, "NewRecordWait", "NewRecord");
    al::setNerve(this, &Appear);
    appear();
}

void MiniGameLayout::startVolleyball() {
    al::setPaneString(this, "TxtTitle",
                      al::getSystemMessageString(this, "MiniGame", "TitleVolleyball"), 0);
    al::startAction(this, "Volleyball", "State");
    al::startAction(this, "NewRecordWait", "NewRecord");
    al::setNerve(this, &Appear);
    appear();
}

void MiniGameLayout::end() {
    al::setNerve(this, &End);
}

void MiniGameLayout::setBestCount(s32 count) {
    al::replaceMessageTagScore(
        &mScoreText, this, al::getSystemMessageString(this, "MiniGame", "Counter"), count, "Score");
    al::setPaneString(this, "TxtBestRecord", mScoreText.cstr(), 0);
}

void MiniGameLayout::setTodayCount(s32 count) {
    al::replaceMessageTagScore(
        &mScoreText, this, al::getSystemMessageString(this, "MiniGame", "Counter"), count, "Score");
    al::setPaneString(this, "TxtBestRecordToday", mScoreText.cstr(), 0);
}

void MiniGameLayout::setCount(s32 count) {
    al::replaceMessageTagScore(
        &mScoreText, this, al::getSystemMessageString(this, "MiniGame", "Counter"), count, "Score");
    al::setPaneString(this, "TxtRecord", mScoreText.cstr(), 0);
}

void MiniGameLayout::startNewRecord() {
    al::startAction(this, "NewRecord", "NewRecord");
}

void MiniGameLayout::startNewRecordWait() {
    al::startAction(this, "NewRecordWait", "NewRecord");
}

void MiniGameLayout::startNewRecordToday() {
    al::startAction(this, "NewRecordToday", "NewRecord");
}

bool MiniGameLayout::isEnd() const {
    return al::isNerve(this, &End);
}

void MiniGameLayout::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "Appear", nullptr);
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
}

void MiniGameLayout::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", nullptr);
}

void MiniGameLayout::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);
    if (al::isActionEnd(this, nullptr))
        kill();
}
