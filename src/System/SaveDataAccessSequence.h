#pragma once

#include <basis/seadTypes.h>

class SaveDataAccessSequence {
public:
    void startSaveDataInit();
    void startSaveDataInitSync();
    void startSaveDataLoadFile();
    void startSaveDataReadSync();
    void startSaveDataReadAll();
    void startSaveDataWrite();
    void startSaveDataWriteWithWindow();
    void startSaveDataCopyWithWindow(s32, s32);
    void startSaveDataDeleteWithWindow(s32);
    void startSaveDataWriteSync();
    bool updateSaveDataAccess();
    bool isEnableSave() const;
    bool isDoneSave();
    void setWindowSave();
    void setWindowSwitch();
    void setWindowDelete();
};
