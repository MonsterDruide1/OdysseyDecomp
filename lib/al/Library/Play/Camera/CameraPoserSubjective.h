#pragma once

#include <basis/seadTypes.h>

#include "Library/Camera/CameraPoser.h"

namespace al {
class ByamlIter;
struct CameraStartInfo;
class IntervalTrigger;
class LiveActor;

class CameraPoserSubjective : public CameraPoser {
public:
    CameraPoserSubjective(const char* name);
    void init() override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void movement() override;
    void update() override;
    void startSnapShotMode() override;
    void endSnapShotMode() override;
    void exeWait();
    void exeReset();
    f32 getCameraOffsetFront();

    bool isZooming() const override { return mIsZooming; }

    bool isEnableRotateByPad() const override { return true; }

    void setStartAngleH(f32 angle) {
        mIsSetStartAngleH = true;
        mStartAngleH = angle;
    }

    void setCameraOffsetUp(f32 offset) { mCameraOffsetUp = offset; }

private:
    f32 mInitialAngleH = 0.0f;
    f32 mCurrentAngleH = 0.0f;
    f32 mCurrentAngleV = 0.0f;
    f32 mTargetAngleV = 0.0f;
    f32 mTargetAngleH = 0.0f;  // an angle
    f32 mGyroAngleH = 0.0f;
    f32 mGyroAngleV = 0.0f;
    f32 _15c = 0.0f;
    f32 _160 = 0.0f;
    bool mIsZooming = false;
    bool mIsRequestZoomIn = false;
    bool mIsResetAngleHValid = false;
    f32 mResetAngleH = 0.0f;
    f32 mResetAngleV = 0.0f;
    f32 mMinAngleV = -30.0f;
    f32 mMaxAngleV = 75.0f;
    f32 mCameraOffsetUp = 180.0f;
    f32 mStartAngleV = 0.0f;
    bool mIsSetStartAngleH = false;
    f32 mStartAngleH = 0.0f;
    f32 mPrevAngleH = 0.0f;
    f32 mPrevAngleV = 0.0f;
    IntervalTrigger* mIntervalTrigger = nullptr;
    LiveActor* mActor = nullptr;
    bool mIsSnapshotModeActive = false;
};

static_assert(sizeof(CameraPoserSubjective) == 0x1A8);

}  // namespace al
