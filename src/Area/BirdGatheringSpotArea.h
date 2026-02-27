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
    struct AreaClippingInfo {
        AreaClippingInfo();

        bool isClipped = false;
        f32 sightDistance = 0.0f;
        sead::BoundBox3f boundingBox = {sead::Vector3f::zero, sead::Vector3f::zero};
    };

    BirdGatheringSpotArea(const char* name);
    void init(const al::AreaInitInfo& info) override;

    void updateClipping(const al::ClippingJudge* judge, const sead::Vector3f& pos);
    bool isClipped() const;
    f32 getSightDistance() const;
    bool isGreaterPriorityNotClipped(const BirdGatheringSpotArea* other) const;
    void calcRandomGroundTrans(sead::Vector3f* outTrans) const;

private:
    s32 mBirdNumMax = 0;
    AreaClippingInfo mClippingInfo;
};
