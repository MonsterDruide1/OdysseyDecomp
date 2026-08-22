#include "Item/ShineFukankunWatchObj.h"

#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"

#include "Item/Shine.h"
#include "MapObj/FukankunZoomTargetFunction.h"
#include "Util/ItemUtil.h"

namespace {
NERVE_IMPL(ShineFukankunWatchObj, Wait);

NERVES_MAKE_NOSTRUCT(ShineFukankunWatchObj, Wait);
}  // namespace

ShineFukankunWatchObj::ShineFukankunWatchObj(const char* name) : al::LiveActor(name) {}

void ShineFukankunWatchObj::init(const al::ActorInitInfo& info) {
    al::initActorSceneInfo(this, info);
    al::initActorPoseTQSV(this);
    al::initActorSRT(this, info);
    al::initExecutorWatchObj(this, info);
    al::initActorClipping(this, info);
    al::setClippingInfo(this, 500.0f, nullptr);
    al::initNerve(this, &Wait, 0);
    FukankunZoomTargetFunction::declareUseFukankunZoomTargetActor(this);

    mShine = rs::initShineByPlacementInfo(info);
    mShine->initAppearDemoFromHost(info, al::getTrans(this));

    makeActorAlive();
}

void ShineFukankunWatchObj::initAfterPlacement() {
    return FukankunZoomTargetFunction::registerFukankunZoomTargetActor(
        this, 0, sead::Vector3f::zero, nullptr);
}

void ShineFukankunWatchObj::exeWait() {
    if (FukankunZoomTargetFunction::getWatchCount(this) >= 121) {
        const sead::Matrix34f* linkedShineMtx = nullptr;
        if (FukankunZoomTargetFunction::tryGetActiveFukankunLinkedShineMtx(&linkedShineMtx, this)) {
            sead::Vector3f linkedShinePos;
            linkedShineMtx->getTranslation(linkedShinePos);
            mShine->appearWarp(al::getTrans(this), linkedShinePos);
            kill();
        }
    }
}
