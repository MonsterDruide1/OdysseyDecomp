#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class AimingCursor : public al::LayoutActor {
public:
    AimingCursor(const char*, const al::LayoutInitInfo&);

    void startAppear();
    void end();
    void setTrans(const sead::Vector2f&);
    void setScale(f32);
    bool tryLookOn();
    bool tryLookOff();
    void exeAppear();
    void exeWait();
    void exeEnd();

private:
    bool mIsLockOn = false;
};
