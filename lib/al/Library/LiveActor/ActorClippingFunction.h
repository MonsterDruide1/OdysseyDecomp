#pragma once

#include <math/seadBoundBox.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
struct ActorInitInfo;
class ClippingDirector;

void initActorClipping(LiveActor* actor, const ActorInitInfo& initInfo);
void initGroupClipping(LiveActor* actor, const ActorInitInfo& initInfo);
f32 getClippingRadius(const LiveActor* actor);
void setClippingInfo(LiveActor* actor, f32, const sead::Vector3f*);
void setClippingObb(LiveActor* actor, const sead::BoundBox3f& boundingBox);
const sead::BoundBox3f& getClippingObb(LiveActor* actor);
const sead::Vector3f& getClippingCenterPos(const LiveActor* actor);
void setClippingNearDistance(LiveActor* actor, f32 near);
void expandClippingRadiusByShadowLength(LiveActor* actor, sead::Vector3f*, f32 radius);
bool tryExpandClippingToGround(LiveActor* actor, sead::Vector3f*, f32);
bool tryExpandClippingByShadowMaskLength(LiveActor* actor, sead::Vector3f*);
bool tryExpandClippingByDepthShadowLength(LiveActor* actor, sead::Vector3f*);
bool tryExpandClippingByExpandObject(LiveActor*, const ActorInitInfo& initInfo);
bool isClipped(const LiveActor* actor);
bool isInvalidClipping(const LiveActor* actor);
void invalidateClipping(LiveActor* actor);
void validateClipping(LiveActor* actor);
void onDrawClipping(LiveActor* actor);
void offDrawClipping(LiveActor* actor);
void onGroupClipping(LiveActor* actor);
void offGroupClipping(LiveActor* actor);
bool isInClippingFrustum(const LiveActor* actor, const sead::Vector3f&, f32, f32, s32);
bool isInClippingFrustum(const ClippingDirector* director, const sead::Vector3f&, f32, f32, s32);
bool isInClippingFrustumAllView(const LiveActor* actor, const sead::Vector3f& pos, f32 radius, f32);
}  // namespace al

namespace alActorFunction {
void invalidateFarClipping(al::LiveActor*);
void validateFarClipping(al::LiveActor*);
f32 getFarClipDistance(const al::LiveActor*);
bool isInvalidFarClipping(const al::LiveActor*);
bool isDrawClipping(const al::LiveActor* actor);
bool checkActiveViewGroupAny(const al::LiveActor* actor);
}  // namespace alActorFunction
