#pragma once

namespace al {
class ByamlIter;

class ActorScoreKeeper {
public:
    struct Entry {
        const char* factorName;
        const char* categoryName;
    };

    ActorScoreKeeper();

    void init(const ByamlIter& iter);
    void getCategoryName();  // unknown return type
    const char* tryGetCategoryName(const char* a1);

private:
    inline void allocArray();
    inline void putEntry(int index, const ByamlIter& iter);

    Entry* array;
    int size;
};
};  // namespace al
