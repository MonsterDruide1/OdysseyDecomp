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
    void setName(const char*);
    const char* getName() const;
    void init();
    bool isEqualName(const char*);
    void setRecord(s32, s32, s32, bool);
    void write(al::ByamlWriter*) override;
    void read(const al::ByamlIter&) override;

private:
    s32 mRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    s32 mBestRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    s32 mLapRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    bool mIsWin = false;
    bool mIsExistRecord = false;
    bool mIsRecordSet = false;
    sead::FixedSafeString<0x20> mName;
};
