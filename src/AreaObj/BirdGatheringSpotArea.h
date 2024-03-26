#pragma once

#include <math/seadBoundBox.h>

#include "Library/Area/AreaObj.h"

namespace al {
class ClippingJudge;
}

class BirdGatheringSpotArea : public al::AreaObj {
    struct AreaClippingInfo {
        bool mIsClipped = false;
        f32 mSightDistance = 0.0;
        sead::BoundBox3f mBoundBox = sead::BoundBox3f(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
    };

public:
    BirdGatheringSpotArea(const char* name);

    void init(const al::AreaInitInfo& initInfo) override;

    void calcRandomGroundTrans(sead::Vector3f* trans) const;
    s32 getSightDistance() const;
    bool isClipped() const;
    bool isGreaterPriorityNotClipped(const BirdGatheringSpotArea* other) const;
    void updateClipping(const al::ClippingJudge*, const sead::Vector3f&);

private:
    s32 mBirdNumMax = 0;
    AreaClippingInfo mClippingInfo;
};
