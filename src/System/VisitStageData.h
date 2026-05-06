#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

namespace sead {
template <s32 L>
class FixedSafeString;
}  // namespace sead

class VisitStageData : public ByamlSave {
public:
    VisitStageData();
    void init();
    bool checkAlreadyVisit(const char* stageName) const;
    void visit(const char* stageName);
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    sead::FixedSafeString<128>* mStageNames = nullptr;
    s32 mVisitStageNum = 0;
};

static_assert(sizeof(VisitStageData) == 0x18);
