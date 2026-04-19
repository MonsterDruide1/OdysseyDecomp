#pragma once

#include "Library/Camera/CameraPoser.h"

namespace al {

class CameraPoserRailMoveMovie : public CameraPoser {
public:
    CameraPoserRailMoveMovie(const char* name);

    void initByPlacementObj(const PlacementInfo& info) override;
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo& info) override;
    void exeMove();

private:
    s8 filler[0x148 - sizeof(CameraPoser)];
};

}  // namespace al

static_assert(sizeof(al::CameraPoserRailMoveMovie) == 0x148, "al::CameraPoserRailMoveMovie size");
