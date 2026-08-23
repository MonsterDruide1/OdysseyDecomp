#pragma once

#include "Library/Audio/IAudioFrameProcess.h"

namespace al {

class WaveVibrationHolder : public HioNode, public aal::IAudioFrameProcess {
public:
    WaveVibrationHolder(const GamePadSystem*);

private:
    char filler[0xd8];
};

}  // namespace al
