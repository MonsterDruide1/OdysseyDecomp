#pragma once

#include <basis/seadTypes.h>

#include "Library/HostIO/HioNode.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

struct SearchAmiiboData {
    SearchAmiiboData();
    void init();
    bool isInvalidId() const;

    s32 mId;
    s32 mNumberingId;
    u64 mSearchStartTime;
};

struct SearchAmiiboDataTableEntries {
    SearchAmiiboDataTableEntries() {
        for (auto& entry : e)
            entry.init();
    };

    SearchAmiiboData e[3];
};

class SearchAmiiboDataTable : public al::HioNode {
public:
    SearchAmiiboDataTable();

    void init();
    virtual void write(al::ByamlWriter* writer);
    virtual void read(const al::ByamlIter& iter);
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
    SearchAmiiboDataTableEntries* mEntries;
};
