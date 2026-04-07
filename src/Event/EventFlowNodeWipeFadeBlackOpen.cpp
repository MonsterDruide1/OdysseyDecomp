#include "Event/EventFlowNodeWipeFadeBlackOpen.h"

#include "Library/Event/EventFlowFunction.h"

#include "Util/NpcEventFlowUtil.h"

EventFlowNodeWipeFadeBlackOpen::EventFlowNodeWipeFadeBlackOpen(const char* name)
    : al::EventFlowNode(name) {}

void EventFlowNodeWipeFadeBlackOpen::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    al::tryGetParamIterKeyInt(&mFadeStep, info, "FadeStep");
}

void EventFlowNodeWipeFadeBlackOpen::control() {
    if (rs::isOpenEventWipeFadeBlack(this))
        end();
    else
        return rs::requestOpenEventWipeFadeBlack(this, mFadeStep);
}
