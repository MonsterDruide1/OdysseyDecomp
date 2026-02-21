#include "Area/BirdGatheringSpotArea.h"

#include "Library/Area/AreaInitInfo.h"
#include "Library/Area/AreaShape.h"
#include "Library/Clipping/ClippingJudge.h"
#include "Library/Math/MathUtil.h"
#include "Library/Placement/PlacementFunction.h"

extern "C" sead::Vector3f gDefaultBoundingBoxMinMax;

BirdGatheringSpotArea::BirdGatheringSpotArea(const char* name)
    : al::AreaObj(name), mBirdNumMax(0), mClippingInfo() {}

void BirdGatheringSpotArea::init(const al::AreaInitInfo& info) {
    al::AreaObj::init(info);
    al::getArg(&mBirdNumMax, *getPlacementInfo(), "BirdNumMax");
    getAreaShape()->calcLocalBoundingBox(&mClippingInfo.boundingBox);
}

BirdGatheringSpotArea::AreaClippingInfo::AreaClippingInfo() {
    const u32* def = reinterpret_cast<const u32*>(&gDefaultBoundingBoxMinMax);
    u32* box = reinterpret_cast<u32*>(&boundingBox);

    isClipped = false;
    sightDistance = 0.0f;

    box[0] = def[0];
    box[1] = def[1];
    box[2] = def[2];
    box[3] = def[0];
    box[4] = def[1];
    box[5] = def[2];
}

void BirdGatheringSpotArea::updateClipping(const al::ClippingJudge* judge,
                                           const sead::Vector3f& pos) {
    const sead::Matrix34f& mtx = getAreaMtx();

    bool isClippedLocal =
        judge->isJudgedToClipFrustumObb(&mtx, mClippingInfo.boundingBox, 300.0f, 1);
    mClippingInfo.isClipped = isClippedLocal;

    if (isClippedLocal)
        return;

    f32 tx = mtx.m[0][3];
    f32 ty = mtx.m[1][3];
    f32 tz = mtx.m[2][3];

    f32 dx = pos.x - tx;
    f32 dy = pos.y - ty;
    f32 dz = pos.z - tz;

    mClippingInfo.sightDistance = sqrtf((dx * dx) + (dy * dy) + (dz * dz));
}

void BirdGatheringSpotArea::calcRandomGroundTrans(sead::Vector3f* trans) const {
    f32 rx = al::getRandom(-1.0f, 1.0f);
    f32 rz = al::getRandom(-1.0f, 1.0f);

    const sead::Matrix34f& m = getAreaMtx();
    const sead::Vector3f& scale = getAreaShape()->getScale();
    const f32 range = 500.0f;

    f32 x_off = (rx * scale.x) * range;
    f32 z_off = (rz * scale.z) * range;
    f32 y_off = 0.0f;

    trans->x = (x_off * m.m[0][0] + y_off * m.m[0][1] + z_off * m.m[0][2]) + m.m[0][3];
    trans->y = (x_off * m.m[1][0] + y_off * m.m[1][1] + z_off * m.m[1][2]) + m.m[1][3];
    f32 z_comp = (x_off * m.m[2][0] + y_off * m.m[2][1] + z_off * m.m[2][2]);
    trans->z = z_comp + m.m[2][3];
}

bool BirdGatheringSpotArea::isGreaterPriorityNotClipped(const BirdGatheringSpotArea* other) const {
    return other->mClippingInfo.sightDistance <= mClippingInfo.sightDistance;
}

bool BirdGatheringSpotArea::isClipped() const {
    return mClippingInfo.isClipped;
}

f32 BirdGatheringSpotArea::getSightDistance() const {
    return mClippingInfo.sightDistance;
}
