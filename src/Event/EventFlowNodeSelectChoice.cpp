#include "Event/EventFlowNodeSelectChoice.h"

#include "Library/Event/EventFlowFunction.h"

#include "Event/EventFlowChoiceInfo.h"
#include "Util/NpcEventFlowUtil.h"

EventFlowNodeSelectChoice::EventFlowNodeSelectChoice(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeSelectChoice::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
    al::makeParamMessageString(&mMessage, info, "Text");

    mChoiceInfo = new al::EventFlowChoiceInfo(al::getCaseEventNum(this));
    mChoiceInfo->mNodeMessage = mMessage;

    if (al::getCaseEventNum(this) >= 1) {
        s32 index = 0;

        do {
            mChoiceInfo->registerChoiceMessage(al::getCaseEventMessage(this, index));
            index++;
        } while (index < al::getCaseEventNum(this));
    }

    s32 cancelIndex = -1;
    if (al::tryGetParamIterKeyInt(&cancelIndex, info, "CancelIndex") && cancelIndex >= 0)
        mChoiceInfo->mCancelIndex = cancelIndex;

    bool isContinuePreSelectIndex = false;
    if (al::tryGetParamIterKeyBool(&isContinuePreSelectIndex, info, "IsContinuePreSelectIndex") &&
        isContinuePreSelectIndex) {
        mChoiceInfo->mIsContinuePreSelectIndex = true;
    }
}

void EventFlowNodeSelectChoice::start() {
    EventFlowNode::start();
    return rs::startChoiceEvent(this, mChoiceInfo);
}

void EventFlowNodeSelectChoice::control() {
    if (mChoiceInfo->mSelectedIndex < 0)
        return;

    return end();
}

s32 EventFlowNodeSelectChoice::getNextId() const {
    s32 selectedIndex = mChoiceInfo->mSelectedIndex;

    if (selectedIndex < 0)
        return -1;

    return al::getCaseEventNextId(this, selectedIndex);
}
