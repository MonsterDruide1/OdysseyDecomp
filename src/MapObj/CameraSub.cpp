#include "MapObj/CameraSub.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObj.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Draw/GraphicsSystemInfo.h"
#include "Library/Draw/SubCameraRenderer.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

#include "Util/PlayerUtil.h"

namespace {
NERVE_IMPL(CameraSub, Wait);
NERVE_IMPL(CameraSub, Active);

NERVES_MAKE_NOSTRUCT(CameraSub, Wait, Active);
}  // namespace

CameraSub::CameraSub(const char* name) : al::LiveActor(name) {}

void CameraSub::init(const al::ActorInitInfo& info) {
    using CameraSubFunctor = al::FunctorV0M<CameraSub*, void (CameraSub::*)()>;

    al::initActorSceneInfo(this, info);
    al::initActorPoseTRSV(this);
    al::initActorSRT(this, info);
    al::initActorClipping(this, info);
    al::initExecutorUpdate(this, info, "地形オブジェ[Movement]");
    al::initStageSwitch(this, info);

    mCameraTicket = al::initObjectCamera(this, info, nullptr, nullptr);
    mSubCameraRenderer = info.actorSceneInfo.graphicsSystemInfo->getSubCameraRenderer();
    mSubCameraRenderer->addCameraSub();

    al::initNerve(this, &Wait, 0);

    mAreaObj = new al::AreaObj("カメラエリア[DRC]");

    al::AreaInitInfo areaInitInfo(*info.placementInfo, info.stageSwitchDirector,
                                  info.actorSceneInfo.sceneObjHolder);
    mAreaObj->init(areaInitInfo);

    al::invalidateClipping(this);

    al::PlacementInfo screenInfo;
    if (al::tryGetLinksInfo(&screenInfo, info, "Screen")) {
        mHasScreenInfo = true;
        al::tryGetMatrixTR(&mScreenMatrix, screenInfo);
        al::tryGetScale(&mScreenScale, screenInfo);
    }

    makeActorAlive();

    if (al::listenStageSwitchOnAppear(this, CameraSubFunctor(this, &CameraSub::onAppear)))
        makeActorDead();
}

void CameraSub::onAppear() {
    al::LiveActor::appear();
}

void CameraSub::initAfterPlacement() {
    if (mHasScreenInfo) {
        al::CameraSubAreaScreenInfo* screenInfo =
            new al::CameraSubAreaScreenInfo(mScreenScale, mScreenMatrix, mAreaObj);
        mSubCameraRenderer->addCameraSubAreaScreenInfo(screenInfo);
    }
}

void CameraSub::exeWait() {
    if (al::isActiveCamera(mCameraTicket))
        return;

    const sead::Vector3f& playerPos = rs::getPlayerPos(this);
    if (mAreaObj->isInVolume(playerPos)) {
        al::startCameraSub(this, mCameraTicket, -1);
        al::setNerve(this, &Active);
    }
}

void CameraSub::exeActive() {
    if (!al::isActiveCamera(mCameraTicket))
        return;

    const sead::Vector3f& playerPos = rs::getPlayerPos(this);
    if (!mAreaObj->isInVolume(playerPos)) {
        al::endCameraSub(this, mCameraTicket, -1);
        al::setNerve(this, &Wait);
    }
}
