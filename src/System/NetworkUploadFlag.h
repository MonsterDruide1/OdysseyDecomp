#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al
enum class RankingCategory : s32;

class NetworkUploadFlag : public ByamlSave {
public:
    NetworkUploadFlag();
    void init();
    bool isNeedUpload(const RankingCategory&) const;
    void upload(const RankingCategory&);
    void updateScore(const RankingCategory&);
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

    struct Entry {
        bool isNeedUpload;
        u64 lastUploadTime;
    };

private:
    Entry* mFlags;
};

static_assert(sizeof(NetworkUploadFlag) == 0x10);
