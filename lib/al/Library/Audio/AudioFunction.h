#pragma once

namespace al {
class AudioKeeper;
class AudioDirector;
}  // namespace al

namespace alAudioFunction {
al::AudioKeeper* createAudioKeeper(const al::AudioDirector*, const char*, const char*);
}
