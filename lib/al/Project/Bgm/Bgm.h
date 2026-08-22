#pragma once

#include <basis/seadTypes.h>

namespace al {

class Bgm {
public:
    const char* getRunningResourceName();
    s64 getCurSamplePosition() const;
};

}  // namespace al
