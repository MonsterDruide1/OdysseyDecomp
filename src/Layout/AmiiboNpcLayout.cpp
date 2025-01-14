#include "Layout/AmiiboNpcLayout.h"

#include "Library/Layout/LayoutActionFunction.h"
#include "Library/Layout/LayoutActor.h"
#include "Library/Layout/LayoutInitInfo.h"
#include "Library/Message/MessageHolder.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Layout/FooterParts.h"

namespace {
NERVE_IMPL(AmiiboNpcLayout, Appear);
NERVE_IMPL(AmiiboNpcLayout, Decide);
NERVE_IMPL(AmiiboNpcLayout, End);
NERVE_IMPL(AmiiboNpcLayout, Wait);

NERVES_MAKE_NOSTRUCT(AmiiboNpcLayout, Appear, Decide, End, Wait);
}  // namespace

AmiiboNpcLayout::AmiiboNpcLayout(const al::LayoutInitInfo& info)
    : al::LayoutActor("AmiiboNpc用レイアウト") {
    al::initLayoutActor(this, info, "ControllerGuideAmiibo", nullptr);
    initNerve(&Appear, 0);
    al::startAction(this, "Appear", nullptr);
    mFooterParts = new FooterParts(this, info, al::getSystemMessageString(this, "Footer", "Return"),
                                   "TxtGuide", "ParFooter");
    mAmiiboIcon = new al::LayoutActor("AmiiboNpc用レイアウト[アイコン]");
    al::initLayoutPartsActor(mAmiiboIcon, this, info, "ParAmiiboIcon", nullptr);
    kill();
}

void AmiiboNpcLayout::startTouch() {
    mAmiiboIcon->appear();
    al::startAction(mAmiiboIcon, "Appear", nullptr);
}

void AmiiboNpcLayout::endTouch() {
    al::startAction(mAmiiboIcon, "End", nullptr);
}

void AmiiboNpcLayout::appear() {
    al::LayoutActor::appear();
    al::setNerve(this, &Appear);
    al::startAction(mAmiiboIcon, "Hide", nullptr);
}

void AmiiboNpcLayout::control() {
    if (isIconEndActionEnd())
        al::startAction(mAmiiboIcon, "Wait", nullptr);
}

void AmiiboNpcLayout::decide() {
    al::setNerve(this, &Decide);
}

void AmiiboNpcLayout::end() {
    al::startAction(mAmiiboIcon, "Hide", nullptr);
    al::setNerve(this, &End);
}

bool AmiiboNpcLayout::isIconEndActionEnd() const {
    return al::isActionPlaying(mAmiiboIcon, "End", nullptr) &&
           al::isActionEnd(mAmiiboIcon, nullptr);
}

void AmiiboNpcLayout::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear", nullptr);
        al::startAction(this, "Loop", "Loop");
    }
    if (al::isActionEnd(this, nullptr))
        al::setNerve(this, &Wait);
}

void AmiiboNpcLayout::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait", nullptr);
}

void AmiiboNpcLayout::exeDecide() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Decide", nullptr);
        al::startHitReaction(this, "タッチ", nullptr);
    }
    if (al::isActionEnd(this, nullptr))
        kill();
}

void AmiiboNpcLayout::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End", nullptr);
    if (al::isActionEnd(this, nullptr))
        kill();
}
