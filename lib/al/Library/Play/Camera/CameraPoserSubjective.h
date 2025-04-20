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
    CameraPoserSubjective(const char*);
    void init() override;
    void loadParam(const ByamlIter&) override;
    void start(const CameraStartInfo&) override;
    void movement() override;
    void update() override;
    void startSnapShotMode() override;
    void endSnapShotMode() override;
    void exeWait();
    void exeReset();
    f32 getCameraOffsetFront();
    bool isZooming() const override;
    bool isEnableRotateByPad() const override;

    void setStartAngleH(f32 angle) {
        mIsSetStartAngleH = true;
        mStartAngleH = angle;
    }

    void setCameraOffsetUp(f32 offset) { mCameraOffsetUp = offset; }

private:
    f32 _140;
    f32 _144;
    f32 _148;
    f32 _14c;
    f32 _150;
    f32 mGyroAngleH;
    f32 mGyroAngleV;
    f32 _15c;
    f32 _160;
    bool mIsZooming;
    bool _165;
    f32 _168;
    f32 _16c;
    f32 mMinAngleV;
    f32 mMaxAngleV;
    f32 mCameraOffsetUp;
    f32 mStartAngleV;
    bool mIsSetStartAngleH;
    f32 mStartAngleH;
    f32 _188;
    f32 _18c;
    IntervalTrigger* mIntervalTrigger;
    LiveActor* mActor;
    bool mIsSnapShowMode;
};

static_assert(sizeof(CameraPoserSubjective) == 0x1A8);

}  // namespace al
