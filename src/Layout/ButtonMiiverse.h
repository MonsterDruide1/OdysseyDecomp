#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;
}

class ButtonMiiverse : public al::LayoutActor {
public:
    ButtonMiiverse();

    void init(const al::LayoutInitInfo& info);
    bool isOn() const;
    void setOff();
    void validate();
    void forceValidate();
    void invalidate();

    void exeWait();
    void exeHoldOn();
    void exeHoldOff();
    void exeDecide();
    void exeOnWait();
    void exeDisable();
};
