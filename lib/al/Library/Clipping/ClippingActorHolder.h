#pragma once

#include <basis/seadTypes.h>
#include <math/seadBoundBox.h>
#include <thread/seadThread.h>

namespace al {
struct ActorInitInfo;
class ClippingActorInfo;
class LiveActor;
class ClippingGroupHolder;
class ClippingJudge;

class ClippingActorHolder {
public:

    ClippingActorHolder(s32);
    void updateAsync(sead::Thread*, s64);
    ~ClippingActorHolder();
    void update(const ClippingJudge*);
    ClippingActorInfo* registerActor(LiveActor*);
    ClippingActorInfo* initGroupClipping(LiveActor*, const ActorInitInfo&);
    void endInit(ClippingGroupHolder*);
    void validateClipping(LiveActor*);
    void invalidateClipping(LiveActor*);
    void addToClippingTarget(LiveActor*);
    void removeFromClippingTarget(LiveActor*);
    void onGroupClipping(LiveActor*);
    void offGroupClipping(LiveActor*);
    ClippingActorInfo* getClippingRadius(const LiveActor*);
    ClippingActorInfo* find(const LiveActor*) const;
    ClippingActorInfo* getClippingCenterPos(const LiveActor*);
    void setTypeToSphere(LiveActor*, f32, const sead::Vector3f*);
    void setTypeToObb(LiveActor* liveActor, const sead::BoundBox3f&);
    LiveActor* getClippingObb(const LiveActor*);
    void setNearClipDistance(LiveActor*, f32);
    void setFarClipLevel20M(LiveActor*);
    void setFarClipLevelMax(LiveActor*);
    void updateFarClipLevel();
private:
    void* filler[0x8];
};

}  // namespace al
