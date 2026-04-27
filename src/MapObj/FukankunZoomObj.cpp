#include "MapObj/FukankunZoomObj.h"

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Rail/RailUtil.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace {
NERVE_IMPL(FukankunZoomObj, Wait);

NERVES_MAKE_STRUCT(FukankunZoomObj, Wait);
}  // namespace

FukankunZoomObj::FukankunZoomObj(const char* name) : al::LiveActor(name) {}

void FukankunZoomObj::init(const al::ActorInitInfo& info) {
    using FukankunZoomObjFunctor = al::FunctorV0M<FukankunZoomObj*, void (FukankunZoomObj::*)()>;

    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::initExecutorUpdate(this, info, "地形オブジェ[Movement]");
    al::initStageSwitch(this, info);
    al::initNerve(this, &NrvFukankunZoomObj.Wait, 0);
    al::initGroupClipping(this, info);

    if (al::isExistRail(info, "Rail")) {
        initRailKeeper(info, "Rail");
        al::setSyncRailToNearestPos(this);
    }

    al::tryGetArg(&mRailMoveSpeed, info, "RailMoveSpeed");
    al::tryGetArg((s32*)&mMoveType, info, "MoveType");

    makeActorAlive();
    if (al::listenStageSwitchOnAppear(this,
                                      FukankunZoomObjFunctor(this, &FukankunZoomObj::onAppear)))
        makeActorDead();

    al::listenStageSwitchOnKill(this, FukankunZoomObjFunctor(this, &FukankunZoomObj::onKill));
}

void FukankunZoomObj::onAppear() {
    al::LiveActor::appear();
}

void FukankunZoomObj::onKill() {
    al::LiveActor::kill();
}

void FukankunZoomObj::attackSensor(al::HitSensor* self, al::HitSensor* other) {}

bool FukankunZoomObj::receiveMsg(const al::SensorMsg* message, al::HitSensor* other,
                                 al::HitSensor* self) {
    return false;
}

void FukankunZoomObj::exeWait() {
    if (!al::isExistRail(this))
        return;

    if (mMoveType == al::MoveType::Loop) {
        al::moveSyncRailLoop(this, mRailMoveSpeed);
        return;
    }

    if (mMoveType == al::MoveType::Turn) {
        al::moveSyncRailTurn(this, mRailMoveSpeed);
        return;
    }

    al::moveSyncRail(this, mRailMoveSpeed);
}
