#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <prim/seadSafeString.h>

#include "System/ByamlSave.h"
#include "Util/SequentialUtil.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

class RaceRecord : public ByamlSave {
public:
    RaceRecord();
    void setName(const char* name);
    const char* getName() const;
    void init();
    bool isEqualName(const char* name);
    void setRecord(s32 record, s32 bestRecord, s32 lapRecord, bool isWin);
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

    s32 getRecord() const { return mRecord; }

    s32 getBestRecord() const { return mBestRecord; }

    s32 getLapRecord() const { return mLapRecord; }

    bool isExistRecord() const { return mIsExistRecord; }

    bool isRecordSet() const { return mIsRecordSet; }

private:
    s32 mRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    s32 mBestRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    s32 mLapRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    bool mIsWin = false;
    bool mIsExistRecord = false;
    bool mIsRecordSet = false;
    sead::FixedSafeString<0x20> mName;
};
