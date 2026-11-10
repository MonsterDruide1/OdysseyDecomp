#pragma once

#include <math/seadVector.h>

namespace al {

class PadRumbleKeeper {
public:
    PadRumbleKeeper(s32 port);

    s32 getPort() const { return mPort; }
    void setPort(s32 port) { mPort = port; }

private:
    s32 mPort;
};

}  // namespace al
