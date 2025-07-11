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
    al::initLayoutActor(this, info, "MiniGame");
    initNerve(&Appear);
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
                      al::getSystemMessageString(this, "MiniGame", "TitleJumprope"));
    al::startAction(this, "Jumprope", "State");
    startNewRecordWait();
    al::setNerve(this, &Appear);
    appear();
}

void MiniGameLayout::startRace() {
    al::setPaneString(this, "TxtTitle",
                      al::getSystemMessageString(this, "MiniGame", "TitleRaceManRace"));
    al::startAction(this, "Runrace", "State");
    startNewRecordWait();
    al::setNerve(this, &Appear);
    appear();
}

void MiniGameLayout::startVolleyball() {
    al::setPaneString(this, "TxtTitle",
                      al::getSystemMessageString(this, "MiniGame", "TitleVolleyball"));
    al::startAction(this, "Volleyball", "State");
    startNewRecordWait();
    al::setNerve(this, &Appear);
    appear();
}

void MiniGameLayout::end() {
    al::setNerve(this, &End);
}

void MiniGameLayout::setBestCount(s32 count) {
    al::replaceMessageTagScore(
        &mScoreText, this, al::getSystemMessageString(this, "MiniGame", "Counter"), count, "Score");
    al::setPaneString(this, "TxtBestRecord", mScoreText.cstr());
}

void MiniGameLayout::setTodayCount(s32 count) {
    al::replaceMessageTagScore(
        &mScoreText, this, al::getSystemMessageString(this, "MiniGame", "Counter"), count, "Score");
    al::setPaneString(this, "TxtBestRecordToday", mScoreText.cstr());
}

void MiniGameLayout::setCount(s32 count) {
    al::replaceMessageTagScore(
        &mScoreText, this, al::getSystemMessageString(this, "MiniGame", "Counter"), count, "Score");
    al::setPaneString(this, "TxtRecord", mScoreText.cstr());
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
        al::startAction(this, "Appear");
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void MiniGameLayout::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void MiniGameLayout::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End");
    if (al::isActionEnd(this))
        kill();
}
