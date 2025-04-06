#include "Project/Clipping/ClippingDirector.h"

#include "Library/Area/AreaObjDirector.h"
#include "Library/Clipping/ClippingActorHolder.h"
#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/Clipping/ClippingJudge.h"
#include "Library/Placement/PlacementFunction.h"
#include "Project/Clipping/ClippingFarAreaObserver.h"
#include "Project/Clipping/ViewInfoCtrl.h"

namespace al {

void ClippingDirector::addToGroupClipping(LiveActor* liveActor,
                                          const ActorInitInfo& actorInitInfo) {
    bool isEnableGroupClipping = alPlacementFunction::isEnableGroupClipping(actorInitInfo);
    if (isEnableGroupClipping)
        mClippingActorHolder->initGroupClipping(liveActor, actorInitInfo);
}

void ClippingDirector::endInit(const AreaObjDirector* areaObjDirector) {
    ViewInfoCtrl* viewInfoCtrl = mViewInfoCtrl;
    AreaObjGroup* areaObjGroup = areaObjDirector->getAreaObjGroup("ViewCtrlArea");
    viewInfoCtrl->initViewCtrlAreaGroup(areaObjGroup);
    mFarAreaObserver->endInit();
    mClippingActorHolder->endInit(mGroupHolder);
}

void ClippingDirector::execute() {
    if (mIsExecute) {
        mFarAreaObserver->update();
        mClippingJudge->update();
        bool isUpdate = mViewInfoCtrl->update();
        if (isUpdate)
            mClippingActorHolder->updateFarClipLevel();
        mClippingActorHolder->update(mClippingJudge);
        mGroupHolder->update(mClippingJudge);
    }
}

f32 ClippingDirector::getFarClipDistance() const {
    return mFarAreaObserver->getFarClipDistance();
}

void ClippingDirector::offGroupClipping(LiveActor* liveActor) {
    ClippingActorInfo* clippingActorInfo = mClippingActorHolder->find(liveActor);
    if (clippingActorInfo->isGroupClippingInit()) {
        mClippingActorHolder->offGroupClipping(liveActor);
        mGroupHolder->leave(clippingActorInfo);
        clippingActorInfo->setIsClipping(false);
    }
}

void ClippingDirector::onGroupClipping(LiveActor* liveActor) {
    ClippingActorInfo* clippingActorInfo = mClippingActorHolder->find(liveActor);
    if (clippingActorInfo->isGroupClippingInit()) {
        mClippingActorHolder->onGroupClipping(liveActor);
        mGroupHolder->leave(clippingActorInfo);
        clippingActorInfo->setIsClipping(true);
    }
}

void ClippingDirector::registerActor(LiveActor* liveActor, const ViewIdHolder* idHolder) {
    ClippingActorInfo* clippingActorInfo = mClippingActorHolder->registerActor(liveActor);
    clippingActorInfo->initViewGroup(idHolder);
    mViewInfoCtrl->initActorInfo(clippingActorInfo);
}

void ClippingDirector::setDefaultFarClipDistance(f32 distance) {
    mFarAreaObserver->setDefaultFarClipDistance(distance);
}

void ClippingDirector::setDefaultFarClipDistanceSub(f32 distance) {
    mFarAreaObserver->setDefaultFarClipDistanceSub(distance);
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

ClippingDirector::ClippingDirector(s32 index, const AreaObjDirector* areaObjDirector,
                                   const PlayerHolder* playerHolder,
                                   const SceneCameraInfo* sceneCameraInfo) {
    mFarAreaObserver = new ClippingFarAreaObserver(areaObjDirector, playerHolder);
    mClippingJudge = new ClippingJudge(mFarAreaObserver, sceneCameraInfo);
    mClippingActorHolder = new ClippingActorHolder(index);
    mGroupHolder = new ClippingGroupHolder();
    mViewInfoCtrl = new ViewInfoCtrl(playerHolder, sceneCameraInfo);
}

ClippingDirector::~ClippingDirector() {
    if (mClippingActorHolder) {
        delete mClippingActorHolder;
        mClippingActorHolder = nullptr;
    }
}

}  // namespace al
