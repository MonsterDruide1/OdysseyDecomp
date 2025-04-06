#include "Project/Clipping/ClippingFarAreaObserver.h"

#include "Library/Area/AreaObj.h"
#include "Library/Area/AreaObjDirector.h"
#include "Library/Area/AreaObjUtil.h"
#include "Library/Player/PlayerUtil.h"

namespace al {

ClippingFarAreaObserver::ClippingFarAreaObserver(const AreaObjDirector* areaObjDirector,
                                                 const PlayerHolder* playerHolder)
    : mAreaObjDirector(areaObjDirector), mPlayerHolder(playerHolder) {}

void ClippingFarAreaObserver::endInit() {
    mAreaObjGroup = mAreaObjDirector->getAreaObjGroup("ClippingFarArea");
}

void ClippingFarAreaObserver::setDefaultFarClipDistance(f32 distance) {
    mDefaultFarClipDistanceHorizontal = distance;
    mDefaultFarClipDistanceVertical = distance;
}

void ClippingFarAreaObserver::setDefaultFarClipDistanceSub(f32 distance) {
    mDefaultFarClipDistanceHorizontalSub = distance;
    mDefaultFarClipDistanceVerticalSub = distance;
}

void ClippingFarAreaObserver::update() {
    if (!mAreaObjGroup)
        return;
    mAreaObj = nullptr;
    const s32 playerNumMax = getPlayerNumMax(mPlayerHolder);
    for (s32 i = 0; i < playerNumMax; ++i) {
        if (isPlayerDead(mPlayerHolder, i))
            continue;
        const AreaObjGroup* group = mAreaObjGroup;
        const sead::Vector3f& playerPos = getPlayerPos(mPlayerHolder, i);
        AreaObj* areaObj = group->getInVolumeAreaObj(playerPos);
        if (areaObj && (!mAreaObj || areaObj->getPriority() > mAreaObj->getPriority()))
            mAreaObj = areaObj;
    }
    mDefaultFarClipDistanceHorizontal = mDefaultFarClipDistanceVertical;
    mDefaultFarClipDistanceHorizontalSub = mDefaultFarClipDistanceVerticalSub;
    if (mAreaObj) {
        tryGetAreaObjArg(&mDefaultFarClipDistanceHorizontal, mAreaObj, "FarClipDistance");
        tryGetAreaObjArg(&mDefaultFarClipDistanceHorizontalSub, mAreaObj, "FarClipDistanceSub");
    }
}
}  // namespace al
