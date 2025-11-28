#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class LayoutInitInfo;
}

class GameDataHolder;

class SaveDataAccessSequence : public al::NerveExecutor {
public:
    SaveDataAccessSequence(GameDataHolder*, const al::LayoutInitInfo&);

    void update();
    bool isDone() const;
    void startInit();
    void startInitSync();
    void startReadSync();
    void startReadAll();
    void addRequest(sead::FixedSafeString<32>*, sead::FixedSafeString<32>*, s32);
    void getFileNamePtrByIndex(s32) const;
    void startWrite();
    void getPlayingFileNamePtr() const;
    void getFileNameCommon() const;
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
    void getNextFileNameSrc() const;
    void getNextFileNameDest() const;
    void clearCurrentRequest();
    void tryChangeNextNerve();
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
