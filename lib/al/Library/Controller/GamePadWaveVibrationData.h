#pragma once

#include "Library/Audio/IAudioFrameProcess.h"

namespace al {

class WaveVibrationHolder : public al::HioNode, public aal::IAudioFrameProcess {
public:
    WaveVibrationHolder(const al::GamePadSystem*);

private:
    char filler[0xd8];
};

}  // namespace al
