#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ActorResource;

struct ActionAnimDataInfo {
    const char* mName = nullptr;
    float _8 = -1.0;
    bool mIsKeep = false;
    bool mIsAction = false;
    short _e = 0;
};

struct ActionAnimCtrlInfo {
    ActionAnimCtrlInfo(int sklAnimSize);

    const char* mActionName;
    s32 mStackSize = 0;
    ActionAnimDataInfo* mAnimDataStack;
    s32 mAnimPartIndex;
};

class ActionAnimCtrl {
public:
    static ActionAnimCtrl* tryCreate(LiveActor*, const ActorResource*, const char*, const char*);

    ActionAnimCtrl(LiveActor*);

    void init(const ActorResource*, const char*, const char*);
    bool start(const char*);
    const char* findAnimInfo(const char*) const;
    int getActionFrameMax(const char*) const;
    bool trySetFrame(float);
    bool isExistAction(const char*);
    bool isActionOneTime(const char*);
    bool isActionEnd();
    const char* getPlayingActionName();
    void sortCtrlInfo();

private:
    LiveActor* mParentActor;
    const char* mPlayingAnimName = nullptr;
    int mStackSize = 0;
    ActionAnimCtrlInfo* mAnimInfoStack = nullptr;
    const char* mPlayingActionName = nullptr;
    int mPartIndex = -1;
};

}  // namespace al
