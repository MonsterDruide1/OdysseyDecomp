#pragma once

<<<<<<< HEAD
namespace al {
class SceneCameraInfo;
class PlayerHolder;
class AreaObjGroup;
class ClippingActorInfo;

class ViewInfoCtrl {
public:
    ViewInfoCtrl(const PlayerHolder*, const SceneCameraInfo*);

    void initActorInfo(ClippingActorInfo*);
    void initViewCtrlAreaGroup(const AreaObjGroup*);
=======
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
>>>>>>> f1a16e51 (uploaded because need to rebase other pr)
    bool update();
    void startCheckByCameraPos();
    void startCheckByLookAtPos();
    void startCheckByPlayerPos();
<<<<<<< HEAD

private:
    void* filler[0x7];
};

}  // namespace al
=======
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
>>>>>>> f1a16e51 (uploaded because need to rebase other pr)
