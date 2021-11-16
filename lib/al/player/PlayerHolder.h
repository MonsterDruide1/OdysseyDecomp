#pragma once

namespace al {

class PadRumbleKeeper;
class LiveActor;

class PlayerHolder {
public:
    PlayerHolder(int);
    LiveActor* getPlayer(int);
    int* getPlayerNum() const;
    int* getBufferSize() const;
    void registerPlayer(LiveActor*, PadRumbleKeeper*);
};
}  // namespace al