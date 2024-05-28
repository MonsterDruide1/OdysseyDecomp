#pragma once

#include <basis/seadTypes.h>

namespace al {

class ComboCounter {
public:
    ComboCounter() {}
    
    virtual void increment() {
        mCombo++;
    }

private:
    s32 mCombo;
};
static_assert(sizeof(ComboCounter) == 0x10);

}
