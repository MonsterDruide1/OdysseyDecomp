#pragma once

namespace al {
class AudioKeeper;
class AudioDirector;
}

namespace alAudioFunction {
al::AudioKeeper* createAudioKeeper(const al::AudioDirector *, const char *, const char *);
}