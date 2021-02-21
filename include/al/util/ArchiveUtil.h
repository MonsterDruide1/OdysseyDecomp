#pragma once

#include "sead/seadSafeString.h"
#include "sead/seadResource.h"

namespace al
{
    sead::ArchiveRes* loadArchive(const sead::SafeStringBase<char> &);
};