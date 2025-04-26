#pragma once

#include <basis/seadTypes.h>
#include <math/seadBoundBox.h>
#include <math/seadVectorFwd.h>

namespace sead {
class Thread;
}

namespace al {
class ClippingActorInfo;
class ClippingGroupHolder;
struct ActorInitInfo;
class LiveActor;
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
    f32 getClippingRadius(const LiveActor*);
    void find(LiveActor*) const;
    sead::Vector3f& getClippingCenterPos(const LiveActor*);
    void setTypeToSphere(LiveActor*, f32, const sead::Vector3f*);
    void setTypeToObb(LiveActor*, const sead::BoundBox3f&);
    sead::BoundBox3f& getClippingObb(const LiveActor*);
    void setNearClipDistance(LiveActor*, f32);
    void setFarClipLevel20M(LiveActor*);
    void setFarClipLevelMax(LiveActor*);
    void updateFarClipLevel();
};

}  // namespace al
