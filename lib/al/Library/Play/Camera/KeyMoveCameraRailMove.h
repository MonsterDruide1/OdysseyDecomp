#pragma once

#include "Project/Poser/KeyMoveCameraBase.h"

namespace al {

class KeyMoveCameraRailMove : public KeyMoveCameraBase {
public:
    KeyMoveCameraRailMove(const char* name);

    void initByPlacementObj(const PlacementInfo& info) override;
    void initPointInfoTable(s32);
    void loadParam(const ByamlIter& iter) override;
    void start(const CameraStartInfo&) override;
    void calcPoseInfoDirect(f32*, f32*, f32*) const;
    void updatePose();
    void update() override;

private:
    s8 filler[0x198 - sizeof(KeyMoveCameraBase)];
};

}  // namespace al

static_assert(sizeof(al::KeyMoveCameraRailMove) == 0x198, "al::KeyMoveCameraRailMove size");
