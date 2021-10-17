#pragma once

#include <prim/seadSafeString.h>
#include "al/resource/Resource.h"

namespace al {

Resource* findOrCreateResource(const sead::SafeString&, const char*);

}
