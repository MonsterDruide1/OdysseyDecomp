#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserTower : public CameraPoser {
public:
    CameraPoserTower(const char* name, const sead::Vector3f* posPtr = nullptr);

    void init() override;
    void initByPlacementObj(const PlacementInfo& info) override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void movement() override;
    void update() override;
    void resetInputRotate(f32, s32);
    void makeLookAtCamera(sead::LookAtCamera* cam) const override;

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

static_assert(sizeof(CameraPoserTower) == 0x1F8);

}  // namespace al
