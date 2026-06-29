#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;
class LiveActor;
}  // namespace al

namespace sead {
template <typename T>
class BufferedSafeStringBase;
}  // namespace sead

class FukankunZoomCapMessage {
public:
    FukankunZoomCapMessage(const al::LiveActor* actor);

    void init(const al::ActorInitInfo& info, const char* capMessageName,
              const char* stageMessageName);
    void initAfterPlacement();
    void update();

    void setWatchCount(s32 watchCount) { mWatchCount = watchCount; }

    void resetZoomTypes() {
        mFukankunZoomType = 0;
        mCapMessageShowType = 0;
    }

private:
    const al::LiveActor* mActor = nullptr;
    s32 mFukankunZoomType = 0;
    s32 mCapMessageShowType = 1;
    s32 mWatchCount = 0;
    bool mIsDisableAfterEnding = false;
    u8 _15[3] = {};
    sead::Vector3f mOffset = sead::Vector3f::zero;
    const char* mTargetJointName = nullptr;
    bool mIsDisable = false;
    u8 _31[7] = {};
    sead::BufferedSafeStringBase<char>* mCapMessageName = nullptr;
    sead::BufferedSafeStringBase<char>* mStageMessageName = nullptr;
    bool mIsShown = false;
    u8 _49[7] = {};
};

static_assert(sizeof(FukankunZoomCapMessage) == 0x50);
