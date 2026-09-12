#pragma once

#include <basis/seadTypes.h>

class ProjectSeNamedList {
public:
    ProjectSeNamedList();

    s32 getMuteSeInPVListSize();
    const char* const* getMuteSeInPVList();
};
