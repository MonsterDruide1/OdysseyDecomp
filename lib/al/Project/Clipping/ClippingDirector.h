#pragma once

#include <basis/seadTypes.h>

#include "Library/Clipping/ClippingGroupHolder.h"
#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class ViewInfoCtrl;
class AreaObjDirector;
class PlayerHolder;
class SceneCameraInfo;
class ViewIdHolder;
struct ActorInitInfo;
class LiveActor;
class ClippingActorHolder;
class ClippingFarAreaObserver;

class ClippingDirector : public HioNode, public IUseExecutor {
public:
    ClippingDirector(s32, const AreaObjDirector* areaObjDirector, const PlayerHolder* playerHolder,
                     const SceneCameraInfo* sceneCameraInfo);
    ~ClippingDirector();

    void execute() override;

    void endInit(const AreaObjDirector* areaObjDirector);
    void setDefaultFarClipDistance(f32 distance);
    void setDefaultFarClipDistanceSub(f32 distance);
    f32 getFarClipDistance() const;
    void registerActor(LiveActor* liveActor, const ViewIdHolder* idHolder);
    void addToGroupClipping(LiveActor* liveActor, const ActorInitInfo& actorInitInfo);
    void onGroupClipping(LiveActor* liveActor);
    void offGroupClipping(LiveActor* liveActor);
    void startCheckViewCtrlByCameraPos();
    void startCheckViewCtrlByLookAtPos();
    void startCheckViewCtrlByPlayerPos();

private:
    ClippingJudge* mClippingJudge = nullptr;
    ClippingFarAreaObserver* mFarAreaObserver = nullptr;
    ClippingActorHolder* mClippingActorHolder = nullptr;
    ClippingGroupHolder* mGroupHolder = nullptr;
    ViewInfoCtrl* mViewInfoCtrl = nullptr;
    bool mIsExecute = true;
};

}  // namespace al
