#pragma once

#include <math/seadBoundBox.h>
#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

namespace al {
class AreaInitInfo;
class ClippingJudge;
}  // namespace al

class BirdGatheringSpotArea : public al::AreaObj {
public:
    BirdGatheringSpotArea(const char* name);
    void init(const al::AreaInitInfo& info) override;

    void updateClipping(const al::ClippingJudge* judge, const sead::Vector3f& pos);
    bool isClipped() const;
    f32 getSightDistance() const;
    bool isGreaterPriorityNotClipped(const BirdGatheringSpotArea* other) const;
    void calcRandomGroundTrans(sead::Vector3f* trans) const;

    struct AreaClippingInfo {
        AreaClippingInfo();
        bool isClipped = false;
        f32 sightDistance = 0.0f;
        sead::BoundBox3f boundingBox = sead::BoundBox3f(sead::Vector3f::zero, sead::Vector3f::zero);
    };

private:
    s32 mBirdNumMax = 0;
    AreaClippingInfo mClippingInfo;
};
