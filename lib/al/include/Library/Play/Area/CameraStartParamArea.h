#pragma once

#include "Library/Area/AreaObj.h"

namespace al {
class CameraStartParamArea : public AreaObj {
public:
    CameraStartParamArea(const char* name);

    void init(const AreaInitInfo& areaInitInfo) override;

    void appear();
    void kill();

private:
    bool mIsAlive = true;
    f32* mAngleH = nullptr;
    f32* mAngleV = nullptr;
};
}  // namespace al
