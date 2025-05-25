#include "Util/ActorDimensionKeeper.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjGroup.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/LiveActor/ActorAreaFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/LiveActor.h"

#include "Area/In2DAreaMoveControl.h"
#include "Util/IUseDimension.h"

ActorDimensionKeeper::ActorDimensionKeeper(const al::LiveActor* actor) : mActor(actor) {
    mIn2DAreaMoveControl = new In2DAreaMoveControl();
}

void ActorDimensionKeeper::validate() {
    mIsValid = true;
}

void ActorDimensionKeeper::invalidate() {
    mIsValid = false;

    if (mIs2D) {
        mIs2D = false;
        mIsIn2DArea = false;
        mIsCurrently2D = false;
        mIsCanChange3D = true;
    }
}

void ActorDimensionKeeper::forceChange2DKeep() {
    mIsCurrently2D = true;
}

void ActorDimensionKeeper::forceEndChange2DKeep() {
    mIsCurrently2D = false;
}

void get2DMoveAreas(sead::PtrArray<al::AreaObj>* areas, const al::LiveActor* actor,
                    const sead::Vector3f& trans);

void ActorDimensionKeeper::update() {
    sead::FixedPtrArray<al::AreaObj, 8> areas;
    areas.clear();

    if (!mIsValid) {
        if (mIsCanChange3D)
            mIsCanChange3D = false;
        get2DMoveAreas(&areas, mActor, al::getTrans(mActor));
        mIsIn2DArea = areas.size() > 0;
        return;
    }

    get2DMoveAreas(&areas, mActor, al::getTrans(mActor));
    mIsIn2DArea = areas.size() > 0;
    mIn2DAreaMoveControl->update(areas);

    if (mIsIn2DArea)
        mIsCurrently2D = false;
    if (al::isInAreaObj(mActor, "2DKeepArea"))
        mIsCurrently2D = true;

    mIsCanChange2D = (mIsIn2DArea || mIsCurrently2D) && !mIs2D;
    mIsCanChange3D = !mIsIn2DArea && mIs2D && !mIsCurrently2D;

    if (mIsCanChange2D)
        mIs2D = true;
    if (mIsCanChange3D)
        mIs2D = false;
}

void get2DMoveAreas(sead::PtrArray<al::AreaObj>* areas, const al::LiveActor* actor,
                    const sead::Vector3f& trans) {
    areas->clear();

    al::AreaObjGroup* group = al::tryFindAreaObjGroup(actor, "2DMoveArea");
    if (!group)
        return;

    s32 num = group->getSize();
    s32 maxPriority = -1;
    for (s32 i = 0; i < num; ++i) {
        al::AreaObj* area = group->getAreaObj(i);
        if (!area->isInVolume(trans))
            continue;

        s32 areaPriority = area->getPriority();
        if (maxPriority > areaPriority)
            continue;

        bool isIgnoreArea = false;
        al::tryGetAreaObjArg(&isIgnoreArea, area, "IsIgnoreArea");
        if (isIgnoreArea) {
            maxPriority = areaPriority + 1;
            areas->clear();
            continue;
        }

        if (maxPriority < areaPriority) {
            maxPriority = areaPriority;
            areas->clear();
        }

        areas->pushBack(area);
    }
}
