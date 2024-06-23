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
    // TODO: currently a workaround because the game calls operator new[]
    mEntries = new SearchAmiiboDataTableEntries[1];
}

void SearchAmiiboDataTable::init() {
    for (s32 i = 0; i < 3; i++)
        mEntries->e[i].init();
}

void SearchAmiiboDataTable::write(al::ByamlWriter* writer) {
    writer->pushArray("SearchAmiiboData");

    writer->pushHash();
    writer->addInt("Id", mEntries->e[0].mId);
    writer->addInt("NumberingId", mEntries->e[0].mNumberingId);
    writer->addUInt64("SearchStartTime", mEntries->e[0].mSearchStartTime);
    writer->pop();

    writer->pushHash();
    writer->addInt("Id", mEntries->e[1].mId);
    writer->addInt("NumberingId", mEntries->e[1].mNumberingId);
    writer->addUInt64("SearchStartTime", mEntries->e[1].mSearchStartTime);
    writer->pop();

    writer->pushHash();
    writer->addInt("Id", mEntries->e[2].mId);
    writer->addInt("NumberingId", mEntries->e[2].mNumberingId);
    writer->addUInt64("SearchStartTime", mEntries->e[2].mSearchStartTime);
    writer->pop();

    writer->pop();
}

void SearchAmiiboDataTable::read(al::ByamlIter const& iter) {
    init();

    al::ByamlIter dataIter;
    iter.tryGetIterByKey(&dataIter, "SearchAmiiboData");

    for (s32 i = 0; i < dataIter.getSize(); i++) {
        al::ByamlIter indexIter;
        dataIter.tryGetIterByIndex(&indexIter, i);

        indexIter.tryGetIntByKey(&this->mEntries->e[i].mId, "Id");
        indexIter.tryGetIntByKey(&this->mEntries->e[i].mNumberingId, "NumberingId");
        indexIter.tryGetUInt64ByKey(&this->mEntries->e[i].mSearchStartTime, "SearchStartTime");
    }
}

s32 SearchAmiiboDataTable::getDataNumMax() const {
    return mEntries->e.size();
}

bool SearchAmiiboDataTable::isInvalidId(s32 index) const {
    return this->mEntries->e[index].mId == -1;
}

const SearchAmiiboData& SearchAmiiboDataTable::getData(s32 index) const {
    return this->mEntries->e[index];
}

s32 SearchAmiiboDataTable::getId(s32 index) const {
    return this->mEntries->e[index].mId;
}

s32 SearchAmiiboDataTable::getNumberingId(s32 index) const {
    return this->mEntries->e[index].mNumberingId;
}

void SearchAmiiboDataTable::setId(s32 id, s32 numberingId, s32 index) {
    this->mEntries->e[index].mId = id;
    this->mEntries->e[index].mNumberingId = numberingId;
}

u64 SearchAmiiboDataTable::getSearchStartTime(s32 index) const {
    return this->mEntries->e[index].mSearchStartTime;
}

void SearchAmiiboDataTable::setSearchStartTime(u64 searchStartTime, s32 index) {
    this->mEntries->e[index].mSearchStartTime = searchStartTime;
}

void SearchAmiiboDataTable::copy(s32 index, s32 other) {
    this->mEntries->e[index] = this->mEntries->e[other];
}

void SearchAmiiboDataTable::initByIndex(s32 index) {
    this->mEntries->e[index].init();
}