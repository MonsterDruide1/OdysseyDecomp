#pragma once

namespace al {
class LiveActor;
}

class PlayerFunction {
public:
    static u32 getPlayerInputPort(const al::LiveActor*);
    static bool isPlayerDeadStatus(const al::LiveActor*);
};
