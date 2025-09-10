#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al
enum class RankingCategory : s32;

// Unsure of the size of this class. Don't know if the member variable is a pointer to an array or
// just an array.
class NetworkUploadFlag : public ByamlSave {
public:
    NetworkUploadFlag();
    void init();
    bool isNeedUpload(const RankingCategory&) const;
    void upload(const RankingCategory&);
    void updateScore(const RankingCategory&);
    void write(al::ByamlWriter*) override;
    void read(const al::ByamlIter&) override;

private:
    void* _8;  // Seems to be an array of bools
};
