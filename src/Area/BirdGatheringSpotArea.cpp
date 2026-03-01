#include "Area/BirdGatheringSpotArea.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaShape.h"
#include "Library/Clipping/ClippingJudge.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

BirdGatheringSpotArea::BirdGatheringSpotArea(const char* name) : al::AreaObj(name) {}

void BirdGatheringSpotArea::init(const al::AreaInitInfo& info) {
    al::AreaObj::init(info);
    al::getArg(&mBirdNumMax, *getPlacementInfo(), "BirdNumMax");
    getAreaShape()->calcLocalBoundingBox(&mClippingInfo.boundingBox);
}

void BirdGatheringSpotArea::updateClipping(const al::ClippingJudge* judge,
                                           const sead::Vector3f& pos) {
    const sead::Matrix34f& mtx = getAreaMtx();

    mClippingInfo.isClipped =
        judge->isJudgedToClipFrustumObb(&mtx, mClippingInfo.boundingBox, 300.0f, 1);

    if (!mClippingInfo.isClipped)
        mClippingInfo.sightDistance = (pos - mtx.getTranslation()).length();
}

void BirdGatheringSpotArea::calcRandomGroundTrans(sead::Vector3f* outTrans) const {
    f32 rx = al::getRandom(-1.0f, 1.0f);
    f32 rz = al::getRandom(-1.0f, 1.0f);

    const sead::Vector3f& scale = getAreaShape()->getScale();

    sead::Vector3f offset = {rx * scale.x * 500.0f, 0.0f, rz * scale.z * 500.0f};

    outTrans->setMul(getAreaMtx(), offset);
}

bool BirdGatheringSpotArea::isGreaterPriorityNotClipped(const BirdGatheringSpotArea* other) const {
    return other->getSightDistance() <= getSightDistance();
}

bool BirdGatheringSpotArea::isClipped() const {
    return mClippingInfo.isClipped;
}

f32 BirdGatheringSpotArea::getSightDistance() const {
    return mClippingInfo.sightDistance;
}

BirdGatheringSpotArea::AreaClippingInfo::AreaClippingInfo() = default;
