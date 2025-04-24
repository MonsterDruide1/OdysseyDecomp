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
    const PlacementId* parentId;
    bool clipFlag;
    bool _9;
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

    enum class CheckType : s32 {
        PlayerPos,
        CameraPos,
        LookAtPos,
    };

    const AreaObjGroup* mAreaGroup = nullptr;
    s32 mPlacementIdSize = 0;
    ClippingPlacementId** mClippingPlacementIds = nullptr;
    bool mIsUpdated = false;
    const PlayerHolder* mPlayerHolder = nullptr;
    const SceneCameraInfo* mSceneCameraInfo = nullptr;
    CheckType mCheckType = CheckType::PlayerPos;
};
}  // namespace al
