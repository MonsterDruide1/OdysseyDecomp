#pragma once

namespace al {

class IAudioSystemPause {
public:
    virtual void pauseSystem(bool, const char*, bool, f32, bool, bool, bool) = 0;
};

}  // namespace al
