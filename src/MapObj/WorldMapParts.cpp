#include "MapObj/WorldMapParts.h"

#include "Library/LiveActor/ActorInitInfo.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/Math/MathUtil.h"

void localFunc_34B9A8(WorldMapParts* mapParts) {
    al::invalidateOcclusionQuery(mapParts);
    //    if(al::isExistSubActorKeeper(mapParts) && al::getSubActorNum(mapParts) >= 1) {
    //
    //    }
}

WorldMapParts::WorldMapParts(const char* name) : al::LiveActor(name) {}

void WorldMapParts::setWorldMtx(const sead::Matrix34f& srcMtx) {
    sead::Matrix34f copyMtx;
    sead::Matrix34f inverse;

    inverse.setInverse(*mWorldMtx);
    copyMtx.setMul(inverse, srcMtx);
    setLocalMtx(copyMtx);
}

void WorldMapParts::updatePose() {
    sead::Matrix34f prevWorldMtx = *mWorldMtx;
    al::normalize(&prevWorldMtx);

    prevWorldMtx.setMul(prevWorldMtx, mLocalMtx);

    al::updatePoseMtx(this, &prevWorldMtx);
}

void WorldMapParts::control() {
    updatePose();
}

void WorldMapParts::setLocalMtx(const sead::Matrix34f& srcMtx) {
    mLocalMtx = srcMtx;
}

void WorldMapParts::initParts(WorldMapParts* mapParts, const char* arcName,
                              const al::ActorInitInfo& initInfo, const sead::Matrix34f* worldMtx,
                              const sead::Matrix34f& localMtx, const char* suffix) {
    al::initChildActorWithArchiveNameNoPlacementInfo(mapParts, initInfo, arcName, suffix);
    mapParts->mWorldMtx = worldMtx;
    mapParts->setWorldMtx(localMtx);
    mapParts->updatePose();

    if (al::isExistModel(mapParts))
        localFunc_34B9A8(mapParts);

    mapParts->makeActorDead();
}

WorldMapParts* WorldMapParts::create(const char* name, const char* arcName,
                                     const al::ActorInitInfo& initInfo,
                                     const sead::Matrix34f* worldMtx,
                                     const sead::Matrix34f& localMtx, const char* suffix) {
    WorldMapParts* newParts = new WorldMapParts(name);

    initParts(newParts, arcName, initInfo, worldMtx, localMtx, suffix);

    return newParts;
}
