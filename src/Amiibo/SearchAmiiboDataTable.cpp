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
    id = -1;
    numberingId = -1;
    searchStartTime = 0;
}

bool SearchAmiiboData::isInvalidId() const {
    return id == -1;
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
    writer->addInt("Id", mEntries[0].id);
    writer->addInt("NumberingId", mEntries[0].numberingId);
    writer->addUInt64("SearchStartTime", mEntries[0].searchStartTime);
    writer->pop();

    writer->pushHash();
    writer->addInt("Id", mEntries[1].id);
    writer->addInt("NumberingId", mEntries[1].numberingId);
    writer->addUInt64("SearchStartTime", mEntries[1].searchStartTime);
    writer->pop();

    writer->pushHash();
    writer->addInt("Id", mEntries[2].id);
    writer->addInt("NumberingId", mEntries[2].numberingId);
    writer->addUInt64("SearchStartTime", mEntries[2].searchStartTime);
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

        indexIter.tryGetIntByKey(&mEntries[i].id, "Id");
        indexIter.tryGetIntByKey(&mEntries[i].numberingId, "NumberingId");
        indexIter.tryGetUInt64ByKey(&mEntries[i].searchStartTime, "SearchStartTime");
    }
}

s32 SearchAmiiboDataTable::getDataNumMax() const {
    return 3;
}

bool SearchAmiiboDataTable::isInvalidId(s32 index) const {
    return mEntries[index].id == -1;
}

const SearchAmiiboData& SearchAmiiboDataTable::getData(s32 index) const {
    return mEntries[index];
}

s32 SearchAmiiboDataTable::getId(s32 index) const {
    return mEntries[index].id;
}

s32 SearchAmiiboDataTable::getNumberingId(s32 index) const {
    return mEntries[index].numberingId;
}

void SearchAmiiboDataTable::setId(s32 id, s32 numberingId, s32 index) {
    mEntries[index].id = id;
    mEntries[index].numberingId = numberingId;
}

u64 SearchAmiiboDataTable::getSearchStartTime(s32 index) const {
    return mEntries[index].searchStartTime;
}

void SearchAmiiboDataTable::setSearchStartTime(u64 searchStartTime, s32 index) {
    mEntries[index].searchStartTime = searchStartTime;
}

void SearchAmiiboDataTable::copy(s32 index, s32 other) {
    mEntries[index] = mEntries[other];
}

void SearchAmiiboDataTable::initByIndex(s32 index) {
    mEntries[index].init();
}
