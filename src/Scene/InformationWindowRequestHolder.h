#pragma once

#include <container/seadPtrArray.h>

#include "Scene/InformationWindowTutorialInfo.h"

class InformationWindowRequestHolder {
public:
    InformationWindowRequestHolder();

    void requestAppear(const TutorialInfo& info);
    void requestCancel(const TutorialInfo& info);
    void requestChangeText(const TutorialInfo& info);
    void requestAppearMovie(const TutorialInfo& info);
    void requestKillMovie(const TutorialInfo& info);
    void requestAppearSystem();
    void requestCloseSystem();
    void reflesh(const TutorialInfo* info);
    TutorialInfo* tryFindHighPriorityInfo(s32* index);
    TutorialInfo* tryFindInfo(TutorialType type);
    void show(TutorialType type);
    void hide(TutorialType type);
    void cancel(const TutorialInfo* info);
    void invalidate(TutorialType type);
    void endStart(const TutorialInfo* info);
    void done(const TutorialInfo* info);
    void killNotRetry();
    void killCanceled();
    bool isExist(TutorialType type, const char* name) const;
    bool isExist(const TutorialInfo& info) const;
    s32 calcCount() const;

private:
    sead::PtrArray<TutorialInfo> mInfos;
    bool mIsEnabled = true;
};

static_assert(sizeof(InformationWindowRequestHolder) == 0x18);
