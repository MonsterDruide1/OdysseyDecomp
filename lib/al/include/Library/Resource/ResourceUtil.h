#pragma once

#include <prim/seadSafeString.h>
#include "Library/Resource/Resource.h"

namespace al {

Resource* findOrCreateResource(const sead::SafeString&, const char*);

}
