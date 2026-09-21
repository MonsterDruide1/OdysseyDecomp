#pragma once

#include <container/seadObjArray.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "Library/HostIO/HioNode.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Scene/ISceneObj.h"

#include "Scene/SceneObjFactory.h"

namespace al {
class LiveActor;
class PlacementInfo;
class IUseSceneObjHolder;
}  // namespace al

class Fukankun;
class FukankunZoomObj;

struct FukankunZoomObjInfo {
    const al::LiveActor* actor;
    const al::PlacementId* placementId;
};

static_assert(sizeof(FukankunZoomObjInfo) == 0x10);

enum class FukankunZoomType : s32 {
    ZoomOn = 0,
    NoZoomOn = 1,
    CameraNear = 2,
};

struct FukankunZoomTargetActor {
    const al::LiveActor* actor;

    s32 watchCount = 0;
    bool isWatched = false;
    FukankunZoomType zoomType;

    const sead::Vector3f offset;
    const char* targetJointName;
};

static_assert(sizeof(FukankunZoomTargetActor) == 0x28);

class FukankunZoomObjHolder : public al::HioNode, public al::ISceneObj {
public:
    static constexpr s32 sSceneObjId = SceneObjID_FukankunZoomObjHolder;

    FukankunZoomObjHolder();

    const char* getSceneObjName() const override { return "フカンくんズームオブジェ管理"; };

    void initAfterPlacementSceneObj(const al::ActorInitInfo& info) override;
    virtual void execute();

    const al::LiveActor* tryGetFukankunZoomObj(const al::PlacementId& placementId);

    void declareUseFukankunZoomTargetActor();

    void registerFukankunZoomObj(const al::LiveActor* actor, const al::PlacementId* placementId);
    void registerFukankunZoomTargetActor(const al::LiveActor* actor, s32 zoomType,
                                         const sead::Vector3f& offset, const char* jointName);

    s32 getFukankunZoomTargetActorNum() const;
    const al::LiveActor* tryGetFukankunZoomTargetActor(s32 index) const;
    bool tryGetFukankunZoomTargetActorIsNoZoomOn(s32 index) const;
    FukankunZoomType tryGetFukankunZoomTargetActorZoomType(s32 index) const;
    const sead::Vector3f& tryGetFukankunZoomTargetActorOffset(s32 index) const;
    const char* tryGetFukankunZoomTargetActorTargetJointName(s32 index) const;

    void declareWatchFukankunZoomTargetActor(const Fukankun* fukankun, s32 index);
    s32 getWatchCount(const al::LiveActor* actor) const;
    bool tryGetActiveFukankunLinkedShineMtx(const sead::Matrix34f** outMatrix) const;

private:
    sead::FixedObjArray<FukankunZoomObjInfo, 64> mFukankunZoomObjs;
    sead::ObjArray<FukankunZoomTargetActor> mFukankunZoomTargetActors;
    s32 mFukankunZoomTargetActorCount = 0;

    const Fukankun* mActiveFukankun = nullptr;
};

static_assert(sizeof(FukankunZoomObjHolder) == 0x658);

namespace FukankunZoomTargetFunction {

void declareUseFukankunZoomTargetActor(const al::LiveActor* actor);
void registerFukankunZoomTargetActor(const al::LiveActor* actor, s32 zoomType,
                                     const sead::Vector3f& offset, const char* jointName);

s32 getWatchCount(const al::LiveActor* actor);
s32 getFukankunWatchCountDefault();
f32 getFukankunCameraNearDistThres();

bool tryGetActiveFukankunLinkedShineMtx(const sead::Matrix34f** outMatrix,
                                        const al::LiveActor* actor);
const al::LiveActor* tryGetFukankunZoomObj(const al::IUseSceneObjHolder* user,
                                           const al::PlacementInfo& placementInfo);
void registerFukankunZoomObj(const al::LiveActor*, const al::PlacementInfo& placementInfo);

s32 getFukankunZoomTargetActorNum(const al::IUseSceneObjHolder*);
const al::LiveActor* tryGetFukankunZoomTargetActor(const al::IUseSceneObjHolder* user, s32 index);
bool tryGetFukankunZoomTargetActorIsNoZoomOn(const al::IUseSceneObjHolder* user, s32 index);
FukankunZoomType tryGetFukankunZoomTargetActorZoomType(const al::IUseSceneObjHolder* user,
                                                       s32 index);
const sead::Vector3f& tryGetFukankunZoomTargetActorOffset(const al::IUseSceneObjHolder* user,
                                                          s32 index);
const char* tryGetFukankunZoomTargetActorTargetJointName(const al::IUseSceneObjHolder* user,
                                                         s32 index);

void declareWatchFukankunZoomTargetActor(const Fukankun* fukankun, s32 index);

}  // namespace FukankunZoomTargetFunction
