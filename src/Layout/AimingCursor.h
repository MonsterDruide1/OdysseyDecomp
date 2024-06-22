#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class AimingCursor : public al::LayoutActor {
public:
    AimingCursor(const char* name, const al::LayoutInitInfo& info);

    void startAppear();
    void end();
    void setTrans(const sead::Vector2f& pos);
    void setScale(f32 scale);
    bool tryLookOn();
    bool tryLookOff();
    void exeAppear();
    void exeWait();
    void exeEnd();

private:
    bool mIsLookOn = false;
};
