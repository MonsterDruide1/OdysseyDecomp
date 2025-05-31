#pragma once

#include <basis/seadTypes.h>

#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class ClippingGroupHolder;
class ClippingJudge;
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
    ClippingDirector(s32 maxActors, const AreaObjDirector* areaObjDirector,
                     const PlayerHolder* playerHolder, const SceneCameraInfo* sceneCameraInfo);
    // BUG: this destructor should have been virtual.
    // This means that `delete clippingDirector::IUseExecutor` (or with HioNode) will cause the
    // destructor of ClippingDirector to not be called, causing a memory leak.
    ~ClippingDirector();

    void endInit(const AreaObjDirector* areaObjDirector);
    void setDefaultFarClipDistance(f32 distance);
    void setDefaultFarClipDistanceSub(f32 distance);
    f32 getFarClipDistance() const;
    void registerActor(LiveActor* liveActor, const ViewIdHolder* idHolder);
    void addToGroupClipping(LiveActor* liveActor, const ActorInitInfo& actorInitInfo);
    void onGroupClipping(LiveActor* liveActor);
    void offGroupClipping(LiveActor* liveActor);
    void execute() override;
    void startCheckViewCtrlByCameraPos();
    void startCheckViewCtrlByLookAtPos();
    void startCheckViewCtrlByPlayerPos();

    ClippingActorHolder* getClippingActorHolder() const { return mClippingActorHolder; }

private:
    ClippingJudge* mClippingJudge = nullptr;
    ClippingFarAreaObserver* mFarAreaObserver = nullptr;
    ClippingActorHolder* mClippingActorHolder = nullptr;
    ClippingGroupHolder* mGroupHolder = nullptr;
    ViewInfoCtrl* mViewInfoCtrl = nullptr;
    bool mIsExecute = true;
};

static_assert(sizeof(ClippingDirector) == 0x38);
}  // namespace al
