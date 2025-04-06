#include "MapObj/SaveFlagCheckObj.h"

#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"

#include "System/GameDataFunction.h"

SaveFlagCheckObj::SaveFlagCheckObj(const char* name) : al::LiveActor(name) {}

void SaveFlagCheckObj::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initStageSwitch(this, info);

    const char* stageName = "Test";
    const char* objId = "Test";
    al::getStringArg(&stageName, info, "StageName");
    al::getStringArg(&objId, info, "ObjId");

    if (GameDataFunction::isObjStarted(this, stageName, objId))
        al::tryOnStageSwitch(this, "SwitchSaveOn");

    makeActorDead();
}
