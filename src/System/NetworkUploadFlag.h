#pragma once

#include "System/ByamlSave.h"
#include "System/RankingCategory.h"

class NetworkUploadFlag : public ByamlSave {
public:
    NetworkUploadFlag();
    void init();
    bool isNeedUpload(const RankingCategory& category) const;
    void upload(const RankingCategory& category);
    void updateScore(const RankingCategory& category);
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& iter) override;

private:
    void* _padding;
};

static_assert(sizeof(NetworkUploadFlag) == 0x10);
