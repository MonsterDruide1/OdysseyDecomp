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

ViewInfoCtrl::ClippingPlacementId*
ViewInfoCtrl::findClippingPlacementId_(const PlacementId& id) const {
    for (s32 i = 0; i < mClippingPlacementIdsSize; i++) {
        ClippingPlacementId* clipId = mClippingPlacementIds[i];
        if (clipId->parentId && clipId->parentId->isEqual(id))
            return clipId;
    }
    return nullptr;
}

ViewInfoCtrl::ViewInfoCtrl(const PlayerHolder* playerHolder, const SceneCameraInfo* cameraInfo)
    : mPlayerHolder(playerHolder), mSceneCameraInfo(cameraInfo) {
    mClippingPlacementIds = new ClippingPlacementId*[0x80];
    for (s32 i = 0; i < 0x80; i++)
        mClippingPlacementIds[i] = nullptr;
}

void ViewInfoCtrl::initViewCtrlAreaGroup(const AreaObjGroup* viewCtrlAreaGroup) {
    mViewCtrlAreaGroup = viewCtrlAreaGroup;
}

void ViewInfoCtrl::startCheckByCameraPos() {
    mCheckType = CheckType::CameraPos;
}

void ViewInfoCtrl::startCheckByLookAtPos() {
    mCheckType = CheckType::LookAtPos;
}

void ViewInfoCtrl::startCheckByPlayerPos() {
    mCheckType = CheckType::PlayerPos;
}

void ViewInfoCtrl::initActorInfo(ClippingActorInfo* actorInfo) {
    const ViewIdHolder* viewHolder = actorInfo->getViewIdHolder();
    if (!viewHolder)
        return;

    for (s32 i = 0; i < viewHolder->getNumPlacements(); i++) {
        const PlacementId& viewId = viewHolder->getViewId(i);
        ClippingPlacementId* existingId = findClippingPlacementId_(viewId);
        if (existingId) {
            actorInfo->registerViewGroupFarClipFlag(&existingId->isInViewCtrlArea);
        } else {
            ClippingPlacementId* newId = new ClippingPlacementId;
            newId->parentId = &viewHolder->getViewId(i);
            actorInfo->registerViewGroupFarClipFlag(&newId->isInViewCtrlArea);
            mClippingPlacementIds[mClippingPlacementIdsSize] = newId;
            mClippingPlacementIdsSize++;
        }
    }
}

bool ViewInfoCtrl::update() {
    if (mIsInvalid || !mViewCtrlAreaGroup)
        return false;

    bool prevIsInViewCtrlArea[128];
    for (s32 i = 0; i < mClippingPlacementIdsSize; i++) {
        ClippingPlacementId* clipId = mClippingPlacementIds[i];
        prevIsInViewCtrlArea[i] = clipId->isInViewCtrlArea;
        clipId->isInViewCtrlArea = false;
        clipId->_9 = false;
    }

    s32 playerNumMax = getPlayerNumMax(mPlayerHolder);
    for (s32 i = 0; i < mViewCtrlAreaGroup->getSize(); i++) {
        ViewCtrlArea* viewCtrlArea = (ViewCtrlArea*)mViewCtrlAreaGroup->getAreaObj(i);
        if (!viewCtrlArea->isValid())
            continue;

        bool newIsInViewCtrlArea = false;
        CheckType checkType = mCheckType;
        if (checkType == CheckType::PlayerPos) {
            for (s32 j = 0; j < playerNumMax; j++) {
                if (isPlayerDead(mPlayerHolder, j))
                    continue;
                if (isInAreaPos(viewCtrlArea, getPlayerPos(mPlayerHolder, j))) {
                    newIsInViewCtrlArea = true;
                    break;
                }
            }
        } else {
            for (s32 j = 0; j < getViewNumMax(mSceneCameraInfo); j++) {
                if (!isValidView(mSceneCameraInfo, j))
                    continue;
                if (isInAreaPos(viewCtrlArea, checkType == CheckType::LookAtPos ?
                                                  getCameraAt(mSceneCameraInfo, j) :
                                                  getCameraPos(mSceneCameraInfo, j))) {
                    newIsInViewCtrlArea = true;
                    break;
                }
            }
        }

        if (newIsInViewCtrlArea) {
            const PlacementId* placementId = viewCtrlArea->getPlacementId();
            if (!placementId)
                continue;
            ClippingPlacementId* clippingId = findClippingPlacementId_(*placementId);
            if (clippingId)
                clippingId->isInViewCtrlArea = true;
        }
    }

    for (s32 i = 0; i < mClippingPlacementIdsSize; i++)
        if (prevIsInViewCtrlArea[i] != mClippingPlacementIds[i]->isInViewCtrlArea)
            return true;
    return false;
}
}  // namespace al
