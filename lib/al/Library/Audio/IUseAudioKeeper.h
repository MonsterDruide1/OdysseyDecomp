#pragma once

namespace al {
class AudioKeeper;

class IUseAudioKeeper {
public:
    virtual AudioKeeper* getAudioKeeper() const = 0;
};

void notifyDemoSkipToDemoSyncedProc(const IUseAudioKeeper* user);
}  // namespace al
