#include "Event/EventFlowNodeBgmCtrl.h"

#include "Library/Bgm/BgmLineFunction.h"
#include "Library/Event/EventFlowFunction.h"
#include "Library/LiveActor/LiveActor.h"

EventFlowNodeBgmCtrl::EventFlowNodeBgmCtrl(const char* name) : al::EventFlowNode(name) {}

void EventFlowNodeBgmCtrl::init(const al::EventFlowNodeInitInfo& info) {
    al::initEventFlowNode(this, info);
    mBgmPlayName = al::tryGetParamIterKeyString(info, "BgmPlayName");
    mBgmSituationName = al::tryGetParamIterKeyString(info, "BgmSituationName");
    al::tryGetParamIterKeyBool(&mIsBgmPlayStop, info, "IsBgmPlayStop");
    al::tryGetParamIterKeyBool(&mIsBgmSituationEnd, info, "IsBgmSituationEnd");
    al::tryGetParamIterKeyInt(&mStartDelayFrameNum, info, "StartDelayFrameNum");
    mDelayFrameNum = mStartDelayFrameNum;
}

void EventFlowNodeBgmCtrl::start() {
    al::EventFlowNode::start();

    if (mDelayFrameNum > 0) {
        mDelayFrameNum--;
        return;
    }

    al::LiveActor* actor = getActor();
    const char* bgmPlayName = mBgmPlayName;
    const char* bgmSituationName = mBgmSituationName;

    if (bgmPlayName)
        al::startBgm(actor, bgmPlayName, -1, 0);

    if (bgmSituationName)
        al::startBgmSituation(actor, bgmSituationName, false);

    end();
}

void EventFlowNodeBgmCtrl::control() {
    if (mDelayFrameNum > 0) {
        mDelayFrameNum--;
        return;
    }

    al::LiveActor* actor = getActor();
    const char* bgmPlayName = mBgmPlayName;
    const char* bgmSituationName = mBgmSituationName;

    if (bgmPlayName)
        al::startBgm(actor, bgmPlayName, -1, 0);

    if (bgmSituationName)
        al::startBgmSituation(actor, bgmSituationName, false);

    end();
}
