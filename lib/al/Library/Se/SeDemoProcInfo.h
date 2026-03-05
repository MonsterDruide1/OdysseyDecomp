#pragma once

#include <basis/seadTypes.h>

#include "Project/Audio/AudioEffectProcInfo.h"

namespace al {
template <typename T>
class AudioInfoListWithParts;
class ByamlIter;

struct SeDemoProcInfo {
    static SeDemoProcInfo* createInfo(const ByamlIter& iter);
    static SeDemoProcInfo* tryCreate(const char*);
    static s32 compareInfo(const SeDemoProcInfo* lhs, const SeDemoProcInfo* rhs);
    static bool isChangeSituation(const char* action);
    static bool isPause(const char* action);
    static bool isStopOneShot(const char* action);
    static bool isChnageListenerPoser(const char* action);
    static bool isPlaySe(const char* action);

    SeDemoProcInfo();
    SeDemoProcInfo(const SeDemoProcInfo&);

    virtual ~SeDemoProcInfo();
    virtual void dummy();

    const char* name = nullptr;
    s32 triggerFrame = 0;
    s32 endFrame = -1;
};

static_assert(sizeof(SeDemoProcInfo) == 0x18);

struct SeDemoSituationInfo : public SeDemoProcInfo {
    static SeDemoSituationInfo* createInfo(const ByamlIter& iter, const char* name,
                                           s32 triggerFrame, s32 endFrame);
    static s32 compareInfo(const SeDemoSituationInfo* lhs, const SeDemoSituationInfo* rhs);

    SeDemoSituationInfo();
    SeDemoSituationInfo(const SeDemoSituationInfo& demoSituationInfo);

    const char* situationName = nullptr;
    s32 startFadeFrameNum = -1;
    s32 endFadeFrameNum = -1;
};

static_assert(sizeof(SeDemoSituationInfo) == 0x28);

struct SeDemoPauseInfo : public SeDemoProcInfo {
    static SeDemoPauseInfo* createInfo(const ByamlIter& iter, const char* name, s32 triggerFrame,
                                       s32 endFrame);
    static s32 compareInfo(const SeDemoPauseInfo* lhs, const SeDemoPauseInfo* rhs);

    SeDemoPauseInfo();
    SeDemoPauseInfo(const SeDemoPauseInfo& demoPauseInfo);

    s32 fadeOutFrameNum = 0;
    s32 fadeInFrameNum = 0;
};

static_assert(sizeof(SeDemoPauseInfo) == 0x20);

struct SeDemoStopOneShotInfo : public SeDemoProcInfo {
    static SeDemoStopOneShotInfo* createInfo(const ByamlIter& iter, const char* name,
                                             s32 triggerFrame, s32 endFrame);
    static s32 compareInfo(const SeDemoStopOneShotInfo* lhs, const SeDemoStopOneShotInfo* rhs);

    SeDemoStopOneShotInfo();
    SeDemoStopOneShotInfo(const SeDemoStopOneShotInfo& demoStopOneShotInfo);

    s32 fadeOutFrameNum = 45;
};

static_assert(sizeof(SeDemoStopOneShotInfo) == 0x20);

struct SeDemoListenerPoserInfo : public SeDemoProcInfo {
    static SeDemoListenerPoserInfo* createInfo(const ByamlIter& iter, const char* name,
                                               s32 triggerFrame, s32 endFrame);
    static s32 compareInfo(const SeDemoListenerPoserInfo* lhs, const SeDemoListenerPoserInfo* rhs);

    SeDemoListenerPoserInfo();
    SeDemoListenerPoserInfo(const SeDemoListenerPoserInfo& demoListenerPoserInfo);

    const char* poserName = nullptr;
    s32 fadeOutFrameNum = 0;
    s32 fadeInFrameNum = 0;
};

static_assert(sizeof(SeDemoListenerPoserInfo) == 0x28);

struct SeDemoPlaySeInfo : public SeDemoProcInfo {
    static SeDemoPlaySeInfo* createInfo(const ByamlIter& iter, const char* name, s32 triggerFrame,
                                        s32 endFrame);
    static s32 compareInfo(const SeDemoPlaySeInfo* lhs, const SeDemoPlaySeInfo* rhs);

    SeDemoPlaySeInfo();
    SeDemoPlaySeInfo(const SeDemoPlaySeInfo& demoPlaySeInfo);

    bool isStopSe = false;
    const char* playName = nullptr;
    s32 fadeOutFrameNum = -1;
    bool isStartSeEvenIfDemoSkip = false;
    bool isNotStopSeEvenIfDemoSkip = false;
};

static_assert(sizeof(SeDemoPlaySeInfo) == 0x30);

struct SeDemoSyncedProcInfo : public AudioDemoSyncedProcInfo {
    static SeDemoSyncedProcInfo* createInfo(const ByamlIter& iter);
    static s32 compareInfo(const SeDemoSyncedProcInfo* lhs, const SeDemoSyncedProcInfo* rhs);

    SeDemoSyncedProcInfo();
    SeDemoSyncedProcInfo(const SeDemoSyncedProcInfo& demoSyncedProcInfo);

    al::AudioInfoListWithParts<al::SeDemoProcInfo>* procInfoList=nullptr;
    al::AudioInfoListWithParts<al::SeDemoProcInfo>* procEndInfoList=nullptr;
    bool isDisableAudioMaximizer = false;
    bool isNotStopOneShotSe = false;
    s32 oneShotSeFadeOutFrameNum = 45;
};

static_assert(sizeof(SeDemoSyncedProcInfo) == 0x30);

}  // namespace al
