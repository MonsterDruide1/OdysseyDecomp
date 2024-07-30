#pragma once

#include <math/seadVector.h>
#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class PlayerHolder;
class SceneCameraInfo;
}

class Compass : public al::LayoutActor {
public:
    Compass(const char* name, const al::LayoutInitInfo& info, const al::PlayerHolder* playerHolder);

    virtual void appear() override;

    void end();

    void exeAppear();
    void updateLayout();
    void exeWait();
    void exeEnd();

private:
    al::SceneCameraInfo* mSceneCamInfo;
    const al::PlayerHolder* mPlayerHolder;
    sead::Vector3f m140;
    f32 m14C;
};
