#include "Project/Clipping/ClippingDirector.h"

#include "Library/Area/AreaObjDirector.h"
#include "Library/Clipping/ClippingActorHolder.h"
#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/Clipping/ClippingGroupHolder.h"
#include "Library/Clipping/ClippingJudge.h"
#include "Library/Placement/PlacementFunction.h"
#include "Project/Clipping/ClippingFarAreaObserver.h"
#include "Project/Clipping/ViewInfoCtrl.h"

namespace al {

ClippingDirector::ClippingDirector(s32 maxActors, const AreaObjDirector* areaObjDirector,
                                   const PlayerHolder* playerHolder,
                                   const SceneCameraInfo* sceneCameraInfo) {
    mFarAreaObserver = new ClippingFarAreaObserver(areaObjDirector, playerHolder);
    mClippingJudge = new ClippingJudge(mFarAreaObserver, sceneCameraInfo);
    mClippingActorHolder = new ClippingActorHolder(maxActors);
    mGroupHolder = new ClippingGroupHolder();
    mViewInfoCtrl = new ViewInfoCtrl(playerHolder, sceneCameraInfo);
}

ClippingDirector::~ClippingDirector() {
    if (mClippingActorHolder) {
        delete mClippingActorHolder;
        mClippingActorHolder = nullptr;
    }
}

void ClippingDirector::endInit(const AreaObjDirector* areaObjDirector) {
    mViewInfoCtrl->initViewCtrlAreaGroup(areaObjDirector->getAreaObjGroup("ViewCtrlArea"));
    mFarAreaObserver->endInit();
    mClippingActorHolder->endInit(mGroupHolder);
}

void ClippingDirector::setDefaultFarClipDistance(f32 distance) {
    mFarAreaObserver->setDefaultFarClipDistance(distance);
}

void ClippingDirector::setDefaultFarClipDistanceSub(f32 distance) {
    mFarAreaObserver->setDefaultFarClipDistanceSub(distance);
}

f32 ClippingDirector::getFarClipDistance() const {
    return mFarAreaObserver->getFarClipDistance();
}

void ClippingDirector::registerActor(LiveActor* liveActor, const ViewIdHolder* idHolder) {
    ClippingActorInfo* clippingActorInfo = mClippingActorHolder->registerActor(liveActor);
    clippingActorInfo->initViewGroup(idHolder);
    mViewInfoCtrl->initActorInfo(clippingActorInfo);
}

void ClippingDirector::addToGroupClipping(LiveActor* liveActor,
                                          const ActorInitInfo& actorInitInfo) {
    if (alPlacementFunction::isEnableGroupClipping(actorInitInfo))
        mClippingActorHolder->initGroupClipping(liveActor, actorInitInfo);
}

void ClippingDirector::onGroupClipping(LiveActor* liveActor) {
    ClippingActorInfo* clippingActorInfo = mClippingActorHolder->find(liveActor);
    if (clippingActorInfo->isGroupClippingInit()) {
        mClippingActorHolder->onGroupClipping(liveActor);
        mGroupHolder->reentry(clippingActorInfo);
        clippingActorInfo->isEnableGroupClipping = true;
    }
}

void ClippingDirector::offGroupClipping(LiveActor* liveActor) {
    ClippingActorInfo* clippingActorInfo = mClippingActorHolder->find(liveActor);
    if (clippingActorInfo->isGroupClippingInit()) {
        mClippingActorHolder->offGroupClipping(liveActor);
        mGroupHolder->leave(clippingActorInfo);
        clippingActorInfo->isEnableGroupClipping = false;
    }
}

void ClippingDirector::execute() {
    if (!mIsExecute)
        return;
    mFarAreaObserver->update();
    mClippingJudge->update();
    if (mViewInfoCtrl->update())
        mClippingActorHolder->updateFarClipLevel();
    mClippingActorHolder->update(mClippingJudge);
    mGroupHolder->update(mClippingJudge);
}

void ClippingDirector::startCheckViewCtrlByCameraPos() {
    mViewInfoCtrl->startCheckByCameraPos();
}

void ClippingDirector::startCheckViewCtrlByLookAtPos() {
    mViewInfoCtrl->startCheckByLookAtPos();
}

void ClippingDirector::startCheckViewCtrlByPlayerPos() {
    mViewInfoCtrl->startCheckByPlayerPos();
}
}  // namespace al
