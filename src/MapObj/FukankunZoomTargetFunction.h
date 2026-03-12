#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
class PlacementInfo;
class IUseSceneObjHolder;
}  // namespace al

class Fukankun;
class FukankunZoomObj;

namespace FukankunZoomTargetFunction {
void declareUseFukankunZoomTargetActor(const al::LiveActor*);
void registerFukankunZoomTargetActor(const al::LiveActor*, s32, const sead::Vector3f&, const char*);
s32 getWatchCount(const al::LiveActor*);
s32 getFukankunWatchCountDefault();
f32 getFukankunCameraNearDistThres();
Fukankun* tryGetActiveFukankunLinkedShineMtx(const sead::Matrix34f**, const al::LiveActor*);
FukankunZoomObj* tryGetFukankunZoomObj(const al::IUseSceneObjHolder*, const al::PlacementInfo&);
void registerFukankunZoomObj(const al::LiveActor*, const al::PlacementInfo&);
s32 getFukankunZoomTargetActorNum(const al::IUseSceneObjHolder*);
al::LiveActor* tryGetFukankunZoomTargetActor(const al::IUseSceneObjHolder*, s32);
bool tryGetFukankunZoomTargetActorIsNoZoomOn(const al::IUseSceneObjHolder*, s32);
s32 tryGetFukankunZoomTargetActorZoomType(const al::IUseSceneObjHolder*, s32);
const sead::Vector3f& tryGetFukankunZoomTargetActorOffset(const al::IUseSceneObjHolder*, s32);
const char* tryGetFukankunZoomTargetActorTargetJointName(const al::IUseSceneObjHolder*, s32);
void declareWatchFukankunZoomTargetActor(const Fukankun*, s32);
}  // namespace FukankunZoomTargetFunction
