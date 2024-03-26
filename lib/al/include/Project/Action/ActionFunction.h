#pragma once

#include <prim/seadSafeString.h>

namespace al {
struct ActionAnimCtrlInfo;
struct ActionAnimDataInfo;
}  // namespace al

namespace alActionFunction {
const char* getAnimName(const al::ActionAnimCtrlInfo* infoCtrl,
                        const al::ActionAnimDataInfo* infoData);
}  // namespace alActionFunction
