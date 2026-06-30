#include "Event/EventFlowNodeWipeFadeBlackClose.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/NpcEventFlowUtil.h"

EventFlowNodeWipeFadeBlackClose::EventFlowNodeWipeFadeBlackClose(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeWipeFadeBlackClose::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::tryGetParamIterKeyInt(&mFadeStep, info, "FadeStep");
}

void EventFlowNodeWipeFadeBlackClose::control() {
    if (rs::isCloseEventWipeFadeBlack(this))
        end();
    else
        rs::requestCloseEventWipeFadeBlack(this, mFadeStep);
}
