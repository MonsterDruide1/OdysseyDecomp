#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

struct SearchAmiiboData {
    SearchAmiiboData();
    void init();
    bool isInvalidId() const;

    s32 id;
    s32 numberingId;
    u64 searchStartTime;
};

class SearchAmiiboDataTable : public ByamlSave {
public:
    SearchAmiiboDataTable();

    void init();
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;
    s32 getDataNumMax() const;
    bool isInvalidId(s32 index) const;
    const SearchAmiiboData& getData(s32 index) const;
    s32 getId(s32 index) const;
    s32 getNumberingId(s32 index) const;
    void setId(s32 id, s32 numberingId, s32 index);
    u64 getSearchStartTime(s32 index) const;
    void setSearchStartTime(u64 searchStartTime, s32 index);
    void copy(s32 index, s32 other);
    void initByIndex(s32 index);

private:
    SearchAmiiboData* mEntries;
};
