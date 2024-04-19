#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;

struct ActionScreenEffectCtrlInfo {
    const char* mName;
    bool mIsActive;
    // note: member list not complete.
};

class ActionScreenEffectCtrl {
public:
    static ActionScreenEffectCtrl* tryCreate(const LiveActor*, const char*);

    ActionScreenEffectCtrl(const LiveActor*, const char*);
    void startAction(const char*);
    void update(float, float, float, bool);

private:
    LiveActor* mParentActor;
    const char* mLastEffectName = nullptr;
    s32 mInfoCount = 0;
    ActionScreenEffectCtrlInfo* mInfoTable = nullptr;
};
}  // namespace al
