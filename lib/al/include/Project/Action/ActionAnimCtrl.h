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
    ActionAnimCtrlInfo(s32 sklAnimSize);

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
    bool trySetFrame(float);
    bool isExistAction(const char*);
    bool isActionOneTime(const char*);
    bool isActionEnd();
    void sortCtrlInfo();
    const char* findAnimInfo(const char*) const;
    const char* getPlayingActionName();
    s32 getActionFrameMax(const char*) const;

private:
    LiveActor* mParentActor;
    const char* mPlayingAnimName = nullptr;
    s32 mInfoCount = 0;
    ActionAnimCtrlInfo* mInfoTable = nullptr;
    const char* mPlayingActionName = nullptr;
    s32 mPartIndex = -1;
};

}  // namespace al
