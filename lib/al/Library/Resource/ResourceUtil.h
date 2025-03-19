#pragma once

#include <prim/seadSafeString.h>

namespace al {
class Resource;

Resource* findOrCreateResource(const sead::SafeString&, const char*);

}  // namespace al
