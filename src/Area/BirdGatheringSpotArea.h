#pragma once

#include <math/seadBoundBox.h>
#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"

namespace al {
class ClippingJudge;
}

class BirdGatheringSpotArea : public al::AreaObj {
public:
    struct AreaClippingInfo {
        AreaClippingInfo();

        bool isClipped = false;
        f32 sightDistance = 0.0f;
        sead::BoundBox3f boundingBox = sead::BoundBox3f(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
    };

    BirdGatheringSpotArea(const char* name);

    void init(const al::AreaInitInfo& initInfo) override;

    void calcRandomGroundTrans(sead::Vector3f* trans) const;
    f32 getSightDistance() const;
    bool isClipped() const;
    bool isGreaterPriorityNotClipped(const BirdGatheringSpotArea* other) const;
    void updateClipping(const al::ClippingJudge*, const sead::Vector3f&);

private:
    s32 mBirdNumMax = 0;
    AreaClippingInfo mClippingInfo;
};
