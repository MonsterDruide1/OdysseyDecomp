#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Nerve/NerveExecutor.h"

class GameDataHolder;
class SaveDataAccessor;

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
    void startWrite();
    void startWriteSync();
    void startWriteWithWindow();
    void startCopyWithWindow(s32, s32);
    void startDeleteWithWindow(s32);
    void addRequest(sead::FixedSafeString<32>*, sead::FixedSafeString<32>*, s32);
    void clearAllRequest();
    void clearCurrentRequest();
    bool isExistRequest() const;
    sead::FixedSafeString<32>* getFileNamePtrByIndex(s32) const;
    sead::FixedSafeString<32>* getPlayingFileNamePtr() const;
    sead::FixedSafeString<32>* getFileNameCommon() const;
    bool isNextFileCommon() const;
    sead::FixedSafeString<32>* getNextFileNameSrc() const;
    sead::FixedSafeString<32>* getNextFileNameDest() const;
    bool isEnableSave() const;
    bool isDoneSave() const;
    void setWindowSave();
    void setWindowSwitch();
    void setWindowDelete();
    void exeIdle();
    void exeInit();
    void exeWaitLayout();
    void exeWrite();
    void exeFlush();
    void exeRead();
    bool tryChangeNextNerve();

private:
    char filler_8[0x50];
};

static_assert(sizeof(SaveDataAccessSequence) == 0x60);
