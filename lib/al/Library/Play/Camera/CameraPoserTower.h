#pragma once

#include <math/seadVector.h>

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserTower : public CameraPoser {
public:
    CameraPoserTower(const char* name, const sead::Vector3f* axis);

    void init() override;
    void initByPlacementObj(const PlacementInfo& placementInfo) override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& startInfo) override;
    void movement() override;
    void update() override;
    void resetInputRotate(f32 angle, s32 step);
    void makeLookAtCamera(sead::LookAtCamera* camera) const override;
    void exeTower();
    void exeTowerInput();
    void exeFollow();

    void setDistance(f32 distance) { mDistance = distance; }

    void setUserMarginAngleH(f32 angleH) { mUserMarginAngleH = angleH; }

private:
    u8 _140[0x50];
    f32 mDistance;
    u8 _194[0x10];
    f32 mUserMarginAngleH;
    u8 _1a8[0x50];
};

class CameraPoserInnerTower : public CameraPoser {
public:
    CameraPoserInnerTower(const char* name);

    void init() override;
    void initByPlacementObj(const PlacementInfo& placementInfo) override;
    void loadParam(const ByamlIter& iter) override;
    void update() override;
    void start(const CameraStartInfo& startInfo) override;

private:
    u8 _140[0x28];
};

static_assert(sizeof(CameraPoserTower) == 0x1F8);
static_assert(sizeof(CameraPoserInnerTower) == 0x168);

}  // namespace al
