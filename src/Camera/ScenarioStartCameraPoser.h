#pragma once

#include "Library/Camera/CameraPoser.h"

class ScenarioStartCameraPoser : public al::CameraPoser {
public:
    ScenarioStartCameraPoser(const char* name, bool isUseDefault = false);

    virtual void initScenarioCamera(const al::PlacementInfo& info);
    void loadParam(const al::ByamlIter& iter) override;

    s32 getInterpoleType() const;
    s32 getEndWaitStep() const;
    s32 getNextInterpoleStep() const { return mNextInterpoleStep; }
    s32 getPlayStep() const { return mPlayStep; }
    void setInterpoleRate(f32 rate) { mInterpoleRate = rate; }

protected:
    // Note: the original member names are unverified; member names do not affect codegen.
    f32 mInterpoleRate;
    s32 mNextInterpoleStep;
    s32 mPlayStep;
    s32 mInterpoleType;
    s32 mEndWaitStep;
    bool mIsUseDefault;
};

static_assert(sizeof(ScenarioStartCameraPoser) == 0x158);

class ScenarioStartCameraPoserSimpleZoom : public ScenarioStartCameraPoser {
public:
    ScenarioStartCameraPoserSimpleZoom(const char* name, bool isUseDefault = false);

    void initScenarioCamera(const al::PlacementInfo& info) override;
    void loadParam(const al::ByamlIter& iter) override;
    void update() override;

private:
    sead::Vector3f mTargetTrans;
    f32 mStartDistance;
    f32 mEndDistance;
    f32 mAngleH;
    f32 mAngleV;
};

static_assert(sizeof(ScenarioStartCameraPoserSimpleZoom) == 0x178);

class ScenarioStartCameraPoserRailMove : public ScenarioStartCameraPoser {
public:
    struct Point {
        const char* mObjId;
        f32 mDistance;
        f32 mAngleH;
        f32 mAngleV;
    };

    ScenarioStartCameraPoserRailMove(const char* name, bool isUseDefault = false);

    void initScenarioCamera(const al::PlacementInfo& info) override;
    void loadParam(const al::ByamlIter& iter) override;
    void update() override;

private:
    s32 mPointNum;
    Point** mPoints;
};

static_assert(sizeof(ScenarioStartCameraPoserRailMove) == 0x168);
