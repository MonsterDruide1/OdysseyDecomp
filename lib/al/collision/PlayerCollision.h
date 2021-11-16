#pragma once

class PlayerCollider;

class IUsePlayerCollision {
public:
    virtual PlayerCollider* getPlayerCollider() const = 0;
};
