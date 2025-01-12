#include "Amiibo/SearchAmiiboDataTable.h"

#include <basis/seadTypes.h>

#include "Library/Yaml/ByamlIter.h"
#include "Library/Yaml/Writer/ByamlWriter.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

SearchAmiiboData::SearchAmiiboData() {
    init();
}

void SearchAmiiboData::init() {
    mId = -1;
    mNumberingId = -1;
    mSearchStartTime = 0;
}

bool SearchAmiiboData::isInvalidId() const {
    return mId == -1;
}

SearchAmiiboDataTable::SearchAmiiboDataTable() {
    mEntries = new SearchAmiiboData[3]();
}

void SearchAmiiboDataTable::init() {
    for (s32 i = 0; i < 3; i++)
        mEntries[i].init();
}

void SearchAmiiboDataTable::write(al::ByamlWriter* writer) {
    writer->pushArray("SearchAmiiboData");

    writer->pushHash();
    writer->addInt("Id", mEntries[0].mId);
    writer->addInt("NumberingId", mEntries[0].mNumberingId);
    writer->addUInt64("SearchStartTime", mEntries[0].mSearchStartTime);
    writer->pop();

    writer->pushHash();
    writer->addInt("Id", mEntries[1].mId);
    writer->addInt("NumberingId", mEntries[1].mNumberingId);
    writer->addUInt64("SearchStartTime", mEntries[1].mSearchStartTime);
    writer->pop();

    writer->pushHash();
    writer->addInt("Id", mEntries[2].mId);
    writer->addInt("NumberingId", mEntries[2].mNumberingId);
    writer->addUInt64("SearchStartTime", mEntries[2].mSearchStartTime);
    writer->pop();

    writer->pop();
}

void SearchAmiiboDataTable::read(const al::ByamlIter& iter) {
    init();

    al::ByamlIter dataIter;
    iter.tryGetIterByKey(&dataIter, "SearchAmiiboData");

    for (s32 i = 0; i < dataIter.getSize(); i++) {
        al::ByamlIter indexIter;
        dataIter.tryGetIterByIndex(&indexIter, i);

        indexIter.tryGetIntByKey(&mEntries[i].mId, "Id");
        indexIter.tryGetIntByKey(&mEntries[i].mNumberingId, "NumberingId");
        indexIter.tryGetUInt64ByKey(&mEntries[i].mSearchStartTime, "SearchStartTime");
    }
}

s32 SearchAmiiboDataTable::getDataNumMax() const {
    return 3;
}

bool SearchAmiiboDataTable::isInvalidId(s32 index) const {
    return mEntries[index].mId == -1;
}

const SearchAmiiboData& SearchAmiiboDataTable::getData(s32 index) const {
    return mEntries[index];
}

s32 SearchAmiiboDataTable::getId(s32 index) const {
    return mEntries[index].mId;
}

s32 SearchAmiiboDataTable::getNumberingId(s32 index) const {
    return mEntries[index].mNumberingId;
}

void SearchAmiiboDataTable::setId(s32 id, s32 numberingId, s32 index) {
    mEntries[index].mId = id;
    mEntries[index].mNumberingId = numberingId;
}

u64 SearchAmiiboDataTable::getSearchStartTime(s32 index) const {
    return mEntries[index].mSearchStartTime;
}

void SearchAmiiboDataTable::setSearchStartTime(u64 searchStartTime, s32 index) {
    mEntries[index].mSearchStartTime = searchStartTime;
}

void SearchAmiiboDataTable::copy(s32 index, s32 other) {
    mEntries[index] = mEntries[other];
}

void SearchAmiiboDataTable::initByIndex(s32 index) {
    mEntries[index].init();
}
