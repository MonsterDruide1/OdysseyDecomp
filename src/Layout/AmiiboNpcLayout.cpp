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
    al::initLayoutActor(this, info, "ControllerGuideAmiibo");
    initNerve(&Appear);
    al::startAction(this, "Appear");
    mFooterParts = new FooterParts(this, info, al::getSystemMessageString(this, "Footer", "Return"),
                                   "TxtGuide", "ParFooter");
    mAmiiboIcon = new al::LayoutActor("AmiiboNpc用レイアウト[アイコン]");
    al::initLayoutPartsActor(mAmiiboIcon, this, info, "ParAmiiboIcon");
    kill();
}

void AmiiboNpcLayout::startTouch() {
    mAmiiboIcon->appear();
    al::startAction(mAmiiboIcon, "Appear");
}

void AmiiboNpcLayout::endTouch() {
    al::startAction(mAmiiboIcon, "End");
}

void AmiiboNpcLayout::appear() {
    al::LayoutActor::appear();
    al::setNerve(this, &Appear);
    al::startAction(mAmiiboIcon, "Hide");
}

void AmiiboNpcLayout::control() {
    if (al::isActionPlaying(mAmiiboIcon, "Appear") && al::isActionEnd(mAmiiboIcon))
        al::startAction(mAmiiboIcon, "Wait");
}

void AmiiboNpcLayout::decide() {
    al::setNerve(this, &Decide);
}

void AmiiboNpcLayout::end() {
    al::startAction(mAmiiboIcon, "Hide");
    al::setNerve(this, &End);
}

bool AmiiboNpcLayout::isIconEndActionEnd() const {
    return al::isActionPlaying(mAmiiboIcon, "End") && al::isActionEnd(mAmiiboIcon);
}

void AmiiboNpcLayout::exeAppear() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Appear");
        al::startAction(this, "Loop", "Loop");
    }
    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void AmiiboNpcLayout::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Wait");
}

void AmiiboNpcLayout::exeDecide() {
    if (al::isFirstStep(this)) {
        al::startAction(this, "Decide");
        al::startHitReaction(this, "タッチ");
    }
    if (al::isActionEnd(this))
        kill();
}

void AmiiboNpcLayout::exeEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "End");
    if (al::isActionEnd(this))
        kill();
}
