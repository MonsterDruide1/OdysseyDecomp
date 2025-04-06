#include "Project/Clipping/ClippingDirector.h"

#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Clipping/ClippingActorHolder.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Clipping/ClippingJudge.h"

#include "Project/Clipping/ViewInfoCtrl.h"
#include "Project/Clipping/ClippingFarAreaObserver.h"

namespace al {

void ClippingDirector::addToGroupClipping(LiveActor* liveActor,
                                          const ActorInitInfo& actorInitInfo) {
    bool isEnableGroupClipping = alPlacementFunction::isEnableGroupClipping(actorInitInfo);
    if (isEnableGroupClipping) {
        mClippingActorHolder->initGroupClipping(liveActor, actorInitInfo);
    }
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
        if (isUpdate) {
            mClippingActorHolder->updateFarClipLevel();
        }
        mClippingActorHolder->update(mClippingJudge);
        mGroupHolder->update(mClippingJudge);
    }
}



} // namespace al