#pragma once

class ActorDimensionKeeper;

class IUseDimension {
public:
    virtual ActorDimensionKeeper* getActorDimensionKeeper() const = 0;
};
