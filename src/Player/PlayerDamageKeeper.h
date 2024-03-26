#pragma once

#include <basis/seadTypes.h>

class PlayerDamageKeeper {
public:
    void activatePreventDamage();
    void dead();
    void damage(s32 level);
};
