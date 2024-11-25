#pragma once

#include <math/seadVector.h>

namespace al {
class LiveActor;
class AreaObj;
class IUseAreaObj;
}  // namespace al

namespace rs {
al::AreaObj* tryFind2DAreaObj(const al::IUseAreaObj* user, const sead::Vector3f& trans);
al::AreaObj* tryFind2DAreaObj(const al::LiveActor* actor, sead::Vector3f*, sead::Vector3f*);
void calc2DAreaLockDir(sead::Vector3f* lockDir, const al::AreaObj* area,
                       const sead::Vector3f& trans);
void get2DAreaPos(sead::Vector3f* pos, const al::AreaObj* area);
bool isIn2DArea(const al::LiveActor* actor, sead::Vector3f*, sead::Vector3f*);
bool isInChangeStage(const al::LiveActor* actor, const sead::Vector3f* trans);
bool isInWarpArea(const al::LiveActor* actor);
bool isInLowGravityArea(const al::LiveActor* actor);
bool isInItemVanishArea(const al::LiveActor* actor);
bool tryFindStageMapAreaAndGetPlayerPoint(sead::Vector3f* playerPoint, const al::IUseAreaObj* user,
                                          const sead::Vector3f& trans);
bool calcFindWaterAreaSurfaceNoWaveByArrow(const al::LiveActor* actor, sead::Vector3f*,
                                           sead::Vector3f*, const sead::Vector3f&,
                                           const sead::Vector3f&);
bool isPushOut2DAreaOpposite(const al::AreaObj* area);
bool isFaceTo2DAreaPushOutDir(const al::AreaObj* area);
void calc2DAreaFreeDir(sead::Vector3f* freeDir, const al::AreaObj* area,
                       const sead::Vector3f& trans);
bool calc2DAreaGravity(sead::Vector3f* gravity, const al::AreaObj* area,
                       const sead::Vector3f& trans);
bool calc2DAreaSnapPower(sead::Vector3f*, f32* snapPower, const al::AreaObj* area,
                         const sead::Vector3f& trans);
bool calc2DAreaDistanceGravity(sead::Vector3f*, const al::AreaObj* area,
                               const sead::Vector3f& trans);
f32 get2DAreaSurfaceDistance(const al::AreaObj* area);
bool calc2DAreaSnapPowerSurfaceDistance(sead::Vector3f*, f32* snapPower, const al::AreaObj* area,
                                        const sead::Vector3f& trans, f32 surfaceDistance);
bool calcExtForceAreaVelocity(sead::Vector3f*, const al::IUseAreaObj* user,
                              const sead::Vector3f& trans, const sead::Vector3f&,
                              const sead::Vector3f& velocity);
bool tryFindRecoveryTargetPosArea(bool* isNoCollideGround, sead::Vector3f* targetPos,
                                  sead::Vector3f* targetRotate, const al::AreaObj** area,
                                  const al::IUseAreaObj* user, const sead::Vector3f& trans);
bool tryFindForceRecoveryArea(sead::Vector3f* targetPos, sead::Vector3f* targetRotate,
                              const al::AreaObj** area, const al::IUseAreaObj* user,
                              const sead::Vector3f& trans);
}  // namespace rs
