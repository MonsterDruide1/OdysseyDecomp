#pragma once

#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class AreaObjDirector;
class PlayerHolder;
class SceneCameraInfo;
class ViewIdHolder;
class ActorInitInfo;
class LiveActor;

class ClippingDirector : public HioNode, public IUseExecutor {
public:
    ClippingDirector(int, const AreaObjDirector*, const PlayerHolder*, const SceneCameraInfo*);
    ~ClippingDirector();

    void execute() override;

    void endInit(const AreaObjDirector*);
    void setDefaultFarClipDistance(float);
    void setDefaultFarClipDistanceSub(float);
    void getFarClipDistance();
    void registerActor(LiveActor*, const ViewIdHolder*);
    void addToGroupClipping(LiveActor*, const ActorInitInfo&);
    void onGroupClipping(LiveActor*);
    void offGroupClipping(LiveActor*);
    void startCheckViewCtrlByCameraPos();
    void startCheckViewCtrlByLookAtPos();
    void startCheckViewCtrlByPlayerPos();

private:
    void* filler[6];
};
static_assert(sizeof(ClippingDirector) == 0x38);

}  // namespace al
