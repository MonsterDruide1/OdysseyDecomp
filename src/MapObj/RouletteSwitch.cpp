#include "MapObj/RouletteSwitch.h"

#include <basis/seadTypes.h>

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "MapObj/HipDropSwitch.h"
#include "MapObj/TrampleSwitch.h"

namespace {
NERVE_IMPL(RouletteSwitch, Wait);

NERVES_MAKE_STRUCT(RouletteSwitch, Wait);
}  // namespace

RouletteSwitch::RouletteSwitch(const char* name) : al::LiveActor(name) {}

void RouletteSwitch::init(const al::ActorInitInfo& actorInitInfo) {
    al::initActorSceneInfo(this, actorInitInfo);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, actorInitInfo);
    al::initActorClipping(this, actorInitInfo);
    al::initExecutorUpdate(this, actorInitInfo, "地形オブジェ[Movement]");
    al::initStageSwitch(this, actorInitInfo);
    al::initNerve(this, &NrvRouletteSwitch.Wait, 0);
    al::initGroupClipping(this, actorInitInfo);

    for (s32 i = 0; i < mTrampleSwitchArray.capacity(); i++) {
        sead::FixedSafeString<0x80>* string = new sead::FixedSafeString<0x80>();
        string->format("Button%d", i + 1);
        al::PlacementInfo placementInfo;
        if (!al::tryGetLinksInfo(&placementInfo, actorInitInfo, string->cstr()))
            break;

        TrampleSwitch* trampleSwitch = new TrampleSwitch(string->cstr());
        al::ActorInitInfo switchInitInfo;
        switchInitInfo.initViewIdHost(&placementInfo, actorInitInfo);
        trampleSwitch->init(switchInitInfo);

        mTrampleSwitchArray.pushBack(trampleSwitch);
    }

    al::PlacementInfo placementInfo;
    if (!al::tryGetLinksInfo(&placementInfo, actorInitInfo, "ButtonReset")) {
        makeActorDead();
    } else {
        mHipDropSwitch = new HipDropSwitch("リセットボタン");
        al::ActorInitInfo switchInitInfo;
        switchInitInfo.initViewIdHost(&placementInfo, actorInitInfo);
        mHipDropSwitch->init(switchInitInfo);
        mIsOnWait = mHipDropSwitch->isOnWait();
        makeActorAlive();
    }
}

void RouletteSwitch::control() {
    if (!mIsOnWait && mHipDropSwitch->isOnWait()) {
        for (s32 i = 0; i < mTrampleSwitchArray.size(); i++)
            mTrampleSwitchArray[i]->offSwitch();
    } else {
        bool isAnySwitchOn = false;
        for (s32 i = 0; i < mTrampleSwitchArray.size(); i++) {
            sead::FixedSafeString<0x100> string;
            string.format("SwitchButton%dOn", i + 1);
            if (!mTrampleSwitchArray[i]->isOn()) {
                al::offStageSwitch(this, string.cstr());
            } else {
                al::onStageSwitch(this, string.cstr());
                isAnySwitchOn = true;
            }
        }

        if (isAnySwitchOn && mHipDropSwitch->isOnWait())
            mHipDropSwitch->reset();
    }
    mIsOnWait = mHipDropSwitch->isOnWait();
}

void RouletteSwitch::exeWait() {}
