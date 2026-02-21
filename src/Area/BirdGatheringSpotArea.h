#pragma once

#include <math/seadBoundBox.h>
#include <math/seadVector.h>

#include "Library/Area/AreaObj.h"
#include "Library/Clipping/ClippingJudge.h"

class BirdGatheringSpotArea : public al::AreaObj {
public:
    BirdGatheringSpotArea(const char* name);
    virtual void init(const al::AreaInitInfo& info) override;

    void updateClipping(const al::ClippingJudge* judge, const sead::Vector3f& pos);
    bool isClipped() const;
    f32 getSightDistance() const;
    bool isGreaterPriorityNotClipped(const BirdGatheringSpotArea* other) const;
    void calcRandomGroundTrans(sead::Vector3f* trans) const;

    struct AreaClippingInfo {
        AreaClippingInfo();
        bool isClipped;
        u8 _pad[3];
        f32 sightDistance;
        sead::BoundBox3f boundingBox;
    };

private:
    s32 mBirdNumMax;
    AreaClippingInfo mClippingInfo;
};
