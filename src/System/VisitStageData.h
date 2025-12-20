#pragma once

#include <basis/seadTypes.h>

#include "System/ByamlSave.h"

namespace al {
class ByamlIter;
class ByamlWriter;
}  // namespace al

class VisitStageData : public ByamlSave {
public:
    VisitStageData();
    void init();
    bool checkAlreadyVisit(const char*) const;
    void visit(const char*);
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& save) override;

private:
    void* _8;
    s32 _10;
};

static_assert(sizeof(VisitStageData) == 0x18);
