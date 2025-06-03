#pragma once

namespace al {
class AreaObjGroup;
struct ClippingActorInfo;
class PlayerHolder;
class SceneCameraInfo;

class ViewInfoCtrl {
public:
    ViewInfoCtrl(const PlayerHolder*, const SceneCameraInfo*);

    void initActorInfo(ClippingActorInfo*);
    void initViewCtrlAreaGroup(const AreaObjGroup*);
    bool update();
    void startCheckByCameraPos();
    void startCheckByLookAtPos();
    void startCheckByPlayerPos();

private:
    void* filler[0x7];
};

}  // namespace al
