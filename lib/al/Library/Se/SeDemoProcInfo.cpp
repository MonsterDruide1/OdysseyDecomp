#include "Library/Se/SeDemoProcInfo.h"

#include "Library/Audio/AudioInfo.h"
#include "Library/Base/StringUtil.h"
#include "Library/Yaml/ByamlIter.h"

namespace al {
SeDemoProcInfo* SeDemoProcInfo::createInfo(const ByamlIter& iter) {
    const char* name = nullptr;
    if (!iter.tryGetStringByKey(&name, "Name"))
        name = nullptr;

    s32 triggerFrame = 0;
    iter.tryGetIntByKey(&triggerFrame, "TriggerFrame");

    s32 endFrame = -1;
    iter.tryGetIntByKey(&endFrame, "EndFrame");

    if (!name)
        return nullptr;

    if (SeDemoProcInfo::isChangeSituation(name))
        return SeDemoSituationInfo::createInfo(iter, name, triggerFrame, endFrame);

    if (SeDemoProcInfo::isPause(name))
        return SeDemoPauseInfo::createInfo(iter, name, triggerFrame, endFrame);

    if (SeDemoProcInfo::isStopOneShot(name))
        return SeDemoStopOneShotInfo::createInfo(iter, name, triggerFrame, endFrame);

    if (SeDemoProcInfo::isChnageListenerPoser(name))
        return SeDemoListenerPoserInfo::createInfo(iter, name, triggerFrame, endFrame);

    if (SeDemoProcInfo::isPlaySe(name))
        return SeDemoPlaySeInfo::createInfo(iter, name, triggerFrame, endFrame);

    return nullptr;
}

SeDemoProcInfo* SeDemoProcInfo::tryCreate(const char* name) {
    if (!name)
        return nullptr;

    if (isChangeSituation(name)) {
        SeDemoSituationInfo* info = new SeDemoSituationInfo();
        info->name = name;
        return info;
    }

    if (isPause(name)) {
        SeDemoPauseInfo* info = new SeDemoPauseInfo();
        info->name = name;
        return info;
    }

    if (isStopOneShot(name)) {
        SeDemoStopOneShotInfo* info = new SeDemoStopOneShotInfo();
        info->name = name;
        return info;
    }

    if (isChnageListenerPoser(name)) {
        SeDemoListenerPoserInfo* info = new SeDemoListenerPoserInfo();
        info->name = name;
        return info;
    }

    if (isPlaySe(name)) {
        SeDemoPlaySeInfo* info = new SeDemoPlaySeInfo();
        info->name = name;
        return info;
    }

    return nullptr;
}

s32 SeDemoProcInfo::compareInfo(const SeDemoProcInfo* lhs, const SeDemoProcInfo* rhs) {
    return strcmp(lhs->name, rhs->name);
}

bool SeDemoProcInfo::isChangeSituation(const char* name) {
    if (name)
        return isEqualString(name, "ChnageSituation");
    return false;
}

bool SeDemoProcInfo::isPause(const char* name) {
    if (name)
        return isEqualString(name, "Pause");
    return false;
}

bool SeDemoProcInfo::isStopOneShot(const char* name) {
    if (name)
        return isEqualString(name, "StopOneShot");
    return false;
}

bool SeDemoProcInfo::isChnageListenerPoser(const char* name) {
    if (name)
        return isEqualString(name, "ChnageListenerPoser");
    return false;
}

bool SeDemoProcInfo::isPlaySe(const char* name) {
    if (name)
        return isEqualString(name, "PlaySe");
    return false;
}

SeDemoProcInfo::SeDemoProcInfo() = default;

SeDemoProcInfo::SeDemoProcInfo(const SeDemoProcInfo& info)
    : name(info.name), triggerFrame(info.triggerFrame), endFrame(info.endFrame) {}

SeDemoProcInfo::~SeDemoProcInfo() {
    ;
}

void SeDemoProcInfo::dummy() {}

SeDemoSituationInfo* SeDemoSituationInfo::createInfo(const ByamlIter& iter, const char* name,
                                                     s32 triggerFrame, s32 endFrame) {
    SeDemoSituationInfo* demoSituationInfo = new SeDemoSituationInfo();
    demoSituationInfo->name = name;
    demoSituationInfo->triggerFrame = triggerFrame;
    demoSituationInfo->endFrame = endFrame;

    if (!iter.tryGetStringByKey(&demoSituationInfo->situationName, "SituationName"))
        demoSituationInfo->situationName = nullptr;

    if (!iter.tryGetIntByKey(&demoSituationInfo->startFadeFrameNum, "StartFadeFrameNum"))
        demoSituationInfo->startFadeFrameNum = -1;

    if (!iter.tryGetIntByKey(&demoSituationInfo->endFadeFrameNum, "EndFadeFrameNum"))
        demoSituationInfo->endFadeFrameNum = -1;

    return demoSituationInfo;
}

s32 SeDemoSituationInfo::compareInfo(const SeDemoSituationInfo* lhs,
                                     const SeDemoSituationInfo* rhs) {
    return SeDemoProcInfo::compareInfo(lhs, rhs);
}

SeDemoSituationInfo::SeDemoSituationInfo() = default;

SeDemoSituationInfo::SeDemoSituationInfo(const SeDemoSituationInfo& demoSituationInfo)
    : SeDemoProcInfo(demoSituationInfo), situationName(demoSituationInfo.situationName),
      startFadeFrameNum(demoSituationInfo.startFadeFrameNum),
      endFadeFrameNum(demoSituationInfo.endFadeFrameNum) {}

SeDemoPauseInfo* SeDemoPauseInfo::createInfo(const ByamlIter& iter, const char* name,
                                             s32 triggerFrame, s32 endFrame) {
    SeDemoPauseInfo* demoPauseInfo = new SeDemoPauseInfo();
    demoPauseInfo->name = name;
    demoPauseInfo->triggerFrame = triggerFrame;
    demoPauseInfo->endFrame = endFrame;

    if (!iter.tryGetIntByKey(&demoPauseInfo->fadeOutFrameNum, "FadeOutFrameNum"))
        demoPauseInfo->fadeOutFrameNum = 0;

    if (!iter.tryGetIntByKey(&demoPauseInfo->fadeInFrameNum, "FadeInFrameNum"))
        demoPauseInfo->fadeInFrameNum = 0;

    return demoPauseInfo;
}

s32 SeDemoPauseInfo::compareInfo(const SeDemoPauseInfo* lhs, const SeDemoPauseInfo* rhs) {
    return SeDemoProcInfo::compareInfo(lhs, rhs);
}

SeDemoPauseInfo::SeDemoPauseInfo() = default;

SeDemoPauseInfo::SeDemoPauseInfo(const SeDemoPauseInfo& demoPauseInfo)
    : SeDemoProcInfo(demoPauseInfo), fadeOutFrameNum(demoPauseInfo.fadeOutFrameNum),
      fadeInFrameNum(demoPauseInfo.fadeInFrameNum) {}

SeDemoStopOneShotInfo* SeDemoStopOneShotInfo::createInfo(const ByamlIter& iter, const char* name,
                                                         s32 triggerFrame, s32 endFrame) {
    SeDemoStopOneShotInfo* demoStopOneShotInfo = new SeDemoStopOneShotInfo();
    demoStopOneShotInfo->name = name;
    demoStopOneShotInfo->triggerFrame = triggerFrame;
    demoStopOneShotInfo->endFrame = endFrame;

    if (!iter.tryGetIntByKey(&demoStopOneShotInfo->fadeOutFrameNum, "FadeOutFrameNum"))
        demoStopOneShotInfo->fadeOutFrameNum = 45;

    return demoStopOneShotInfo;
}

s32 SeDemoStopOneShotInfo::compareInfo(const SeDemoStopOneShotInfo* lhs,
                                       const SeDemoStopOneShotInfo* rhs) {
    return SeDemoProcInfo::compareInfo(lhs, rhs);
}

SeDemoStopOneShotInfo::SeDemoStopOneShotInfo() = default;

SeDemoStopOneShotInfo::SeDemoStopOneShotInfo(const SeDemoStopOneShotInfo& demoStopOneShotInfo)
    : SeDemoProcInfo(demoStopOneShotInfo), fadeOutFrameNum(demoStopOneShotInfo.fadeOutFrameNum) {}

SeDemoListenerPoserInfo* SeDemoListenerPoserInfo::createInfo(const ByamlIter& iter,
                                                             const char* name, s32 triggerFrame,
                                                             s32 endFrame) {
    SeDemoListenerPoserInfo* info = new SeDemoListenerPoserInfo();
    info->name = name;
    info->triggerFrame = triggerFrame;
    info->endFrame = endFrame;

    if (!iter.tryGetStringByKey(&info->poserName, "PoserName"))
        info->name = nullptr;  // BUG: This should be info->poserName = nullptr;

    if (!iter.tryGetIntByKey(&info->fadeOutFrameNum, "FadeOutFrameNum"))
        info->fadeOutFrameNum = 0;

    if (!iter.tryGetIntByKey(&info->fadeInFrameNum, "FadeInFrameNum"))
        info->fadeInFrameNum = 0;

    return info;
}

s32 SeDemoListenerPoserInfo::compareInfo(const SeDemoListenerPoserInfo* lhs,
                                         const SeDemoListenerPoserInfo* rhs) {
    return SeDemoProcInfo::compareInfo(lhs, rhs);
}

SeDemoListenerPoserInfo::SeDemoListenerPoserInfo() = default;

SeDemoListenerPoserInfo::SeDemoListenerPoserInfo(
    const SeDemoListenerPoserInfo& demoListenerPoserInfo)
    : SeDemoProcInfo(demoListenerPoserInfo), poserName(demoListenerPoserInfo.poserName),
      fadeOutFrameNum(demoListenerPoserInfo.fadeOutFrameNum),
      fadeInFrameNum(demoListenerPoserInfo.fadeInFrameNum) {}

SeDemoPlaySeInfo* SeDemoPlaySeInfo::createInfo(const ByamlIter& iter, const char* name,
                                               s32 triggerFrame, s32 endFrame) {
    SeDemoPlaySeInfo* demoPlaySeInfo = new SeDemoPlaySeInfo();
    demoPlaySeInfo->name = name;
    demoPlaySeInfo->triggerFrame = triggerFrame;
    demoPlaySeInfo->endFrame = endFrame;

    iter.tryGetStringByKey(&demoPlaySeInfo->playName, "SePlayName");
    iter.tryGetBoolByKey(&demoPlaySeInfo->isStopSe, "IsStopSe");
    iter.tryGetIntByKey(&demoPlaySeInfo->fadeOutFrameNum, "FadeOutFrameNum");
    iter.tryGetBoolByKey(&demoPlaySeInfo->isStartSeEvenIfDemoSkip, "IsStartSeEvenIfDemoSkip");
    iter.tryGetBoolByKey(&demoPlaySeInfo->isNotStopSeEvenIfDemoSkip, "IsNotStopSeEvenIfDemoSkip");

    return demoPlaySeInfo;
}

s32 SeDemoPlaySeInfo::compareInfo(const SeDemoPlaySeInfo* lhs, const SeDemoPlaySeInfo* rhs) {
    return SeDemoProcInfo::compareInfo(lhs, rhs);
}

SeDemoPlaySeInfo::SeDemoPlaySeInfo() = default;

SeDemoPlaySeInfo::SeDemoPlaySeInfo(const SeDemoPlaySeInfo& demoPlaySeInfo)
    : SeDemoProcInfo(demoPlaySeInfo), isStopSe(demoPlaySeInfo.isStopSe),
      playName(demoPlaySeInfo.playName), fadeOutFrameNum(demoPlaySeInfo.fadeOutFrameNum),
      isStartSeEvenIfDemoSkip(demoPlaySeInfo.isStartSeEvenIfDemoSkip),
      isNotStopSeEvenIfDemoSkip(demoPlaySeInfo.isNotStopSeEvenIfDemoSkip) {}

SeDemoSyncedProcInfo* SeDemoSyncedProcInfo::createInfo(const ByamlIter& iter) {
    SeDemoSyncedProcInfo* demoSyncedProcInfo = new SeDemoSyncedProcInfo();

    if (iter.tryGetStringByKey(&demoSyncedProcInfo->name, "Name")) {
        if (!iter.tryGetBoolByKey(&demoSyncedProcInfo->isEnableEvent, "IsEnableEvent"))
            demoSyncedProcInfo->isEnableEvent = false;

        ByamlIter partsProcNameIter;
        s32 partsProcNameSize = 0;
        bool hasList = iter.tryGetIterByKey(&partsProcNameIter, "PartsProcNameList");
        if (hasList)
            partsProcNameSize = partsProcNameIter.getSize();

        ByamlIter demoProcInfoIter;
        if (iter.tryGetIterByKey(&partsProcNameIter, "DemoProcInfoList")) {
            demoSyncedProcInfo->procInfoList =
                createAudioInfoList<SeDemoProcInfo>(demoProcInfoIter, partsProcNameSize);
        } else if (hasList) {
            demoSyncedProcInfo->procInfoList = new AudioInfoListWithParts<SeDemoProcInfo>();
        }

        ByamlIter demoEndProcInfoIter;
        if (iter.tryGetIterByKey(&partsProcNameIter, "DemoEndProcInfoList")) {
            demoSyncedProcInfo->procEndInfoList =
                createAudioInfoList<SeDemoProcInfo>(demoEndProcInfoIter, partsProcNameSize);
        } else if (hasList) {
            demoSyncedProcInfo->procEndInfoList = new AudioInfoListWithParts<SeDemoProcInfo>();
        }

        iter.tryGetBoolByKey(&demoSyncedProcInfo->isPartsProc, "IsPartsProc");
        if (!demoSyncedProcInfo->isPartsProc) {
            iter.tryGetBoolByKey(&demoSyncedProcInfo->isDisableAudioMaximizer,
                                 "IsDisableAudioMaximizer");
            iter.tryGetBoolByKey(&demoSyncedProcInfo->isNotStopOneShotSe, "IsNotStopOneShotSe");
            iter.tryGetIntByKey(&demoSyncedProcInfo->oneShotSeFadeOutFrameNum,
                                "OneShotSeFadeOutFrameNum");
        }
    }

    return demoSyncedProcInfo;
}

s32 SeDemoSyncedProcInfo::compareInfo(const SeDemoSyncedProcInfo* lhs,
                                      const SeDemoSyncedProcInfo* rhs) {
    return strcmp(lhs->name, rhs->name);
}

SeDemoSyncedProcInfo::SeDemoSyncedProcInfo() = default;

SeDemoSyncedProcInfo::SeDemoSyncedProcInfo(const SeDemoSyncedProcInfo& demoSyncedProcInfo) {}
}  // namespace al
