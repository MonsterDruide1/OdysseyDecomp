#pragma once

namespace al {
class SceneCameraInfo;
class PlayerHolder;
class AreaObjGroup;
class ClippingActorInfo;

class ViewInfoCtrl {
public:
    ViewInfoCtrl(const PlayerHolder*, const SceneCameraInfo*);

    void initActorInfo(ClippingActorInfo*);
    void initViewCtrlAreaGroup(AreaObjGroup*);
    bool update();
    void startCheckByCameraPos();
    void startCheckByLookAtPos();
    void startCheckByPlayerPos();

private:
    void* filler[0x7];
};

}  // namespace al
