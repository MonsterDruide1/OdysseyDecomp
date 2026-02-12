#pragma once

#include <basis/seadTypes.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}

namespace rs {
void startActionDemoPlayer(const al::LiveActor*, const char*);
bool isActionEndDemoPlayer(const al::LiveActor*);
void setActionFrameRateDemoPlayer(const al::LiveActor*, f32);
void setActionFrameDemoPlayer(const al::LiveActor*, f32);
void clearDemoAnimInterpolatePlayer(const al::LiveActor*);
void hideDemoPlayer(const al::LiveActor*);
void showDemoPlayer(const al::LiveActor*);
void hideDemoPlayerSilhouette(const al::LiveActor*);
void showDemoPlayerSilhouette(const al::LiveActor*);
void addDemoLockOnCap(const al::LiveActor*);
void forcePutOnDemoCap(const al::LiveActor*);
void forcePutOffMarioHeadCap(const al::LiveActor*);
void hideDemoCap(const al::LiveActor*);
void showDemoCap(const al::LiveActor*);
void hideDemoCapSilhouette(const al::LiveActor*);
void showDemoCapSilhouette(const al::LiveActor*);
void killAllEffectPlayerAndCap(const al::LiveActor*);
const sead::Vector3f& getDemoPlayerTrans(const al::LiveActor*);
void setDemoPlayerQuat(const al::LiveActor*, const sead::Quatf&);
const sead::Quatf& getDemoPlayerQuat(const al::LiveActor*);
void replaceDemoPlayer(const al::LiveActor*, const sead::Vector3f&, const sead::Quatf&);
void validateIK(const al::LiveActor*);
void invalidateIK(const al::LiveActor*);
void validateWatchTarget(const al::LiveActor*, const sead::Vector3f&);
void invalidateWatchTarget(const al::LiveActor*);
void hideDemoPlayerAndStartDemoResetAction(const al::LiveActor*);
void calcDemoMarioJointPosAllRoot(sead::Vector3f*, const al::LiveActor*);
void invalidateMarioDitherFrame(const al::LiveActor*, s32);
void startMarioCapEyeAction(const al::LiveActor*, const char*);
void killMarioCapEye(const al::LiveActor*);
void startMarioRightHandAction(const al::LiveActor*, const char*);
f32 getMarioActionFrameMax(const al::LiveActor*);
void hideMarioGroundDepthShadow(const al::LiveActor*);
void showMarioGroundDepthShadow(const al::LiveActor*);
void setMarioGroundDepthShadowMapLength(const al::LiveActor*, f32);
void changeMarioDepthShadowMapSizeHight(const al::LiveActor*);
void changeMarioDepthShadowMapSizeNormal(const al::LiveActor*);
void setMarioDirectionalShadowMaskTypeNone(const al::LiveActor*);
void setMarioDirectionalShadowMaskTypeSelf(const al::LiveActor*);
void resetMarioDynamics(const al::LiveActor*);
void keepMarioCapVisibilityEndDemo(const al::LiveActor*);
void clearMarioFootPrint(const al::LiveActor*);
}  // namespace rs

namespace PlayerDemoFunction {
void startCapCheckpointWarpMode(const al::LiveActor*);
void endMarioShadowMainShine(const al::LiveActor*);
void prepareSphinxQuizRouteKill(const al::LiveActor*);
void clearMarioStain(al::LiveActor*);
}  // namespace PlayerDemoFunction
