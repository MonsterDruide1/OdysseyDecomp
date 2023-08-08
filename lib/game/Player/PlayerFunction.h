#pragma once

namespace al {
class LiveActor;
}

class PlayerFunction {
public:
    static unsigned int getPlayerInputPort(const al::LiveActor*);
};
