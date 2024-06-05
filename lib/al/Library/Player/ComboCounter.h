#pragma once

#include <basis/seadTypes.h>

namespace al {

class ComboCounter {
public:
    ComboCounter() {}
    
    virtual void increment() {
        mCombo++;
    }

    void reset() {
        mCombo = 0;
    }

private:
    s32 mCombo;
};
static_assert(sizeof(ComboCounter) == 0x10);

}
