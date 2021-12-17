#pragma once

namespace al {

class LiveActorFlag {
public:
    LiveActorFlag();

    bool val1 = true;
    bool val2 = false;
    bool val3 = true;
    bool val4 = false;
    bool val5 = false;
    bool val6 = false;
    bool val7 = true;
    bool val8 = false;
    bool val9 = false;
    bool val10 = false;
    bool val11 = true;
    bool val12 = true;
};
static_assert(sizeof(LiveActorFlag) == 0xC);

}  // namespace al
