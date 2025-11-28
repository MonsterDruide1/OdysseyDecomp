#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Nerve/NerveExecutor.h"

class GameDataHolder;

namespace al {
class LayoutInitInfo;
}

class SaveDataAccessSequence : public al::NerveExecutor {
public:
    SaveDataAccessSequence(GameDataHolder*, const al::LayoutInitInfo&);
    bool update();
    bool isDone() const;
    void startInit();
    void startInitSync();
    void startReadSync();
    void startReadAll();
    void addRequest(sead::FixedSafeString<32>*, sead::FixedSafeString<32>*, s32);
    sead::FixedSafeString<32>* getFileNamePtrByIndex(s32) const;
    void startWrite();
    sead::FixedSafeString<32>* getPlayingFileNamePtr() const;
    sead::FixedSafeString<32>* getFileNameCommon() const;
    void startWriteSync();
    void startWriteWithWindow();
    void startCopyWithWindow(s32, s32);
    void startDeleteWithWindow(s32);
    void exeIdle();
    void exeInit();
    void exeWaitLayout();
    void exeWrite();
    void clearAllRequest();
    bool isNextFileCommon() const;
    const char* getNextFileNameSrc() const;
    const char* getNextFileNameDest() const;
    void clearCurrentRequest();
    bool tryChangeNextNerve();
    void exeFlush();
    void exeRead();
    bool isEnableSave() const;
    bool isDoneSave() const;
    void setWindowSave();
    void setWindowSwitch();
    void setWindowDelete();
    bool isExistRequest() const;

    void setDevelop() { mIsDevelop = true; }

private:
    char filler[0x10];
    bool _20;
    bool mIsDevelop;
    char filler_22[0x3e];
};

static_assert(sizeof(SaveDataAccessSequence) == 0x60);
