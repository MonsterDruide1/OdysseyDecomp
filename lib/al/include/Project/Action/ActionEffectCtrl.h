#pragma once

#include <basis/seadTypes.h>

#include "Library/Effect/IUseEffectKeeper.h"

namespace al {

struct ActionEffectCtrlInfo {};

class ActionEffectCtrl {
private:
    IUseEffectKeeper* mIEffectKeeper;
    s32 mInfoMax;
    ActionEffectCtrlInfo* mInfoArr;
};

}  // namespace al
