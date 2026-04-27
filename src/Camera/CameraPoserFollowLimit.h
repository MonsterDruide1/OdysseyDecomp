#pragma once

#include "Library/Camera/CameraPoser.h"

class CameraPoserFollowLimit : public al::CameraPoser {
public:
    CameraPoserFollowLimit(const char* name);

    void init() override;
    bool isUseDistanceCurve() const;
    void initByPlacementObj(const al::PlacementInfo& info) override;
    void loadParam(const al::ByamlIter& iter) override;
    void start(const al::CameraStartInfo& info) override;
    f32 getAngleDegreeV() const;
    bool isInvalidCollider() const;
    f32 calcDistanceRaw() const;
    f32 calcDistance() const;
    bool tryStartWater(bool);
    void movement() override;
    void update() override;
    void calcCameraPose(sead::LookAtCamera* cam) const override;
    bool receiveRequestFromObject(const al::CameraObjectRequestInfo& info) override;
    void startSnapShotMode() override;
    void endSnapShotMode() override;
    bool isEnableRotateByPad() const override;
    void exeFollow();
    bool trySwitchLimitObj();
    void updateInputOrSubTargetTurnH();
    void exeResetAngle();
    void endWater();
    void exeFollowRail();
    void exeFollowRailUserCtrl();
    void updateRotateSpeedInputH();
    void exeWater();
    void exeWaterRadicon();
    void startTurnBrake(s32);
    bool requestTurnToDirection(const al::CameraTurnInfo* info) override;
    void invalidateAutoResetLowAngleV();

private:
    s8 filler[0x2A0 - sizeof(al::CameraPoser)];
};

static_assert(sizeof(CameraPoserFollowLimit) == 0x2A0, "CameraPoserFollowLimit size");
