#pragma once

namespace al {
class StageSwitchListener {
public:
    virtual void listenOn() = 0;
    virtual void listenOff() = 0;
};
}  // namespace al
