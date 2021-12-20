#pragma once

namespace al {
class PlayerHackKeeper;

class IUsePlayerHack {
public:
    virtual PlayerHackKeeper* getPlayerHackKeeper() const = 0;
};
}  // namespace al
