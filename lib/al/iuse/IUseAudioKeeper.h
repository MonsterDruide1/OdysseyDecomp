#pragma once

namespace al {
class AudioKeeper;

class IUseAudioKeeper {
public:
    virtual AudioKeeper* getAudioKeeper() const = 0;
};
}  // namespace al
