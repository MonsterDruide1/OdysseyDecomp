#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;

class EventFlowActorTerritory {
public:
    EventFlowActorTerritory(LiveActor*, const char*);

    f32 getRadius() const { return mRadius; }

    f32 getEmotionRadius() const { return mEmotionRadius; }

    f32 getEmotionLowPriorityRadius() const { return mEmotionLowPriorityRadius; }

    f32 getTalkableRadius() const { return mTalkableRadius; }

    const sead::Vector3f& getLocalOffset() const { return mLocalOffset; }

    const sead::Vector3f& getBalloonLocalOffset() const { return mBalloonLocalOffset; }

    void setBalloonLocalOffset(const sead::Vector3f& localOffset) {
        mBalloonLocalOffset.set(localOffset);
    }

    f32 getBalloonCollisionCheckOffsetRadius() const { return mBalloonCollisionCheckOffsetRadius; }

private:
    f32 mRadius = 1000.0f;
    f32 mEmotionRadius = 4000.0f;
    f32 mEmotionLowPriorityRadius = 1000.0f;
    f32 mTalkableRadius = 400.0f;
    sead::Vector3f mLocalOffset = sead::Vector3f::zero;
    sead::Vector3f mBalloonLocalOffset = {0.0f, 300.0f, 0.0f};
    f32 mBalloonCollisionCheckOffsetRadius = 0.0f;
};

static_assert(sizeof(EventFlowActorTerritory) == 0x2C);
}  // namespace al
