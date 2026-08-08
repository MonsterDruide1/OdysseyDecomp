#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

class FukankunZoomCapMessage {
public:
    FukankunZoomCapMessage(const al::LiveActor* actor);

    void init(const al::ActorInitInfo& info, const char* capMessageName,
              const char* fukankunZoomMessageName);
    void initAfterPlacement();
    void update();

    const al::LiveActor* mActor;
    s32 mFukankunZoomType;
    s32 mCapMessageShowType;
    s32 mFukankunZoomWatchTimeThreshold;
    bool mIsDisableAfterEnding;
    sead::Vector3f mFukankunZoomOffset;
    const char* mFukankunZoomTargetJointName;
    bool mIsDisabled;
    sead::FixedSafeString<256>* mCapMessageName;
    sead::FixedSafeString<256>* mFukankunZoomMessageName;
    bool mIsCapMessageShown;
};

static_assert(sizeof(FukankunZoomCapMessage) == 0x50);
