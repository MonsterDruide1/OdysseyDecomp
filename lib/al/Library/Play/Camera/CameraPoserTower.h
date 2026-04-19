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

private:
    s8 filler[0x1F8 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserTower) == 0x1F8, "al::CameraPoserTower size");
