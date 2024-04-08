#pragma once

#include <basis/seadTypes.h>

namespace al {
class LiveActor;
class ActorResource;

struct ActionAnimDataInfo {
    const char* mActionName = nullptr;
    float _8 = -1.0;
    bool mIsKeepAnim = false;
    bool mIsActionAnim = false;
};

struct ActionAnimCtrlInfo {
    ActionAnimCtrlInfo(s32 sklAnimSize);

    const char* mActionName;
    int mSklDataCount;
    ActionAnimDataInfo** mSklDatas;
    ActionAnimDataInfo mMclData;
    ActionAnimDataInfo mMtpData;
    ActionAnimDataInfo mMtsData;
    ActionAnimDataInfo mVisData;
    int mPartNum;
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
    ActionAnimCtrlInfo* mInfos = nullptr;
    const char* mPlayingActionName = nullptr;
    s32 mPartIndex = -1;
};

}  // namespace al
