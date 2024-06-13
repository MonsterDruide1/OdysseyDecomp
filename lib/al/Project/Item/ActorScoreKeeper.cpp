#include "Project/Item/ActorScoreKeeper.h"

#include "Library/Yaml/ByamlIter.h"

namespace al {
ActorScoreKeeper::ActorScoreKeeper() = default;

void ActorScoreKeeper::init(const ByamlIter& iter) {
    if (iter.isTypeArray()) {
        mSize = iter.getSize();
        allocArray();
        for (s32 i = 0; i < mSize; i++) {
            ByamlIter subIter;
            iter.tryGetIterByIndex(&subIter, i);
            putEntry(i, subIter);
        }
    } else {
        mSize = 1;
        allocArray();
        putEntry(0, iter);
    }
}

inline void ActorScoreKeeper::allocArray() {
    Entry* local_array = new Entry[mSize];
    if (mSize)
        memset(local_array, 0, sizeof(Entry) * mSize);
    mArray = local_array;
}

inline void ActorScoreKeeper::putEntry(s32 index, const ByamlIter& iter) {
    auto& entry = mArray[index];
    iter.tryGetStringByKey(&entry.factorName, "FactorName");
    iter.tryGetStringByKey(&entry.categoryName, "CategoryName");
}
}  // namespace al
