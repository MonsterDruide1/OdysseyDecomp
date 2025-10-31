#pragma once

#include <basis/seadTypes.h>

namespace al {

class PlayerHolder;
class CameraTargetBase;
class CameraSubTargetBase;

class CameraTargetHolder {
public:
    CameraTargetHolder(u32 maxTargets);

    CameraTargetBase* tryGetViewTarget(u32 index) const;
    CameraTargetBase* getViewTarget(u32 index) const;
    CameraSubTargetBase* getTopSubTarget() const;

    void initAfterPlacement(const PlayerHolder*);
    void update();
    void addTarget(CameraTargetBase* target);
    void removeTarget(const CameraTargetBase* target);
    void addSubTarget(CameraSubTargetBase* subTarget);
    void removeSubTarget(CameraSubTargetBase* subTarget);
    void addPlacementSubTarget(CameraSubTargetBase* subTarget);
    void removePlacementSubTarget(CameraSubTargetBase* subTarget);
    bool isChangeViewTarget(u32 index) const;

    CameraSubTargetBase* getSubTarget() const { return mSubTarget; }

    s8 get_30() const { return _30; }

private:
    void* buffer[5];
    CameraSubTargetBase* mSubTarget;  // This type is unconfirmed and inferred from a function name.
    s8 _30;
};
}  // namespace al
