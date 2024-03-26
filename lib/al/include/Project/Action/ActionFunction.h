#pragma once

#include <prim/seadSafeString.h>

namespace al {
class ActionAnimCtrlInfo;
class ActionAnimDataInfo;
}  // namespace al

namespace alActionFunction {
const char* getAnimName(const al::ActionAnimCtrlInfo* infoCtrl,
                        const al::ActionAnimDataInfo* infoData);
}  // namespace alActionFunction
