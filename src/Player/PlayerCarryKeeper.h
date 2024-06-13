#pragma once

#include <basis/seadTypes.h>

class PlayerCarryKeeper {
public:
    bool isThrowHold() const;
    bool isCarry() const;

private:
    u8 padding[0x70];
};
