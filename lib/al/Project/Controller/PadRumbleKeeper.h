#pragma once

#include <math/seadVector.h>

namespace al {

class PadRumbleKeeper {
public:
    explicit PadRumbleKeeper(s32 port);

private:
    s32 mPort;
};

}  // namespace al
