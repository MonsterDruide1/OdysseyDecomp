#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LayoutInitInfo;
}

class DecideIconLayout : public al::NerveExecutor {
public:
    DecideIconLayout(const char*, const al::LayoutInitInfo*);
    void appear();
    void exeAppear();
    void exeWait();
    void exeDecide();
    bool isDecide() const;
    bool isWait() const;
    bool isEnd() const;
    char filler[0x140];
};
