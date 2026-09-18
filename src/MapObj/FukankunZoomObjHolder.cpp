#include "MapObj/FukankunZoomObjHolder.h"

#include <math/seadVector.h>

#include "Library/LiveActor/LiveActor.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementId.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Scene/SceneObjUtil.h"

#include "MapObj/Fukankun.h"

FukankunZoomObjHolder::FukankunZoomObjHolder() = default;

const al::LiveActor*
FukankunZoomObjHolder::tryGetFukankunZoomObj(const al::PlacementId& placementId) {
    for (s32 i = 0; i < mFukankunZoomObjs.size(); i++) {
        auto* zoomObj = mFukankunZoomObjs[i];
        if (al::isEqualPlacementId(*zoomObj->placementId, placementId))
            return zoomObj->actor;
    }

    return nullptr;
}

void FukankunZoomObjHolder::declareUseFukankunZoomTargetActor() {
    mFukankunZoomTargetActorCount++;
}

void FukankunZoomObjHolder::registerFukankunZoomObj(const al::LiveActor* actor,
                                                    const al::PlacementId* placementId) {
    mFukankunZoomObjs.emplaceBack(FukankunZoomObjInfo{
        .actor = actor,
        .placementId = placementId,
    });
}

void FukankunZoomObjHolder::registerFukankunZoomTargetActor(const al::LiveActor* actor,
                                                            FukankunZoomType zoomType,
                                                            const sead::Vector3f& offset,
                                                            const char* jointName) {
    mFukankunZoomTargetActors.emplaceBack(FukankunZoomTargetActor{
        .actor = actor,
        .zoomType = zoomType,
        .offset = offset,
        .targetJointName = jointName,
    });
}

void FukankunZoomObjHolder::initAfterPlacementSceneObj(const al::ActorInitInfo& info) {
    mFukankunZoomTargetActors.allocBuffer(mFukankunZoomTargetActorCount, nullptr);
}

void FukankunZoomObjHolder::execute() {
    bool anyWatched = false;

    for (s32 i = 0; i < mFukankunZoomTargetActors.size(); i++) {
        auto* zoomTargetActor = mFukankunZoomTargetActors[i];

        bool unwatched = !zoomTargetActor->isWatched;
        s32 count = zoomTargetActor->watchCount + 1;

        if (unwatched)
            count = 0;

        anyWatched = anyWatched || !unwatched;
        zoomTargetActor->watchCount = count;
        zoomTargetActor->isWatched = false;
    }

    if (!anyWatched)
        mActiveFukankun = nullptr;
}

s32 FukankunZoomObjHolder::getFukankunZoomTargetActorNum() const {
    return mFukankunZoomTargetActors.size();
}

const al::LiveActor* FukankunZoomObjHolder::tryGetFukankunZoomTargetActor(s32 index) const {
    return mFukankunZoomTargetActors[index]->actor;
}

bool FukankunZoomObjHolder::tryGetFukankunZoomTargetActorIsNoZoomOn(s32 index) const {
    return mFukankunZoomTargetActors[index]->zoomType == FukankunZoomType_NoZoomOn;
}

FukankunZoomType FukankunZoomObjHolder::tryGetFukankunZoomTargetActorZoomType(s32 index) const {
    return mFukankunZoomTargetActors[index]->zoomType;
}

const sead::Vector3f& FukankunZoomObjHolder::tryGetFukankunZoomTargetActorOffset(s32 index) const {
    return mFukankunZoomTargetActors[index]->offset;
}

const char* FukankunZoomObjHolder::tryGetFukankunZoomTargetActorTargetJointName(s32 index) const {
    return mFukankunZoomTargetActors[index]->targetJointName;
}

void FukankunZoomObjHolder::declareWatchFukankunZoomTargetActor(const Fukankun* fukankun,
                                                                s32 index) {
    mFukankunZoomTargetActors[index]->isWatched = true;
    mActiveFukankun = fukankun;
}

s32 FukankunZoomObjHolder::getWatchCount(const al::LiveActor* actor) const {
    for (s32 i = 0; i < mFukankunZoomTargetActors.size(); i++)
        if (mFukankunZoomTargetActors[i]->actor == actor)
            return mFukankunZoomTargetActors[i]->watchCount;

    return -1;
}

bool FukankunZoomObjHolder::tryGetActiveFukankunLinkedShineMtx(
    const sead::Matrix34f** outMatrix) const {
    if (mActiveFukankun && mActiveFukankun->hasLinkedShine()) {
        *outMatrix = mActiveFukankun->getLinkedShineMtx();
        return true;
    }

    return false;
}

namespace FukankunZoomTargetFunction {

void declareUseFukankunZoomTargetActor(const al::LiveActor* actor) {
    al::getSceneObj<FukankunZoomObjHolder>(actor)->declareUseFukankunZoomTargetActor();
}

void registerFukankunZoomTargetActor(const al::LiveActor* actor, FukankunZoomType zoomType,
                                     const sead::Vector3f& offset, const char* jointName) {
    al::getSceneObj<FukankunZoomObjHolder>(actor)->registerFukankunZoomTargetActor(
        actor, zoomType, offset, jointName);
}

s32 getWatchCount(const al::LiveActor* actor) {
    return al::getSceneObj<FukankunZoomObjHolder>(actor)->getWatchCount(actor);
}

s32 getFukankunWatchCountDefault() {
    return 120;
}

f32 getFukankunCameraNearDistThres() {
    return 4000.0f;
}

bool tryGetActiveFukankunLinkedShineMtx(const sead::Matrix34f** outMatrix,
                                        const al::LiveActor* actor) {
    return al::getSceneObj<FukankunZoomObjHolder>(actor)->tryGetActiveFukankunLinkedShineMtx(
        outMatrix);
}

const al::LiveActor* tryGetFukankunZoomObj(const al::IUseSceneObjHolder* user,
                                           const al::PlacementInfo& placementInfo) {
    al::PlacementId placementId;
    al::getPlacementId(&placementId, placementInfo);

    return al::getSceneObj<FukankunZoomObjHolder>(user)->tryGetFukankunZoomObj(placementId);
}

void registerFukankunZoomObj(const al::LiveActor* actor, const al::PlacementInfo& placementInfo) {
    al::PlacementId* placementId = new al::PlacementId();
    al::getPlacementId(placementId, placementInfo);

    return al::getSceneObj<FukankunZoomObjHolder>(actor)->registerFukankunZoomObj(actor,
                                                                                  placementId);
}

s32 getFukankunZoomTargetActorNum(const al::IUseSceneObjHolder* user) {
    return al::getSceneObj<FukankunZoomObjHolder>(user)->getFukankunZoomTargetActorNum();
}

const al::LiveActor* tryGetFukankunZoomTargetActor(const al::IUseSceneObjHolder* user, s32 index) {
    return al::getSceneObj<FukankunZoomObjHolder>(user)->tryGetFukankunZoomTargetActor(index);
}

bool tryGetFukankunZoomTargetActorIsNoZoomOn(const al::IUseSceneObjHolder* user, s32 index) {
    return al::getSceneObj<FukankunZoomObjHolder>(user)->tryGetFukankunZoomTargetActorIsNoZoomOn(
        index);
}

FukankunZoomType tryGetFukankunZoomTargetActorZoomType(const al::IUseSceneObjHolder* user,
                                                       s32 index) {
    return al::getSceneObj<FukankunZoomObjHolder>(user)->tryGetFukankunZoomTargetActorZoomType(
        index);
}

const sead::Vector3f& tryGetFukankunZoomTargetActorOffset(const al::IUseSceneObjHolder* user,
                                                          s32 index) {
    return al::getSceneObj<FukankunZoomObjHolder>(user)->tryGetFukankunZoomTargetActorOffset(index);
}

const char* tryGetFukankunZoomTargetActorTargetJointName(const al::IUseSceneObjHolder* user,
                                                         s32 index) {
    return al::getSceneObj<FukankunZoomObjHolder>(user)
        ->tryGetFukankunZoomTargetActorTargetJointName(index);
}

void declareWatchFukankunZoomTargetActor(const Fukankun* fukankun, s32 index) {
    al::getSceneObj<FukankunZoomObjHolder>(fukankun)->declareWatchFukankunZoomTargetActor(fukankun,
                                                                                          index);
}

}  // namespace FukankunZoomTargetFunction
