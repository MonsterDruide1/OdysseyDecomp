#pragma once

#include <prim/seadSafeString.h>

#include "Project/Action/ActionAnimCtrl.h"

namespace alActionFunction {
const char* getAnimName(const al::ActionAnimCtrlInfo* infoCtrl,
                        const al::ActionAnimDataInfo* infoData);
}  // namespace alActionFunction
