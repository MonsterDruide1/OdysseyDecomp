#pragma once

#include <basis/seadTypes.h>
#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

namespace al {
class AreaObj;
class FireSurfaceFinder;
class IUseFluidSurface;
class LiveActor;
class NatureDirector;
class WaterSurfaceFinder;

void registerFluidSurfaceObj(IUseFluidSurface*, const LiveActor*);
bool isExistFluidSurface(const LiveActor*);
void setWaterRippleFieldScale(const LiveActor*, f32);
bool isInWaterPos(const LiveActor*, const sead::Vector3f&);
bool isInWater(const LiveActor*);
bool isInWaterNoIgnore(const LiveActor*, const sead::Vector3f&);
bool isInSodaWater(const LiveActor*, const sead::Vector3f&);
bool isInIceWaterPos(const LiveActor*, const sead::Vector3f&);
bool isInIceWater(const LiveActor*);
bool isInFirePos(const LiveActor*, const sead::Vector3f&);
bool isInFire(const LiveActor*);
bool isInCloudPos(const LiveActor*, const sead::Vector3f&);
bool isInCloud(const LiveActor*);
bool tryAddRippleMiddle(const LiveActor*);
bool tryAddRippleLarge(const LiveActor*);
bool isWaterAreaIgnore(const AreaObj*);
const char* getFireMaterialCode(const LiveActor*);

bool calcFindWaterSurface(sead::Vector3f*, sead::Vector3f*, const LiveActor*, const sead::Vector3f&,
                          const sead::Vector3f&, f32);
bool calcFindWaterSurfaceFlat(sead::Vector3f*, sead::Vector3f*, const LiveActor*,
                              const sead::Vector3f&, const sead::Vector3f&, f32);
bool calcFindWaterSurfaceDisplacement(sead::Vector3f*, sead::Vector3f*, const LiveActor*,
                                      const sead::Vector3f&, const sead::Vector3f&, f32);
bool calcFindWaterSurfaceOverGround(sead::Vector3f*, sead::Vector3f*, const LiveActor*,
                                    const sead::Vector3f&, const sead::Vector3f&, f32);
bool calcFindFireSurface(sead::Vector3f*, sead::Vector3f*, const LiveActor*, const sead::Vector3f&,
                         const sead::Vector3f&, f32);
bool calcFindCloudSurface(sead::Vector3f*, sead::Vector3f*, const LiveActor*, const sead::Vector3f&,
                          const sead::Vector3f&, f32);

bool tryAddRipple(const NatureDirector*, const sead::Vector3f&, f32, f32);
bool tryAddRippleTiny(const LiveActor*);
bool tryAddRippleSmall(const LiveActor*, const sead::Vector3f&);
bool tryAddRippleTiny(const LiveActor*, const sead::Vector3f&);
bool tryAddRippleSmall(const LiveActor*);
bool tryAddRippleMiddle(const LiveActor*);
bool tryAddRippleMiddle(const LiveActor*, const sead::Vector3f&);
bool tryAddRippleLarge(const LiveActor*);
bool tryAddRippleLarge(const LiveActor*, const sead::Vector3f&);
bool tryAddRippleWithRange(const LiveActor*, const sead::Vector3f&, f32, f32, f32, f32);
bool tryAddRipple(const LiveActor*, const sead::Vector3f&, f32, f32);
bool tryAddRippleRandomBlur(const LiveActor*, const sead::Vector3f&, f32, f32, f32);
bool tryAddQuadRipple(const LiveActor*, const sead::Vector3f&, const sead::Vector3f&,
                      const sead::Vector3f&, const sead::Vector3f&, f32);
bool tryAddQuadRipple(const LiveActor*, const sead::BoundBox3f&, const sead::Vector3f&,
                      const sead::Quatf&, f32, f32);
bool tryAddQuadRippleByBoxRotateY(const LiveActor*, const sead::BoundBox3f&, const sead::Vector3f&,
                                  f32, f32, f32);

void approachWaterSurfaceSpringDumper(LiveActor*, const WaterSurfaceFinder*, f32, f32, f32, f32,
                                      f32);
void approachFireSurfaceSpringDumper(LiveActor*, const FireSurfaceFinder*, f32, f32, f32, f32, f32);
void approachWaterSurfaceRate(LiveActor*, const WaterSurfaceFinder*, f32, f32, f32);
void approachFireSurfaceRate(LiveActor*, const FireSurfaceFinder*, f32, f32, f32);

void keepWaterSurfaceHeight(LiveActor*, const WaterSurfaceFinder*, f32);
void syncWaterSurfaceTrans(LiveActor*, const WaterSurfaceFinder*);
void syncFireSurfaceTrans(LiveActor*, const FireSurfaceFinder*);
void syncWaterSurfaceTransH(LiveActor*, const WaterSurfaceFinder*);
void blendWaterSurfaceTransH(LiveActor*, const WaterSurfaceFinder*, f32);
void syncWaterSurfaceUp(LiveActor*, const WaterSurfaceFinder*, f32);
void syncFireSurfaceUp(LiveActor*, const FireSurfaceFinder*, f32);
void calcMatrixFromActorPoseAndWaterSurfaceH(sead::Matrix34f*, const WaterSurfaceFinder*,
                                             const LiveActor*);

bool calcFindFireSurface(sead::Vector3f*, sead::Vector3f*, const LiveActor*, const sead::Vector3f&,
                         const sead::Vector3f&, f32);
}  // namespace al

namespace alNatureUtil {

bool calcFindDistanceWaterSurfaceToGround(f32*, const al::LiveActor*, const sead::Vector3f&, f32);

}  // namespace alNatureUtil
