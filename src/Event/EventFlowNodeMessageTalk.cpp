#include "Event/EventFlowNodeMessageTalk.h"

#include "Library/Base/StringUtil.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"

#include "Util/NpcEventFlowUtil.h"

EventFlowNodeMessageTalk::EventFlowNodeMessageTalk(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeMessageTalk::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::initEventNodeMessage(&mMessage, this, info, rs::getEventTalkWindowMessageStyle(this));

    isSystemMessage = al::isEqualSubString(getName(), "MessageTalkSystem");
    isInvalidIconA = al::isEqualString(getName(), "MessageTalkInvalidIconA");

    if (al::isEqualSubString(getName(), "MessageTalkDemo"))
        mDemoActionName = al::getParamIterKeyString(info, "DemoActionName");

    mStartActionName = al::tryGetParamIterKeyString(info, "StartActionName");
    al::tryGetParamIterKeyFloat(&mStartActionFrameRate, info, "StartActionFrameRate");
    mLoopActionName = al::tryGetParamIterKeyString(info, "LoopActionName");
    al::tryGetParamIterKeyFloat(&mLoopActionFrameRate, info, "LoopActionFrameRate");

    if (mStartActionName)
        al::isExistEventAction(getActor(), this, mStartActionName);

    al::tryGetParamIterKeyInt(&mIconAppearDelayStep, info, "IconAppearDelayStep");
}

void EventFlowNodeMessageTalk::start() {
    EventFlowNode::start();
    isNeedOpenMessage = true;

    if (mDemoActionName)
        al::setDemoTalkAction(this, mDemoActionName);

    if (mStartActionName) {
        al::startEventAction(getActor(), this, mStartActionName);
        al::setActionFrameRate(getActor(), mStartActionFrameRate);
        return;
    }

    if (mLoopActionName) {
        al::startEventAction(getActor(), this, mLoopActionName);
        al::setActionFrameRate(getActor(), mLoopActionFrameRate);
    }
}

void EventFlowNodeMessageTalk::control() {
    if (isNeedOpenMessage) {
        if (isSystemMessage)
            rs::startOpenNpcDemoEventSystemMessageWithButtonA(this, mMessage, mIconAppearDelayStep);
        else if (isInvalidIconA)
            rs::startOpenNpcDemoEventTalkMessage(this, mMessage);
        else
            rs::startOpenNpcDemoEventTalkMessageWithButtonA(this, mMessage);
    }

    if (mStartActionName && mLoopActionName &&
        al::isPlayingEventAction(getActor(), this, mStartActionName) &&
        al::isActionEnd(getActor())) {
        al::startEventAction(getActor(), this, mLoopActionName);
        al::setActionFrameRate(getActor(), mLoopActionFrameRate);
    }

    if (!isNeedOpenMessage && rs::isEndNpcDemoEventTalkMessage(this)) {
        end();
        return;
    }

    isNeedOpenMessage = false;
}

void EventFlowNodeMessageTalk::end() {
    if (mDemoActionName)
        al::resetDemoTalkAction(this);

    if (mStartActionName && mLoopActionName &&
        !al::isPlayingEventAction(getActor(), this, mLoopActionName)) {
        al::startEventAction(getActor(), this, mLoopActionName);
        al::setActionFrameRate(getActor(), mLoopActionFrameRate);
    }

    EventFlowNode::end();
}
