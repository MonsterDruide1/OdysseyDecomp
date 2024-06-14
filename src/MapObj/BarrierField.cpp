#include "MapObj/BarrierField.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/Placement/PlacementFunction.h"

#include "System/GameDataHolderAccessor.h"
#include "System/GameDataFunction.h"

static alNerveFunction::NerveActionCollector sBarrierFieldNerveCollector;

BarrierField::BarrierField(const char* name) : al::LiveActor(name){}

/*
void BarrierField::init(const al::ActorInitInfo& initInfo) {
    al::initNerveAction(this, "Hide", &sBarrierFieldNerveCollector, 0);
    al::initMapPartsActor(this, initInfo, nullptr);
    al::tryGetArg(&mIsDisappearByShineGet, initInfo, "IsDisappearByShineGet");
    mIsOnTheMoon = al::isObjectName(initInfo, "WaterfallWorldHomeBarrier") && GameDataFunction::isWorldMoon(GameDataHolderAccessor(this));
    al::trySyncStageSwitchAppearAndKill(this);
}
*/

// NON_MATCHING

void BarrierField::init(const al::ActorInitInfo& initInfo) {
    al::initNerveAction(this, "Hide", &sBarrierFieldNerveCollector, 0);
    al::initMapPartsActor(this, initInfo, nullptr);
    al::tryGetArg(&mIsDisappearByShineGet, initInfo, "IsDisappearByShineGet");
    if (al::isObjectName(initInfo, "WaterfallWorldHomeBarrier")){
        GameDataHolderAccessor dataAccessor(this);
        if(GameDataFunction::isWorldMoon(dataAccessor)){
            mIsOnTheMoon = true;
        }
    }
    al::trySyncStageSwitchAppearAndKill(this);
}
