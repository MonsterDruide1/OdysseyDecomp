#pragma once

#include <basis/seadTypes.h>

namespace al {
class ClippingActorInfo;
class ViewIdHolder;
class PlayerHolder;
class PlacementId;
class AreaObjGroup;
class SceneCameraInfo;

class ViewInfoCtrl {
public:
    ViewInfoCtrl(const PlayerHolder* playerHolder, const SceneCameraInfo* cameraInfo);
    void initActorInfo(ClippingActorInfo* actorInfo);
    void initViewCtrlAreaGroup(const AreaObjGroup* viewCtrlAreaGroup);
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

    struct ClippingPlacementId {
        const PlacementId* parentId;
        bool isInViewCtrlArea;
        bool _9;
    };

    const AreaObjGroup* mViewCtrlAreaGroup = nullptr;
    s32 mPlacementIdSize = 0;
    ClippingPlacementId** mClippingPlacementIds = nullptr;
    bool mIsUpdated = false;
    const PlayerHolder* mPlayerHolder = nullptr;
    const SceneCameraInfo* mSceneCameraInfo = nullptr;
    CheckType mCheckType = CheckType::PlayerPos;
};
}  // namespace al
