#include "Npc/VolleyballLayout.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActorUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Layout/MiniGameCueLayout.h"
#include "Layout/MiniGameLayout.h"
#include "Npc/VolleyballNpc.h"
#include "Util/InputInterruptTutorialUtil.h"

namespace {
NERVE_IMPL(VolleyballLayout, Wait);
NERVE_IMPL(VolleyballLayout, Appear);
NERVE_IMPL(VolleyballLayout, Game);
NERVE_IMPL(VolleyballLayout, Result);
NERVE_IMPL(VolleyballLayout, End);
NERVE_IMPL(VolleyballLayout, Message);

NERVES_MAKE_STRUCT(VolleyballLayout, Wait, Appear, End, Message, Game, Result);
}  // namespace

VolleyballLayout::VolleyballLayout(const VolleyballNpc* npc, const al::ActorInitInfo& info)
    : al::NerveExecutor("縄跳びレイアウト管理"), mNpc(npc) {
    mLayout = new MiniGameLayout("ミニゲームレイアウト", al::getLayoutInitInfo(info));
    mCueLayout = new MiniGameCueLayout("ミニゲーム見出しレイアウト", al::getLayoutInitInfo(info));
    initNerve(&NrvVolleyballLayout.Wait, 0);
}

void VolleyballLayout::exeWait() {
    if (mNpc->isEnableAppearLayout())
        al::setNerve(this, &NrvVolleyballLayout.Appear);
}

void VolleyballLayout::exeAppear() {}

void VolleyballLayout::exeGame() {
    if (al::isFirstStep(this)) {
        if (al::isDead(mLayout) || mLayout->isEnd())
            mLayout->startVolleyball();
        mLayout->startNewRecordWait();
        mLayout->setBestCount(mNpc->getBestScore());
        mLayout->setCount(0);
    }
    mLayout->setCount(mNpc->getCurrentScore());
}

void VolleyballLayout::exeResult() {
    if (al::isFirstStep(this)) {
        mCueLayout->appearMiss();
        al::setActionFrameRate(mCueLayout, 1.0f, nullptr);
        _28 = false;
    }

    if (!mNpc->isEnableAppearLayout()) {
        al::setActionFrameRate(mCueLayout, 3.0f, nullptr);
        _28 = true;
    }

    if (al::isStep(this, 180) || !mNpc->isEnableAppearLayout()) {
        mLayout->end();
        al::setNerve(this, &NrvVolleyballLayout.End);
    }
}

void VolleyballLayout::exeEnd() {
    if (!mNpc->isEnableAppearLayout()) {
        al::setActionFrameRate(mCueLayout, 3.0f, nullptr);
        _28 = true;
    }

    if (!al::isDead(mLayout) || !al::isDead(mCueLayout))
        return;

    if (!mNpc->isEnableAppearLayout() || _28) {
        al::setNerve(this, &NrvVolleyballLayout.Wait);
        return;
    }

    al::setNerve(this, &NrvVolleyballLayout.Message);
}

void VolleyballLayout::exeMessage() {
    if (al::isFirstStep(this))
        rs::tryAppearObjectTutorial(mNpc, "JumpingRopeNpc");

    if (mNpc->isEnableAppearLayout())
        return;

    rs::tryCloseObjectTutorial(mNpc, "JumpingRopeNpc");
    al::setNerve(this, &NrvVolleyballLayout.Wait);
}

bool VolleyballLayout::isWaiting() const {
    return al::isNerve(this, &NrvVolleyballLayout.Wait);
}

bool VolleyballLayout::isInGame() const {
    return al::isNerve(this, &NrvVolleyballLayout.Game);
}

void VolleyballLayout::start() {
    if (isInGame())
        return;

    rs::tryCloseObjectTutorial(mNpc, "JumpingRopeNpc");
    al::setNerve(this, &NrvVolleyballLayout.Game);
}

void VolleyballLayout::result() {
    if (isInGame())
        al::setNerve(this, &NrvVolleyballLayout.Result);
}

void VolleyballLayout::end() {
    if (!al::isNerve(this, &NrvVolleyballLayout.Wait) &&
        !al::isNerve(this, &NrvVolleyballLayout.Appear) &&
        !al::isNerve(this, &NrvVolleyballLayout.Result) &&
        !al::isNerve(this, &NrvVolleyballLayout.End) &&
        !al::isNerve(this, &NrvVolleyballLayout.Message)) {
        al::setNerve(this, &NrvVolleyballLayout.Result);
    }
}

void VolleyballLayout::setBest(u32 count) {
    mLayout->setBestCount(count);
    mLayout->startNewRecord();
}

void VolleyballLayout::setBestToday(u32 count) {}
