#pragma once

class PlayerDamageKeeper {
public:
    void activatePreventDamage();
    void dead();
    void damage(s32 level);
};
