#pragma once

#include "Library/Layout/LayoutInitInfo.h"

class DecideIconLayout {
public:
    DecideIconLayout(char* param_1, const al::LayoutInitInfo* param_2); // Haven't Worked out the params yet
    void appear();
    void exeAppear();
    void exeWait();
    void exeDecide();
    void isDecide();
    void isWait();
    void isEnd();
    char filler[0x150];

};

