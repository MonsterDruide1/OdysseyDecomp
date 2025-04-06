#pragma once

#include "basis/seadTypes.h"
#include "math/seadBoundBox.h"
#include "thread/seadThread.h"

namespace al {
struct ActorInitInfo;
class ClippingActorInfo;
class LiveActor;
class ClippingGroupHolder;
class ClippingJudge;

class ClippingActorHolder {
public:
    void addToClippingTarget(LiveActor* liveActor);
    ClippingActorHolder(int);
    void endInit(ClippingGroupHolder*);
    ClippingActorInfo* find(const LiveActor* liveActor) const;
    ClippingActorInfo* getClippingCenterPos(const LiveActor* liveActor);
    LiveActor* getClippingObb(const LiveActor* liveActor);
    ClippingActorInfo* getClippingRadius(const LiveActor* liveActor);
    ClippingActorInfo* initGroupClipping(LiveActor* liveActor, const ActorInitInfo&);
    void invalidateClipping(LiveActor* liveActor);
    void offGroupClipping(LiveActor* liveActor);
    void onGroupClipping(LiveActor* liveActor);
    ClippingActorInfo* registerActor(LiveActor* liveActor);
    void removeFromClippingTarget(LiveActor* liveActor);
    void setFarClipLevel20M(LiveActor* liveActor);
    void setFarClipLevelMax(LiveActor* liveActor);
    void setNearClipDistance(LiveActor* liveActor, f32);
    void setTypeToObb(LiveActor* liveActor, const sead::BoundBox3f&);
    void setTypeToSphere(LiveActor* liveActor, const sead::BoundBox3f*);
    void update(const ClippingJudge*);
    void updateAsync(sead::Thread*, long);
    void updateFarClipLevel();
    void validateClipping(LiveActor* liveActor);
    ~ClippingActorHolder();
};

} // namespace al
