#include "Event/EventFlowNodeSelectYesNo.h"

#include "Library/Event/EventFlowFunction.h"

#include "Event/EventFlowChoiceInfo.h"
#include "Util/NpcEventFlowUtil.h"

EventFlowNodeSelectYesNo::EventFlowNodeSelectYesNo(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeSelectYesNo::init(const al::EventFlowNodeInitInfo& info) {
    const char16* message;

    al::initEventFlowNode(this, info);
    al::initEventQuery(this, info);
    message = nullptr;
    al::initEventNodeMessage(&message, this, info, rs::getEventTalkWindowMessageStyle(this));

    mChoiceInfo = new al::EventFlowChoiceInfo(2);
    mChoiceInfo->mNodeMessage = message;
    mChoiceInfo->registerChoiceMessage(al::getCaseEventMessage(this, 0));
    mChoiceInfo->registerChoiceMessage(al::getCaseEventMessage(this, 1));
    mChoiceInfo->mCancelIndex = 1;
}

void EventFlowNodeSelectYesNo::start() {
    EventFlowNode::start();
    rs::startChoiceEvent(this, mChoiceInfo);
}

void EventFlowNodeSelectYesNo::control() {
    if (mChoiceInfo->mSelectedIndex < 0)
        return;

    end();
}

s32 EventFlowNodeSelectYesNo::getNextId() const {
    s32 selectedIndex = mChoiceInfo->mSelectedIndex;

    if (selectedIndex < 0)
        return -1;

    return al::getCaseEventNextId(this, selectedIndex);
}
