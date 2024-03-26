#pragma once

#include <basis/seadTypes.h>

#include "Library/Effect/IUseEffectKeeper.h"

namespace al {
struct ActionEffectCtrlInfo {
    const char* mName;
    const char* mMaterialName;
    bool mHasEmittedEffect;
};

class ActionEffectCtrl : public IUseEffectKeeper {
public:
    static ActionEffectCtrl* tryCreate(IUseEffectKeeper);

    ActionEffectCtrl(IUseEffectKeeper*);

    void startAction(const char*);
    void update(float, float, float, bool);
    bool isKeepSameEffectNext(const ActionEffectCtrlInfo*, const char*);

private:
    EffectKeeper* mEffectKeeper;
    s32 mEffectCount;
    ActionEffectCtrlInfo* mEffectStack;
};
}  // namespace al
