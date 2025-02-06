#pragma once

#include "Library/Layout/LayoutActor.h"

class HardIconParts : public al::LayoutActor {
public:
    HardIconParts(const char* name);

    bool isHide() const;
    bool isWait() const;
    void startHide();
    void startAppear();
    void startWait();
    void startEnd();
    void exeHide();
    void exeAppear();
    void exeWait();
    void exePageNext();
    void exePageEnd();
    void exeEnd();
};
