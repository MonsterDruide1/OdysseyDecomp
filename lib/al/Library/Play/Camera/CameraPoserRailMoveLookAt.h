#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserRailMoveLookAt : public CameraPoser {
public:
    CameraPoserRailMoveLookAt(const char* name);

    void init() override;
    void initByPlacementObj(const PlacementInfo& info) override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void update() override;

private:
    s8 filler[0x180 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserRailMoveLookAt) == 0x180, "al::CameraPoserRailMoveLookAt size");
