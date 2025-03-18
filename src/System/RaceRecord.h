#pragma once

#include <basis/seadTypes.h>
#include <prim/seadSafeString.h>

#include "System/ByamlSave.h"
#include "Util/SequentialUtil.h"

class RaceRecord : public ByamlSave {
public:
    RaceRecord();
    void setName(const char* name);
    const char* getName() const;
    void init();
    bool isEqualName(const char* name);
    void setRecord(s32 record, s32 bestRecord, s32 lapRecord, bool isWin);
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& reader) override;

private:
    s32 mRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    s32 mBestRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    s32 mLapRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    bool mIsWin = false;
    bool mIsExistRecord = false;
    bool mIsRecordSet = false;
    sead::FixedSafeString<0x20> mName;
};
