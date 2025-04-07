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
    mFarClipDistance = distance;
    mDefaultFarClipDistance = distance;
}

void ClippingFarAreaObserver::setDefaultFarClipDistanceSub(f32 distance) {
    mFarClipDistanceSub = distance;
    mDefaultFarClipDistanceSub = distance;
}

void ClippingFarAreaObserver::update() {
    if (!mAreaObjGroup)
        return;
    mAreaObj = nullptr;
    s32 playerNumMax = getPlayerNumMax(mPlayerHolder);
    for (s32 i = 0; i < playerNumMax; i++) {
        if (isPlayerDead(mPlayerHolder, i))
            continue;
        AreaObj* areaObj = mAreaObjGroup->getInVolumeAreaObj(getPlayerPos(mPlayerHolder, i));
        if (areaObj && (!mAreaObj || areaObj->getPriority() > mAreaObj->getPriority()))
            mAreaObj = areaObj;
    }
    mFarClipDistance = mDefaultFarClipDistance;
    mFarClipDistanceSub = mDefaultFarClipDistanceSub;
    if (mAreaObj) {
        tryGetAreaObjArg(&mFarClipDistance, mAreaObj, "FarClipDistance");
        tryGetAreaObjArg(&mFarClipDistanceSub, mAreaObj, "FarClipDistanceSub");
    }
}
}  // namespace al
