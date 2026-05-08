#include "Library/Area/AreaObj.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaShape.h"
#include "Library/Area/AreaShapeFactory.h"
#include "Library/Placement/PlacementFunction.h"
#include "Library/Placement/PlacementInfo.h"
#include "Library/Stage/StageSwitchKeeper.h"
#include "Library/Stage/StageSwitchUtil.h"
#include "Library/Thread/FunctorV0M.h"

namespace al {

AreaObj::AreaObj(const char* name) : mName(name) {}

void AreaObj::initStageSwitchKeeper() {
    mStageSwitchKeeper = new StageSwitchKeeper();
}

void AreaObj::init(const AreaInitInfo& info) {
    using AreaObjFunctor = FunctorV0M<AreaObj*, void (AreaObj::*)()>;

    mPlacementInfo = new PlacementInfo(info);
    mSceneObjHolder = info.getSceneObjHolder();
    tryGetMatrixTR(&mAreaTR, *mPlacementInfo);
    tryGetArg(&mPriority, *mPlacementInfo, "Priority");

    const char* modelName = nullptr;
    alPlacementFunction::tryGetModelName(&modelName, *mPlacementInfo);

    AreaShapeFactory areaShapeFactory("エリアシェイプファクトリー");
    AreaShapeCreatorFunction creatorFunc = nullptr;
    areaShapeFactory.getEntryIndex(&creatorFunc, modelName);
    mAreaShape = creatorFunc();

    mAreaShape->setBaseMtxPtr(&mAreaTR);
    sead::Vector3f scale = {1.0f, 1.0f, 1.0f};
    tryGetScale(&scale, *mPlacementInfo);
    mAreaShape->setScale(scale);

    initStageSwitch(this, info.getStageSwitchDirector(), info);
    if (listenStageSwitchOnOffAppear(this, AreaObjFunctor(this, &AreaObj::invalidate),
                                     AreaObjFunctor(this, &AreaObj::validate)))
        invalidate();

    if (listenStageSwitchOnKill(this, AreaObjFunctor(this, &AreaObj::validate)))
        validate();
}

bool AreaObj::isInVolume(const sead::Vector3f& pos) const {
    if (!mIsValid)
        return false;
    return mAreaShape->isInVolume(pos);
}

bool AreaObj::isInVolumeOffset(const sead::Vector3f& pos, f32 offset) const {
    if (!mIsValid)
        return false;
    return mAreaShape->isInVolumeOffset(pos, offset);
}

}  // namespace al
