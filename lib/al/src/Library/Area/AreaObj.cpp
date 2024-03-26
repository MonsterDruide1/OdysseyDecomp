#include "Library/Area/AreaObj.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaShape.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Stage/ListenStageSwitch.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {

AreaObj::AreaObj(const char* name) : mName(name) {}

const char* AreaObj::getName() const {
    return mName;
}

StageSwitchKeeper* AreaObj::getStageSwitchKeeper() const {
    return mStageSwitchKeeper;
}

void AreaObj::initStageSwitchKeeper() {
    mStageSwitchKeeper = new StageSwitchKeeper();
}

void AreaObj::init(const AreaInitInfo& initInfo) {
    mPlacementInfo = new PlacementInfo(initInfo);
    mSceneObjHolder = initInfo.getSceneObjHolder();
    tryGetMatrixTR(&mMatrixTR, *mPlacementInfo);
    tryGetArg(&mPriority, *mPlacementInfo, "Priority");

    const char* modelName = nullptr;
    alPlacementFunction::tryGetModelName(&modelName, *mPlacementInfo);

    AreaShapeFactory areaShapeFactory("エリアシェイプファクトリー");
    AreaShape* (*creatorFunc)() = nullptr;
    areaShapeFactory.getEntryIndex(&creatorFunc, modelName);
    mAreaShape = creatorFunc();

    mAreaShape->setBaseMtxPtr(&mMatrixTR);
    sead::Vector3f scale = {1.0, 1.0, 1.0};
    tryGetScale(&scale, *mPlacementInfo);
    mAreaShape->setScale(scale);

    initStageSwitch(this, initInfo.getStageSwitchDirector(), initInfo);
    if (listenStageSwitchOnOffAppear(
            this, FunctorV0M<AreaObj*, void (AreaObj::*)()>(this, &AreaObj::invalidate),
            FunctorV0M<AreaObj*, void (AreaObj::*)()>(this, &AreaObj::validate)))
        invalidate();

    if (listenStageSwitchOnKill(
            this, FunctorV0M<AreaObj*, void (AreaObj::*)()>(this, &AreaObj::validate)))
        validate();
}

bool AreaObj::isInVolume(const sead::Vector3f& position) const {
    if (!isValid)
        return false;
    return mAreaShape->isInVolume(position);
}

bool AreaObj::isInVolumeOffset(const sead::Vector3f& position, f32 offset) const {
    if (!isValid)
        return false;
    return mAreaShape->isInVolumeOffset(position, offset);
}

SceneObjHolder* AreaObj::getSceneObjHolder() const {
    return mSceneObjHolder;
}

void AreaObj::validate() {
    isValid = true;
}

void AreaObj::invalidate() {
    isValid = false;
}

}  // namespace al
