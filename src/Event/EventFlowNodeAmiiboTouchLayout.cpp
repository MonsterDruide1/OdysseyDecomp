#include "Library/Event/EventFlowFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Nfp/NfpFunction.h"
#include "Library/Nfp/NfpTypes.h"

#include "Amiibo/AmiiboNpcDirector.h"
#include "Event/EventFlowNodeAmiiboTouchLayout.h"
#include "Layout/AmiiboNpcLayout.h"
#include "Util/AmiiboUtil.h"
#include "Util/NpcEventFlowUtil.h"
#include "Util/StageInputFunction.h"

namespace {
NERVE_IMPL(EventFlowNodeAmiiboTouchLayout, RequestAppearLayout);
NERVE_IMPL(EventFlowNodeAmiiboTouchLayout, WaitTouchTrigger);
NERVE_IMPL(EventFlowNodeAmiiboTouchLayout, WaitTouchAmiibo);
NERVE_IMPL(EventFlowNodeAmiiboTouchLayout, IconEnd);
NERVE_IMPL(EventFlowNodeAmiiboTouchLayout, WaitEndLayout);

NERVES_MAKE_NOSTRUCT(EventFlowNodeAmiiboTouchLayout, RequestAppearLayout);
NERVES_MAKE_STRUCT(EventFlowNodeAmiiboTouchLayout, WaitTouchTrigger, WaitTouchAmiibo, IconEnd,
                   WaitEndLayout);
}  // namespace

EventFlowNodeAmiiboTouchLayout::EventFlowNodeAmiiboTouchLayout(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeAmiiboTouchLayout::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
    initNerve(&RequestAppearLayout, 0);
}

void EventFlowNodeAmiiboTouchLayout::start() {
    al::EventFlowNode::start();
    al::setNerve(this, &RequestAppearLayout);
    if (rs::isOpenWaitNpcDemoEventTalkMessage(getActor()))
        rs::startCloseNpcDemoEventTalkMessage(getActor());
}

s32 EventFlowNodeAmiiboTouchLayout::getNextId() const {
    return al::getCaseEventNextId(this, mCaseEventIdx);
}

void EventFlowNodeAmiiboTouchLayout::exeRequestAppearLayout() {
    if (AmiiboFunction::requestAppearAmiiboLayout(getActor()))
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitTouchTrigger);
}

void EventFlowNodeAmiiboTouchLayout::exeWaitTouchTrigger() {
    if (rs::isHoldAmiiboMode(getActor())) {
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitTouchAmiibo);
        return;
    }

    tryCancel();
}

bool EventFlowNodeAmiiboTouchLayout::tryCancel() {
    al::LiveActor* actor = getActor();

    if (rs::isTriggerUiCancel(actor)) {
        mCaseEventIdx = 4;
        AmiiboFunction::requestEndAmiiboLayout(actor);
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitEndLayout);
        return true;
    }

    return false;
}

void EventFlowNodeAmiiboTouchLayout::exeCountHold() {
    if (!rs::isHoldAmiiboMode(getActor())) {
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitTouchTrigger);
        return;
    }

    if (!tryCancel() && al::isGreaterEqualStep(this, 8))
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitTouchAmiibo);
}

void EventFlowNodeAmiiboTouchLayout::exeWaitTouchAmiibo() {
    al::LiveActor* actor = getActor();

    if (rs::isTriggerUiCancel(actor)) {
        mCaseEventIdx = 4;
        AmiiboFunction::requestEndAmiiboLayout(actor);
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitEndLayout);
        return;
    }

    if (!rs::isHoldAmiiboMode(actor)) {
        AmiiboFunction::getAmiiboTouchLayout(actor)->endTouch();
        AmiiboFunction::stopNfpTouch(actor);
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.IconEnd);
        return;
    }

    if (al::isFirstStep(this))
        AmiiboFunction::startNfpTouch(actor);

    if (al::isLessStep(this, 10) && AmiiboFunction::isNfpErrorHandled(actor)) {
        AmiiboFunction::stopNfpTouch(actor);
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitTouchTrigger);
        return;
    }

    if (al::isStep(this, 10))
        AmiiboFunction::getAmiiboTouchLayout(actor)->startTouch();

    touch(AmiiboFunction::tryGetTriggerTouchNfpInfo(getActor()));
}

void EventFlowNodeAmiiboTouchLayout::touch(const al::NfpInfo* nfpInfo) {
    if (!nfpInfo)
        return;

    al::LiveActor* actor = getActor();

    if (AmiiboFunction::isSearchAmiibo(actor, *nfpInfo))
        mCaseEventIdx = 3;
    else if (rs::isEnableUseStageSceneAmiibo(*nfpInfo))
        mCaseEventIdx = 0;
    else if (rs::isExistAmiiboMstxtData(this, *nfpInfo))
        mCaseEventIdx = 1;
    else
        mCaseEventIdx = 2;

    AmiiboFunction::stopNfpTouch(actor);
    AmiiboFunction::requestDecideAmiiboLayout(actor);

    al::NfpCharacterId characterId = {};
    al::tryGetCharacterId(&characterId, *nfpInfo);

    s32 numberingId = 0;
    al::tryGetNumberingId(&numberingId, *nfpInfo);

    s32 id = rs::createCharacterIdS32(characterId);
    AmiiboFunction::setTouchAmiiboName(actor, id, numberingId);
    AmiiboFunction::trySetAmiiboCostumeName(actor, id);

    al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitEndLayout);
}

void EventFlowNodeAmiiboTouchLayout::exeIconEnd() {
    al::LiveActor* actor = getActor();

    if (rs::isTriggerUiCancel(actor)) {
        mCaseEventIdx = 4;
        AmiiboFunction::requestEndAmiiboLayout(actor);
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitEndLayout);
        return;
    }

    if (AmiiboFunction::getAmiiboTouchLayout(actor)->isIconEndActionEnd())
        al::setNerve(this, &NrvEventFlowNodeAmiiboTouchLayout.WaitTouchTrigger);
}

void EventFlowNodeAmiiboTouchLayout::exeWaitEndLayout() {
    if (AmiiboFunction::isEndAmiiboLayout(getActor()))
        end();
}
