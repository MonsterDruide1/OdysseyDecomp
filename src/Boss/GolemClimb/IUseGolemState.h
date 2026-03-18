#pragma once

namespace al {
class LiveActor;
}

class IUseGolemState {
public:
    virtual ~IUseGolemState() = default;
    virtual void updateLookAt() = 0;
    virtual void throwSearchBomb() = 0;
    virtual void throwReflectBomb() = 0;
    virtual void throwTsukkun() = 0;
    virtual void startDemo() = 0;
    virtual al::LiveActor* getActor() = 0;
};
