#pragma once

#include <basis/seadTypes.h>

#include "Library/Clipping/ClippingActorHolder.h"
#include "Library/Execute/IUseExecutor.h"
#include "Library/HostIO/HioNode.h"

namespace al {
class ViewInfoCtrl;
class ClippingFarAreaObserver;
class AreaObjDirector;
class PlayerHolder;
class SceneCameraInfo;
class ViewIdHolder;
struct ActorInitInfo;
class LiveActor;

class ClippingDirector : public HioNode, public IUseExecutor {
public:
    ClippingDirector(s32, const AreaObjDirector*, const PlayerHolder*, const SceneCameraInfo*);
    ~ClippingDirector();

    void execute() override;

    void endInit(const AreaObjDirector*);
    void setDefaultFarClipDistance(f32);
    void setDefaultFarClipDistanceSub(f32);
    void getFarClipDistance();
    void registerActor(LiveActor*, const ViewIdHolder*);
    void addToGroupClipping(LiveActor*, const ActorInitInfo&);
    void onGroupClipping(LiveActor*);
    void offGroupClipping(LiveActor*);
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
