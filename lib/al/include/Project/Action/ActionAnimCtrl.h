#pragma once

#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
class ActorResource;

struct ActionAnimCtrlInfo {
    ActionAnimCtrlInfo(int sklAnimSize);

    sead::SafeString mActionName = nullptr;
    unsigned long mSklAnimSize;
    void* _10 = nullptr;
    sead::SafeString* mNameMcl = nullptr;
    float mFloatMcl = -1.0;
    bool isKeepAnimMcl = false;
    bool isActionAnimMcl = false;
    short _26 = 0;
    sead::SafeString* mNameMtp = nullptr;
    float mFloatMtp = -1.0;
    bool isKeepAnimMtp = false;
    bool isActionAnimMtp = false;
    short _35 = 0;
    sead::SafeString* mNameMts = nullptr;
    float mFloatMts = -1.0;
    bool isKeepAnimMts = false;
    bool isActionAnimMts = false;
    short _46 = 0;
    sead::SafeString* mNameVis = nullptr;
    float mFloatVis = -1.0;
    bool isKeepAnimVis = false;
    bool isActionAnimVis = false;
    short _56 = 0;
    int mAnimPartIndex = -1;
};

struct ActionAnimDataInfo {
    sead::SafeString* mAnimName = nullptr;
    float _8 = -1.0;
    short _c = 0;
};

class ActionAnimCtrl {
public:
    static ActionAnimCtrl* tryCreate(LiveActor*, const ActorResource*, const char*, const char*);

    ActionAnimCtrl(LiveActor*);

    void init(const ActorResource*, const sead::BufferedSafeString*, const char*);
    bool start(const char*);
    void sortCtrlIno();

    sead::SafeString* findAnimInfo(const char*) const;
    sead::SafeString* getPlayingActionName();
    int getActionFrameMax(const char*) const;

    bool trySetFrame(float);
    bool isExistAction(const char*);
    bool isActionOneTime(const char*);
    bool isActionEnd();

private:
    const LiveActor* mParentActor;
    sead::SafeString* mPlayingAnimName = nullptr;
    int mUnkn = 0;
    ActionAnimCtrlInfo* mActionAnimCtrlInfo = nullptr;
    sead::SafeString* mPlayingActionName = nullptr;
    int mAnimPartIndex = -1;
};

}  // namespace al
