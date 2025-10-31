#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

namespace al {

class PlayerHolder;
class CameraTargetBase;
class CameraSubTargetBase;

struct ViewTargetInfo {
    CameraTargetBase* target = nullptr;
    // Note: s8 is used instead of bool to match isChangeViewTarget
    s8 hasTargetChanged = false;
};

static_assert(sizeof(ViewTargetInfo) == 0x10);

class CameraTargetHolder {
public:
    CameraTargetHolder(s32 maxTargets);

    void initAfterPlacement(const PlayerHolder* holder);
    CameraTargetBase* tryGetViewTarget(s32 index) const;
    void update();
    void addTarget(CameraTargetBase* target);
    void removeTarget(CameraTargetBase* target);
    CameraTargetBase* getViewTarget(s32 index) const;
    bool isChangeViewTarget(s32 index) const;
    CameraSubTargetBase* getTopSubTarget() const;
    void addSubTarget(CameraSubTargetBase* subTarget);
    void removeSubTarget(CameraSubTargetBase* subTarget);
    void addPlacementSubTarget(CameraSubTargetBase* subTarget);
    void removePlacementSubTarget(CameraSubTargetBase* subTarget);
<<<<<<< HEAD

private:
    s32 mViewTargetSize = 0;
    CameraTargetBase** mViewTargetArray = nullptr;
    ViewTargetInfo* mViewTargetInfo = nullptr;
    sead::PtrArray<CameraTargetBase> mTargetArray;
    CameraSubTargetBase* mTopSubTarget = nullptr;
    bool mHasTopSubTargetChanged = false;
    sead::PtrArray<CameraSubTargetBase> mSubTargetArray;
    sead::PtrArray<CameraSubTargetBase> mPlacementSubTargetArray;
=======
    bool isChangeViewTarget(u32 index) const;

    CameraSubTargetBase* getSubTarget() const { return mSubTarget; }

    s8 get_30() const { return _30; }

private:
    void* buffer[5];
    CameraSubTargetBase* mSubTarget;  // This type is unconfirmed and inferred from a function name.
    s8 _30;
>>>>>>> 4474465b (Stashing)
};

static_assert(sizeof(CameraTargetHolder) == 0x58);
}  // namespace al
