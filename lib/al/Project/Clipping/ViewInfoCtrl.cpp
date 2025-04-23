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
    mPlayerHolder = playerHolder;
    mSceneCameraInfo = cameraInfo;
    mClippingPlacementIds = new ClippingPlacementId*[0x80];
    for (s32 i = 0; i < 0x80; i++)
        mClippingPlacementIds[i] = nullptr;
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
            if (id->parentId && id->parentId->isEqual(viewId)) {
                if (id) {
                    actorInfo->registerViewGroupFarClipFlag(&id->clipFlag);
                    found = true;
                }
                break;
            }
        }
        if (!found) {
            ClippingPlacementId* newId = new ClippingPlacementId{nullptr, false, false};
            newId->parentId = &viewHolder->getViewId(i);
            actorInfo->registerViewGroupFarClipFlag(&newId->clipFlag);
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
        flags[i] = clipId->clipFlag;
        clipId->clipFlag = false;
        clipId->flag2 = false;
    }

    s32 playerNumMax = getPlayerNumMax(mPlayerHolder);
    for (s32 i = 0; i < mAreaGroup->getSize(); i++) {
        ViewCtrlArea* viewCtrlArea = (ViewCtrlArea*)mAreaGroup->getAreaObj(i);
        if (!viewCtrlArea->isValid())
            continue;

        bool shouldSetClip = false;
        s32 checkType = mCheckType;
        if (checkType == 0) {
            for (s32 j = 0; j < playerNumMax; j++) {
                if (isPlayerDead(mPlayerHolder, j))
                    continue;
                if (isInAreaPos(viewCtrlArea, getPlayerPos(mPlayerHolder, j))) {
                    shouldSetClip = true;
                    break;
                }
            }
        } else {
            for (s32 j = 0; j < getViewNumMax(mSceneCameraInfo); j++) {
                if (!isValidView(mSceneCameraInfo, j))
                    continue;
                if (isInAreaPos(viewCtrlArea, checkType == 2 ? getCameraAt(mSceneCameraInfo, j) :
                                                               getCameraPos(mSceneCameraInfo, j))) {
                    shouldSetClip = true;
                    break;
                }
            }
        }

        if (shouldSetClip) {
            const PlacementId* placementId = viewCtrlArea->getPlacementId();
            if (!placementId)
                continue;
            for (s32 j = 0; j < mPlacementIdSize; j++) {
                ClippingPlacementId* clippingId = mClippingPlacementIds[j];
                if (clippingId->parentId && clippingId->parentId->isEqual(*placementId)) {
                    if (clippingId)
                        clippingId->clipFlag = true;
                    break;
                }
            }
        }
    }

    for (s32 i = 0; i < mPlacementIdSize; i++)
        if (flags[i] != mClippingPlacementIds[i]->clipFlag)
            return true;
    return false;
}
}  // namespace al
