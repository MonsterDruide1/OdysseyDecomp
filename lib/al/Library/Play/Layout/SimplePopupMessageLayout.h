#pragma once

#include <math/seadVector.h>

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
class SimplePopupMessageLayout : public LayoutActor {
public:
    SimplePopupMessageLayout(const char*, const char*, const LayoutInitInfo&, const char*, bool);
    void appear();
    void end();
    void startWait();
    void exeAppear();
    void refreshPos();
    void exeWait();
    void exeEnd();
    bool isWait() const;
    bool isAppearOrWait() const;

private:
    s32 mLifetime = -1;
    sead::Vector3f mWorldPos;
};
}  // namespace al
