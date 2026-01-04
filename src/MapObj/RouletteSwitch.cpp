#include "MapObj/RouletteSwitch.h"

#include <basis/seadTypes.h>

#include "Library/Base/StringUtil.h"
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

void RouletteSwitch::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::initExecutorUpdate(this, info, "地形オブジェ[Movement]");
    al::initStageSwitch(this, info);
    al::initNerve(this, &NrvRouletteSwitch.Wait, 0);
    al::initGroupClipping(this, info);

    for (s32 i = 0; i < mButtons.capacity(); i++) {
        sead::FixedSafeString<0x80>* string = new sead::FixedSafeString<0x80>();
        string->format("Button%d", i + 1);
        al::PlacementInfo placementInfo;
        if (!al::tryGetLinksInfo(&placementInfo, info, string->cstr()))
            break;

        TrampleSwitch* trampleSwitch = new TrampleSwitch(string->cstr());
        al::ActorInitInfo switchInitInfo;
        switchInitInfo.initViewIdHost(&placementInfo, info);
        trampleSwitch->init(switchInitInfo);

        mButtons.pushBack(trampleSwitch);
    }

    al::PlacementInfo placementInfo;
    if (!al::tryGetLinksInfo(&placementInfo, info, "ButtonReset")) {
        makeActorDead();
        return;
    }
    mResetButton = new HipDropSwitch("リセットボタン");
    al::ActorInitInfo switchInitInfo;
    switchInitInfo.initViewIdHost(&placementInfo, info);
    mResetButton->init(switchInitInfo);
    mIsPressReset = mResetButton->isOnWait();
    makeActorAlive();
}

void RouletteSwitch::control() {
    if (!mIsPressReset && mResetButton->isOnWait()) {
        for (s32 i = 0; i < mButtons.size(); i++)
            mButtons[i]->offSwitch();
        mIsPressReset = mResetButton->isOnWait();
        return;
    }
    bool isAnySwitchOn = false;
    for (s32 i = 0; i < mButtons.size(); i++) {
        al::StringTmp<256> string;
        string.format("SwitchButton%dOn", i + 1);
        if (!mButtons[i]->isOn()) {
            al::offStageSwitch(this, string.cstr());
        } else {
            al::onStageSwitch(this, string.cstr());
            isAnySwitchOn = true;
        }
    }

    if (isAnySwitchOn && mResetButton->isOnWait())
        mResetButton->reset();
    mIsPressReset = mResetButton->isOnWait();
}

void RouletteSwitch::exeWait() {}
