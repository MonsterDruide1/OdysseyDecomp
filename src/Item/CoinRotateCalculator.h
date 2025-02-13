#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

namespace al {
class LiveActor;
}  // namespace al

class CoinRotateCalculator {
public:
    CoinRotateCalculator(al::LiveActor* actor);
    void reset();
    void update(const sead::Vector3f& force, bool checkWater);
    void addFishingLineTouch();
    f32 getRotate() const;
    f32 getRotateSpeed() const;

private:
    al::LiveActor* mActor = nullptr;
    f32 mRotate = 0.0f;
    f32 mLastObjAngle = 0.0f;
    s32 mForceFrames = 0;
    f32 mForceOffset = 0.0f;
    f32 mFishingLineOffset = 0.0f;
    s32 mObjCountOffset = 0;
    f32 mRotateOffset = 0.0f;
    bool mIsInWater = false;
};
