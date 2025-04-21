#pragma once

#include <basis/seadTypes.h>

namespace al {
class ClippingActorInfo;
class ViewIdHolder;
class PlayerHolder;
class PlacementId;
class AreaObjGroup;
class SceneCameraInfo;

struct ClippingPlacementId {
    const PlacementId* mParentId;
    bool mClipFlag;
    bool mFlag2;
};

class ViewInfoCtrl {
public:
    ViewInfoCtrl(const PlayerHolder* playerHolder, const SceneCameraInfo* cameraInfo);
    void initActorInfo(ClippingActorInfo* actorInfo);
    void initViewCtrlAreaGroup(const AreaObjGroup* areaGroup);
    bool update();
    void startCheckByCameraPos();
    void startCheckByLookAtPos();
    void startCheckByPlayerPos();
private:
    const AreaObjGroup* mAreaGroup = nullptr;
    s32 mPlacementIdSize = 0;

    ClippingPlacementId** mClippingPlacementIds = nullptr;
    bool mIsUpdated ;
    ViewIdHolder* mCameraInfo = nullptr;
    s32 mCheckType = 0;
    const PlayerHolder* mPlayerHolder = nullptr;
    const SceneCameraInfo* mSceneCameraInfo = nullptr;
};
} // namespace al