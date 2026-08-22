#include "Demo/DemoStateHackFirst.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "Demo/DemoHackFirstDirector.h"
#include "System/GameDataFunction.h"
#include "System/GameDataHolderAccessor.h"
#include "Util/DemoUtil.h"
#include "Util/Hack.h"
#include "Util/InputInterruptTutorialUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(DemoStateHackFirst, Demo);
NERVE_IMPL(DemoStateHackFirst, End);

NERVES_MAKE_NOSTRUCT(DemoStateHackFirst, Demo, End);
}  // namespace

DemoStateHackFirst::DemoStateHackFirst(al::LiveActor* actor, const al::ActorInitInfo& initInfo)
    : al::ActorStateBase("初回憑依デモ", actor) {
    if (al::isObjectName(initInfo, "Frog")) {
        mDemoType = 1;
    } else {
        if (!al::isObjectNameSubStr(initInfo, "Koopa"))
            return;
        mDemoType = 2;
    }

    if (al::isExistLinkChild(initInfo, "LinkDemoHackDirector", 0))
        mDemoHackFirstDirector = rs::tryCreateDemoHackFirstDirector(mActor, mDemoType, initInfo);

    initNerve(&Demo, 0);
}

void DemoStateHackFirst::appear() {
    al::NerveStateBase::appear();
    al::setNerve(this, &Demo);
    mDemoHackFirstDirector->setHackFirstDemo(mActor, mPlayerHack);
    al::tryOnStageSwitch(mActor, "SwitchActiveDemoHackFirstOnOff");
    rs::requestValidateDemoSkip(this, mActor);
}

void DemoStateHackFirst::kill() {
    al::NerveStateBase::kill();
}

bool DemoStateHackFirst::isFirstDemo() const {
    if (mDemoType == 1)
        return true;

    if (mDemoType == 2) {
        if (GameDataFunction::isExistInHackDictionary(GameDataHolderAccessor(mActor), "Koopa"))
            return false;
    }

    return true;
}

bool DemoStateHackFirst::isEnableSkipDemo() const {
    return true;
}

void DemoStateHackFirst::skipDemo() {
    mDemoHackFirstDirector->skipDemo();
    endDemo();
}

void DemoStateHackFirst::endDemo() {
    al::setNerve(this, &End);

    if (mDemoType == 1)
        rs::appearFirstHackTutorialFrog(mActor);
    else if (mDemoType == 2)
        rs::appearFirstHackTutorialKoopa(mActor);

    al::tryOnStageSwitch(mActor, "SwitchHackFirstEndOn");
    al::tryOffStageSwitch(mActor, "SwitchActiveDemoHackFirstOnOff");
    rs::endHackStartDemo(mPlayerHack, mActor);
    kill();
}

void DemoStateHackFirst::updateOnlyDemoGraphics() {
    if (mDemoHackFirstDirector)
        mDemoHackFirstDirector->updateOnlyDemoGraphics();
}

bool DemoStateHackFirst::tryHackFirstDemoWait(const al::SensorMsg* message) {
    if (!mDemoHackFirstDirector)
        return false;
    if (!isEnableShowHackDemo())
        return false;
    if (!rs::isMsgStartHack(message))
        return false;

    s32 demoStartWaitFrame = mDemoStartWaitFrame;
    if (demoStartWaitFrame == mDemoHackFirstDirector->getDemoStartWaitFrame())
        return false;

    demoStartWaitFrame = mDemoStartWaitFrame;
    if (demoStartWaitFrame == 0) {
        al::startHitReaction(mActor, "[初キャプチャーデモ用]帽子ヒット");
        mDemoHackFirstDirector->appear();
        demoStartWaitFrame = mDemoStartWaitFrame;
    }

    mDemoStartWaitFrame = demoStartWaitFrame + 1;
    return true;
}

bool DemoStateHackFirst::isEnableShowHackDemo() const {
    s32 demoType = mDemoType;
    if (demoType == 1) {
        if (GameDataFunction::getCurrentWorldId(GameDataHolderAccessor(mActor)) != 0)
            return false;
        demoType = mDemoType;
    }

    if (demoType == 2) {
        s32 worldId = GameDataFunction::getCurrentWorldId(GameDataHolderAccessor(mActor));
        if (worldId != GameDataFunction::getWorldIndexMoon())
            return false;
        demoType = mDemoType;
    }

    if (demoType == 1) {
        if (GameDataFunction::isExistInHackDictionary(GameDataHolderAccessor(mActor), "Frog"))
            return false;
    }

    return true;
}

bool DemoStateHackFirst::tryHackFirst(IUsePlayerHack** playerHack, const al::SensorMsg* message,
                                      al::HitSensor* other, al::HitSensor* self) {
    if (!rs::isMsgStartHack(message))
        return false;
    if (!mDemoHackFirstDirector)
        return false;

    if (!isEnableShowHackDemo()) {
        al::tryOnStageSwitch(mActor, "SwitchHackFirstEndOn");
        return false;
    }

    mPlayerHack = rs::startHack(self, other, nullptr);
    rs::startHackStartDemoPuppetable(mPlayerHack, mActor);

    if (mDemoType != 2) {
        if (mDemoType == 1)
            rs::setDemoInfoDemoName(mActor, DemoName::cHackStartFirstTimeFrog);
    } else {
        rs::setDemoInfoDemoName(mActor, DemoName::cHackStartFirstTimeKoopa);
    }

    *playerHack = mPlayerHack;
    return true;
}

void DemoStateHackFirst::exeDemo() {
    if (al::isFirstStep(this))
        mDemoHackFirstDirector->setHackFirstDemo(mActor, mPlayerHack);

    if (mDemoHackFirstDirector->tryEndDemo())
        endDemo();
}

void DemoStateHackFirst::exeEnd() {}

DemoStateHackFirst* rs::tryCreateDemoStateHackFirst(al::LiveActor* actor,
                                                    const al::ActorInitInfo& initInfo) {
    if (!al::isExistLinkChild(initInfo, "DemoAfterPlayerPos", 0))
        return nullptr;

    return new DemoStateHackFirst(actor, initInfo);
}
