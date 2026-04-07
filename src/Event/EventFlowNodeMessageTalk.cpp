#include "Event/EventFlowNodeMessageTalk.h"

#include "Library/Base/StringUtil.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/Event/EventFlowUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"

#include "Util/NpcEventFlowUtil.h"

EventFlowNodeMessageTalk::EventFlowNodeMessageTalk(const char* name) : al::EventFlowNode(name) {
    mMessage = nullptr;
    mDemoActionName = nullptr;
    mStartActionName = nullptr;
    mStartActionFrameRate = 1.0;
    mLoopActionName = nullptr;
    mLoopActionFrameRate = 1.0;
    isNeedOpenMessage = false;
    isSystemMessage = false;
    isInvalidIconA = false;
    mIconAppearDelayStep = -1;
}

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
        return al::setActionFrameRate(getActor(), mStartActionFrameRate);
    }

    if (!mLoopActionName)
        return;

    al::startEventAction(getActor(), this, mLoopActionName);
    return al::setActionFrameRate(getActor(), mLoopActionFrameRate);
}

void EventFlowNodeMessageTalk::control() {
    if (mMessageFlags & 0xFF) {
        if (mMessageFlags & 0xFF00)
            rs::startOpenNpcDemoEventSystemMessageWithButtonA(this, mMessage, mIconAppearDelayStep);
        else if (mMessageFlags & 0xFF0000)
            rs::startOpenNpcDemoEventTalkMessage(this, mMessage);
        else
            rs::startOpenNpcDemoEventTalkMessageWithButtonA(this, mMessage);
    }

    if (mStartActionName) {
        if (mLoopActionName) {
            if (al::isPlayingEventAction(getActor(), this, mStartActionName) &&
                al::isActionEnd(getActor())) {
                al::startEventAction(getActor(), this, mLoopActionName);
                al::setActionFrameRate(getActor(), mLoopActionFrameRate);
            }
        }
    }

    if (!isNeedOpenMessage) {
        if (rs::isEndNpcDemoEventTalkMessage(this))
            return end();
    }

    isNeedOpenMessage = false;
}

void EventFlowNodeMessageTalk::end() {
    if (mDemoActionName)
        al::resetDemoTalkAction(this);

    if (mStartActionName) {
        if (mLoopActionName && !al::isPlayingEventAction(getActor(), this, mLoopActionName)) {
            al::startEventAction(getActor(), this, mLoopActionName);
            al::setActionFrameRate(getActor(), mLoopActionFrameRate);
        }
    }

    EventFlowNode::end();
}
