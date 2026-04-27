#pragma once

#include "Project/Poser/KeyMoveCameraBase.h"

namespace al {

class KeyMoveCameraZoom : public KeyMoveCameraBase {
public:
    KeyMoveCameraZoom(const char* name);

    void initByPlacementObj(const PlacementInfo& info) override;
    void loadParam(const ByamlIter& iter) override;
    void update() override;

private:
    s8 filler[0x180 - sizeof(KeyMoveCameraBase)];
};

}  // namespace al

static_assert(sizeof(al::KeyMoveCameraZoom) == 0x180, "al::KeyMoveCameraZoom size");
