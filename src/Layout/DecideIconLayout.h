#pragma once

#include "Library/Layout/LayoutInitInfo.h"

class DecideIconLayout : public al::NerveExecutor {
public:
    DecideIconLayout(const char* ,
                     const al::LayoutInitInfo*);
    void appear();
    void exeAppear();
    void exeWait();
    void exeDecide();
    bool isDecide();
    bool isWait();
    bool isEnd();
    char filler[0x150];
};
