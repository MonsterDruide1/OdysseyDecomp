#pragma once

#include <basis/seadTypes.h>

#include "Library/Effect/IUseEffectKeeper.h"

namespace al {
class EffectKeeper;

struct ActionEffectCtrlInfo {
    const char* name;
    const char* materialName;
    bool hasEmittedEffect;
};

class ActionEffectCtrl : public IUseEffectKeeper {
public:
    static ActionEffectCtrl* tryCreate(IUseEffectKeeper*);

    ActionEffectCtrl(IUseEffectKeeper*);

    void startAction(const char*);
    void update(f32, f32, f32, bool);
    bool isKeepSameEffectNext(const ActionEffectCtrlInfo*, const char*);

private:
    EffectKeeper* mEffectKeeper = nullptr;
    s32 mInfoCount = 0;
    ActionEffectCtrlInfo* mInfoTable = nullptr;
};
}  // namespace al
