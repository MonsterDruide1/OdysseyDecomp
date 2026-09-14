#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

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

static_assert(sizeof(CameraPoserInnerTower) == 0x168);

}  // namespace al
