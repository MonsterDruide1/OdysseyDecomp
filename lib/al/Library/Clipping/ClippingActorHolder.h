#pragma once

#include <basis/seadTypes.h>
#include <math/seadBoundBox.h>
#include <math/seadVector.h>

namespace sead {
class Thread;
}

namespace al {
struct ActorInitInfo;
class ClippingActorInfo;
class ClippingGroupHolder;
class ClippingJudge;
class LiveActor;

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
    f32 getClippingRadius(const LiveActor*);
    ClippingActorInfo* find(const LiveActor*) const;
    const sead::Vector3f& getClippingCenterPos(const LiveActor*);
    void setTypeToSphere(LiveActor*, f32, const sead::Vector3f*);
    void setTypeToObb(LiveActor* liveActor, const sead::BoundBox3f&);
    const sead::BoundBox3f& getClippingObb(const LiveActor*);
    void setNearClipDistance(LiveActor*, f32);
    void setFarClipLevel20M(LiveActor*);
    void setFarClipLevelMax(LiveActor*);
    void updateFarClipLevel();

private:
    void* filler[0x8];
};

}  // namespace al
