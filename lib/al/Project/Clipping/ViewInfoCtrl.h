#pragma once

#include <basis/seadTypes.h>

namespace al {
class AreaObjGroup;
struct ClippingActorInfo;
class PlacementId;
class PlayerHolder;
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
        const PlacementId* parentId = nullptr;
        bool isInViewCtrlArea = false;
        bool _9 = false;
    };

    inline ClippingPlacementId* findClippingPlacementId_(const PlacementId& id) const;

    const AreaObjGroup* mViewCtrlAreaGroup = nullptr;
    s32 mClippingPlacementIdsSize = 0;
    ClippingPlacementId** mClippingPlacementIds = nullptr;
    bool mIsInvalid = false;
    const PlayerHolder* mPlayerHolder = nullptr;
    const SceneCameraInfo* mSceneCameraInfo = nullptr;
    CheckType mCheckType = CheckType::PlayerPos;
};
}  // namespace al
