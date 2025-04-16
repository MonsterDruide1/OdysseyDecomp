#pragma once

namespace al {
class LiveActor;

struct ActionScreenEffectCtrlInfo {
    const char* name;
    bool isActive;
    // note: member list not complete.
};

class ActionScreenEffectCtrl {
public:
    static ActionScreenEffectCtrl* tryCreate(const LiveActor*, const char*);

    ActionScreenEffectCtrl(const LiveActor*, const char*);
    void startAction(const char*);
    void update(f32, f32, f32, bool);

private:
    LiveActor* mParentActor;
    const char* mLastEffectName = nullptr;
    s32 mInfoCount = 0;
    ActionScreenEffectCtrlInfo* mInfos = nullptr;
};
}  // namespace al
