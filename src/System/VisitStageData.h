#pragma once

#include "System/ByamlSave.h"

class VisitStageData : public ByamlSave {
public:
    VisitStageData();
    void init();
    bool checkAlreadyVisit(const char* stage_name) const;
    void visit(const char* stage_name);
    void write(al::ByamlWriter* writer) override;
    void read(const al::ByamlIter& iter) override;

private:
    void* _padding[0x2];
};

static_assert(sizeof(VisitStageData) == 0x18);
