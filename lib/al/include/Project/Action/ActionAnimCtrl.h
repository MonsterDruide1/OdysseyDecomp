#pragma once

#include <prim/seadSafeString.h>

namespace al {
class LiveActor;
class ActorResource;

struct ActionAnimCtrlInfo {
    ActionAnimCtrlInfo(int sklAnimSize); // int = mSklAnimSize

    sead::SafeString mActionName = nullptr;
    unsigned long mSklAnimSize;
    void* mVoid = nullptr;
    sead::SafeString* mNameMcl = nullptr;
    float mFloatMcl = -1.0;
    bool isKeepAnimMcl = false;
    bool isActionAnimMcl = false;
    short mUnusedMcl = 0;
    sead::SafeString* mNameMtp = nullptr;
    float mFloatMtp = -1.0;
    bool isKeepAnimMtp = false;
    bool isActionAnimMtp = false;
    short mUnusedMtp = 0;
    sead::SafeString* mNameMts = nullptr;
    float mFloatMts = -1.0;
    bool isKeepAnimMts = false;
    bool isActionAnimMts = false;
    short mUnusedMts = 0;
    sead::SafeString* mNameVis = nullptr;
    float mFloatVis = -1.0;
    bool isKeepAnimVis = false;
    bool isActionAnimVis = false;
    short mUnusedVis = 0;
    int mAnimPartIndex = -1;

};

struct ActionAnimDataInfo {
    sead::SafeString* mAnimName = nullptr;
    float mFloat = -1.0;
    short mShort = 0;
};

class ActionAnimCtrl {
    ActionAnimCtrl(const LiveActor* actor);

    static ActionAnimCtrl* tryCreate(LiveActor*, const ActorResource*, const char*, const char*);


private:
    const LiveActor* mParentActor;
    sead::SafeString* mPlayingAnimName = nullptr;
    int mUnkn = 0;
    ActionAnimCtrlInfo* mActionAnimCtrlInfo = nullptr;
    sead::SafeString* mPlayingActionName = nullptr;
    int mAnimPartIndex;
};

}
