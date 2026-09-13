#pragma once

#include <basis/seadTypes.h>

namespace al {
struct BgmDirectorInitInfo {
    BgmDirectorInitInfo() {}

    bool isScene = false;
    const char* name = nullptr;
};
}  // namespace al
