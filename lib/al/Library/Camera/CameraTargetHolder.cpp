#include "Library/Camera/CameraTargetHolder.h"

#include "Library/Camera/ActorCameraTarget.h"
#include "Library/Camera/CameraSubTargetBase.h"
#include "Library/Camera/CameraTargetBase.h"
#include "Library/Player/PlayerUtil.h"

namespace al {

CameraTargetHolder::CameraTargetHolder(s32 maxTargets) : mViewTargetSize(maxTargets) {
    mViewTargetArray = new CameraTargetBase*[maxTargets];
    mViewTargetInfo = new ViewTargetInfo[maxTargets];

    for (s32 i = 0; i < mViewTargetSize; i++)
        mViewTargetArray[i] = nullptr;

    mTargetArray.allocBuffer(32, nullptr);
    mSubTargetArray.allocBuffer(32, nullptr);
    mPlacementSubTargetArray.allocBuffer(32, nullptr);
}

void CameraTargetHolder::initAfterPlacement(const PlayerHolder* holder) {
    if (mTargetArray.isEmpty()) {
        for (s32 i = 0; i < getPlayerNumMax(holder); i++) {
            if (mTargetArray.isFull())
                break;

            mTargetArray.pushBack(new ActorCameraTarget(getPlayerActor(holder, i), 0.0f, nullptr));
        }
    }

    for (s32 i = 0; i < mViewTargetSize; i++) {
        ViewTargetInfo* info = &mViewTargetInfo[i];
        info->target = getViewTarget(i);
    }
}

CameraTargetBase* CameraTargetHolder::tryGetViewTarget(s32 index) const {
    CameraTargetBase* target = mViewTargetArray[index];
    if (target)
        return target;

    if (mTargetArray.size() > 0)
        return mTargetArray.front();

    return nullptr;
}

void CameraTargetHolder::update() {
    for (s32 i = 0; i < mViewTargetSize; i++) {
        ViewTargetInfo* info = &mViewTargetInfo[i];
        CameraTargetBase* target = getViewTarget(i);

        info->hasTargetChanged = info->target != target;
        info->target = target;

        if (target)
            target->update();
    }

    CameraSubTargetBase* topSubTarget = nullptr;
    if (!mSubTargetArray.isEmpty())
        topSubTarget = mSubTargetArray.front();
    else if (!mPlacementSubTargetArray.isEmpty())
        topSubTarget = mPlacementSubTargetArray.front();

    mHasTopSubTargetChanged = mTopSubTarget != topSubTarget;
    mTopSubTarget = topSubTarget;
    if (topSubTarget)
        topSubTarget->update();
}

void CameraTargetHolder::addTarget(CameraTargetBase* target) {
    s32 index = mTargetArray.indexOf(target);
    if (index > -1)
        mTargetArray.erase(index);
    target->enableTarget();
    mTargetArray.pushFront(target);
}

void CameraTargetHolder::removeTarget(CameraTargetBase* target) {
    s32 index = mTargetArray.indexOf(target);
    if (index > -1) {
        target->disableTarget();
        mTargetArray.erase(index);
    }

    for (s32 i = 0; i < mViewTargetSize; i++) {
        if (mViewTargetArray[i] == target) {
            target->disableTarget();
            mViewTargetArray[i] = nullptr;
        }
    }
}

CameraTargetBase* CameraTargetHolder::getViewTarget(s32 index) const {
    return tryGetViewTarget(index);
}

bool CameraTargetHolder::isChangeViewTarget(s32 index) const {
    return mViewTargetInfo[index].hasTargetChanged;
}

CameraSubTargetBase* CameraTargetHolder::getTopSubTarget() const {
    return mTopSubTarget;
}

// NON_MATCHING: Depends on removeSubTarget
void CameraTargetHolder::addSubTarget(CameraSubTargetBase* subTarget) {
    removeSubTarget(subTarget);
    subTarget->enableTarget();
    mSubTargetArray.pushFront(subTarget);
}

// NON_MATCHING: https://decomp.me/scratch/qrlL5
void CameraTargetHolder::removeSubTarget(CameraSubTargetBase* subTarget) {
    s32 index = mSubTargetArray.indexOf(subTarget);
    if (index > -1)
        mSubTargetArray.erase(index);
    subTarget->disableTarget();
}

// NON_MATCHING: Depends on removePlacementSubTarget
void CameraTargetHolder::addPlacementSubTarget(CameraSubTargetBase* subTarget) {
    removePlacementSubTarget(subTarget);
    subTarget->enableTarget();
    mPlacementSubTargetArray.pushFront(subTarget);
}

// NON_MATCHING: https://decomp.me/scratch/Sv3mo
void CameraTargetHolder::removePlacementSubTarget(CameraSubTargetBase* subTarget) {
    s32 index = mPlacementSubTargetArray.indexOf(subTarget);
    if (index > -1)
        mPlacementSubTargetArray.erase(index);
    subTarget->disableTarget();
}

}  // namespace al
