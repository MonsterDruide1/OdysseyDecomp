#include "Project/Clipping/ViewInfoCtrl.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Camera/CameraUtil.h"
#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/Clipping/ViewIdHolder.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Play/Area/ViewCtrlArea.h"
#include "Library/Player/PlayerUtil.h"

namespace al {


ViewInfoCtrl::ViewInfoCtrl(const PlayerHolder* playerHolder, const SceneCameraInfo* cameraInfo) {
    mAreaGroup = nullptr;
    mPlacementIdSize = 0;
    mIsUpdated = false;
    mPlayerHolder = playerHolder;
    mSceneCameraInfo = cameraInfo;
    mClippingPlacementIds = new ClippingPlacementId*;
    for (s32 i = 0; true; i++) {
        mClippingPlacementIds[i]->mParentId = nullptr;
        mClippingPlacementIds[i]->mFlag2 = false;
        mClippingPlacementIds[i]->mClipFlag = false;
        if (i == 992) break;
    }
}

void ViewInfoCtrl::initViewCtrlAreaGroup(const AreaObjGroup* areaGroup) {
    mAreaGroup = areaGroup;
}

void ViewInfoCtrl::startCheckByCameraPos() {
    mCheckType = 1;
}

void ViewInfoCtrl::startCheckByLookAtPos() {
    mCheckType = 2;
}

void ViewInfoCtrl::startCheckByPlayerPos() {
    mCheckType = 0;
}

void ViewInfoCtrl::initActorInfo(ClippingActorInfo* actorInfo) {
    ViewIdHolder* viewHolder = actorInfo->getViewIdHolder();
    if (viewHolder == nullptr)
        return;

    for (s32 i = 0; i < viewHolder->getNumPlacements(); i++) {
        bool found = false;
        const PlacementId& viewId = viewHolder->getViewId(i);
        for (s32 j = 0; j < mPlacementIdSize; j++) {
            const ClippingPlacementId* id = mClippingPlacementIds[j];
            if (id->mParentId && id->mParentId->isEqual(viewId)){
                if(id){
                    actorInfo->registerViewGroupFarClipFlag(&id->mClipFlag);
                    found = true;
                }
                break;
            }
        }
        if(!found){
            ClippingPlacementId* newId = new ClippingPlacementId{nullptr, false,false};
            newId->mParentId = &viewHolder->getViewId(i);
            actorInfo->registerViewGroupFarClipFlag(&newId->mClipFlag);
            mClippingPlacementIds[mPlacementIdSize] = newId;
            mPlacementIdSize++;
        }
    }
}

bool ViewInfoCtrl::update() {
    if (mIsUpdated || !mAreaGroup)
        return false;

    bool flags[128];
    for (s32 i = 0; i < mPlacementIdSize; i++) {
        ClippingPlacementId* clipId = mClippingPlacementIds[i];
        flags[i] = clipId->mClipFlag;
        clipId->mClipFlag = false;
        clipId->mFlag2 = false;
    }

    s32 playerNumMax = getPlayerNumMax(mPlayerHolder);
    for (s32 i = 0; i < mAreaGroup->getSize(); i++) {
        ViewCtrlArea* viewCtrlArea = (ViewCtrlArea*)mAreaGroup->getAreaObj(i);
        if (!viewCtrlArea->isValid()) {
            continue;
        }

        bool shouldSetClip = false;
        auto checkType = mCheckType;
        if (checkType == 0) {
            for (s32 j = 0; j < playerNumMax; j++) {
                if (al::isPlayerDead(mPlayerHolder, j)) continue;
                if (al::isInAreaPos(viewCtrlArea, al::getPlayerPos(mPlayerHolder, j))) {
                    shouldSetClip = true;
                    break;
                }
            }
        } else {
            for (s32 j = 0; j < getViewNumMax(mSceneCameraInfo); j++) {
                if (!isValidView(mSceneCameraInfo, j))
                    continue;
                if (isInAreaPos(viewCtrlArea, checkType == 2 ? getCameraAt(mSceneCameraInfo, j) : getCameraPos(mSceneCameraInfo, j))) {
                    shouldSetClip = true;
                    break;
                }
            }
        }

        if (shouldSetClip) {
            const PlacementId* placementId = viewCtrlArea->getPlacementId();
            if (!placementId) {
                continue;
            }
            for (s32 j = 0; j < mPlacementIdSize; j++) {
                ClippingPlacementId* clippingId = mClippingPlacementIds[j];
                if (clippingId->mParentId && clippingId->mParentId->isEqual(*placementId)) {
                    if (clippingId)
                        clippingId->mClipFlag = true;
                    break;
                }
            }
        }
    }

    for (s32 i = 0; i < mPlacementIdSize; i++) {
        if (flags[i] != mClippingPlacementIds[i]->mClipFlag)
            return true;
    }
    return false;
}
} // namespace al