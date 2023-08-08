#pragma once

#include <prim/seadSafeString.h>
#include "al/Library/Resource/Resource.h"

namespace al {

Resource* findOrCreateResource(const sead::SafeString&, const char*);

}
