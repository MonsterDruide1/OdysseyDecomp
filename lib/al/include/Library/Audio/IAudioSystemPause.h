#pragma once

namespace al {

class IAudioSystemPause {
    virtual void pauseSystem(bool, char const*, bool, f32, bool, bool, bool) = 0;
};

}  // namespace al
