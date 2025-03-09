#pragma once

#include <basis/seadTypes.h>

namespace al {

class PlayerHolder;
class CameraTargetBase;
class CameraSubTargetBase;

class CameraTargetHolder {
public:
    CameraTargetHolder(u32 maxNum);

    CameraTargetBase* tryGetViewTarget(u32 num);
    CameraTargetBase* getViewTarget(u32 num);
    CameraSubTargetBase* getTopSubTarget();

    void initAfterPlacement(const PlayerHolder*);
    void update();
    void addTarget(CameraTargetBase* target);
    void removeTarget(const CameraTargetBase* target);
    void addSubTarget(CameraSubTargetBase* subTarget);
    void removeSubTarget(CameraSubTargetBase* subTarget);
    void addPlacementSubTarget(CameraSubTargetBase* subTarget);
    void removePlacementSubTarget(CameraSubTargetBase* subTarget);
    bool isChangeViewTarget(u32 num);
};
}  // namespace al
