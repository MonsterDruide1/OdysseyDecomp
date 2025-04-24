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
    const PlacementId* parentId;
    bool clipFlag;
    bool _9;
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
<<<<<<< HEAD

private:
<<<<<<< HEAD
<<<<<<< HEAD
    void* filler[0x7];
};

}  // namespace al
=======
=======

>>>>>>> 02efc0b3 (Fixed issues)
private:
=======
=======

>>>>>>> 6da971f2 (Fixed linter)
    enum class CheckType : s32 {
        PlayerPos,
        CameraPos,
        LookAtPos,
    };

>>>>>>> f1118b6c (Commiting for merge)
    const AreaObjGroup* mAreaGroup = nullptr;
    s32 mPlacementIdSize = 0;
    ClippingPlacementId** mClippingPlacementIds = nullptr;
    bool mIsUpdated = false;
    const PlayerHolder* mPlayerHolder = nullptr;
    const SceneCameraInfo* mSceneCameraInfo = nullptr;
    CheckType mCheckType = CheckType::PlayerPos;
};
<<<<<<< HEAD
} // namespace al
>>>>>>> f1a16e51 (uploaded because need to rebase other pr)
=======
}  // namespace al
>>>>>>> 02efc0b3 (Fixed issues)
