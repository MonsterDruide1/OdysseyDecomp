#include "System/RaceRecord.h"

#include "Library/Base/StringUtil.h"
#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

#include "Util/SequentialUtil.h"

RaceRecord::RaceRecord() {
    mName.clear();
}

void RaceRecord::setName(const char* name) {
    mName.format(name);
}

const char* RaceRecord::getName() const {
    return mName.cstr();
}

void RaceRecord::init() {
    mRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    mBestRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    mLapRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    mIsWin = false;
    mIsExistRecord = false;
    mIsRecordSet = false;
}

bool RaceRecord::isEqualName(const char* name) {
    return al::isEqualString(mName.cstr(), name);
}

void RaceRecord::setRecord(s32 record, s32 bestRecord, s32 lapRecord, bool isWin) {
    mIsExistRecord = true;
    mIsRecordSet = true;
    if (record < mRecord)
        mRecord = record;
    if (bestRecord < mBestRecord)
        mBestRecord = bestRecord;
    if (lapRecord < mLapRecord)
        mLapRecord = lapRecord;
    mIsWin = isWin;
}

void RaceRecord::write(al::ByamlWriter* writer) {
    writer->pushHash(mName.cstr());
    writer->addInt("Record", mRecord);
    writer->addInt("LapRecord", mLapRecord);
    writer->addBool("IsWin", mIsWin);
    writer->addBool("IsExistRecord", mIsExistRecord);
    writer->pop();
}

void RaceRecord::read(const al::ByamlIter& save) {
    mRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    mBestRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    mLapRecord = RaceTimeFunction::getRaceTimeMaxCsec();
    mIsWin = false;
    mIsExistRecord = false;
    mIsRecordSet = false;

    al::ByamlIter recordData{};

    save.tryGetIterByKey(&recordData, mName.cstr());
    recordData.tryGetIntByKey(&mRecord, "Record");
    recordData.tryGetIntByKey(&mLapRecord, "LapRecord");
    recordData.tryGetBoolByKey(&mIsWin, "IsWin");
    recordData.tryGetBoolByKey(&mIsExistRecord, "IsExistRecord");
}
