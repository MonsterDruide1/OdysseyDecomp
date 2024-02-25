#pragma once

class PlayerHackKeeper;

class IUsePlayerHack {
public:
    virtual PlayerHackKeeper* getPlayerHackKeeper() const = 0;
};
