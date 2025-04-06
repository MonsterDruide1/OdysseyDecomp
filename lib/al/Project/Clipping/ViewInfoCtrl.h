#pragma once

namespace al {
class SceneCameraInfo;
class PlayerHolder;
class AreaObjGroup;
class ClippingActorInfo;

class ViewInfoCtrl {
public:
    void initActorInfo(ClippingActorInfo*);
    void initViewCtrlAreaGroup(AreaObjGroup*);
    void startCheckByCameraPos();
    void startCheckByLookAtPos();
    void startCheckByPlayerPos();
    bool update();
    ViewInfoCtrl(const PlayerHolder*, const SceneCameraInfo*);

private:
    void* filler[0x7];
};

}  // namespace al
