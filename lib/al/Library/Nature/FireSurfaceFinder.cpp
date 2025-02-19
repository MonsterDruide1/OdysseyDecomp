#include "Library/Nature/FireSurfaceFinder.h"

#include "Library/Nature/NatureUtil.h"

namespace al {

FireSurfaceFinder::FireSurfaceFinder(const LiveActor* actor) : mActor(actor) {}

void FireSurfaceFinder::update(const sead::Vector3f& position, const sead::Vector3f& gravity,
                               f32 distance) {
    sead::Vector3f surfacePos = {0.0f, 0.0f, 0.0f};
    sead::Vector3f surfaceNormal = {0.0f, 0.0f, 0.0f};
    mIsFoundSurface =
        calcFindFireSurface(&surfacePos, &surfaceNormal, mActor, position, gravity, distance);

    if (mIsFoundSurface) {
        // requires this manual dot product calculation to match
        mSurface.setDistance(gravity.x * (surfacePos.x - position.x) +
                             gravity.y * (surfacePos.y - position.y) +
                             gravity.z * (surfacePos.z - position.z));
        mSurface.setPosition(surfacePos);
        mSurface.setNormal(surfaceNormal);
    } else {
        mSurface.setDistance(0.0f);
        mSurface.setPosition({0.0f, 0.0f, 0.0f});
        mSurface.setNormal({0.0f, 0.0f, 0.0f});
    }

    mSurface.set1c({0.0f, 0.0f, 0.0f});
}

}  // namespace al
