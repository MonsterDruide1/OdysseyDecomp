#include "Library/Se/VolumeKeeper.h"

#include <basis/seadTypes.h>

#include "Library/Audio/AudioEffectLevelDetector.h"
#include "Project/Audio/System/AudioPlayer.h"

namespace al {
SeGroupVolumeKeeper::SeGroupVolumeKeeper(AudioEffectLevelDetector* levelDirector)
    : mLevelDetector(levelDirector) {}

static inline bool isInAudibleRange(f32 mDecibel) {
    return mDecibel > -32.0f;
}

void SeGroupVolumeKeeper::update(bool b11) {
    _11 = b11;
    if (mLevelDetector) {
        mRatio = mLevelDetector->getLevel().length();
        mDecibel = calcRatioToDecibel(mRatio);
    }
}

f32 SeGroupVolumeKeeper::getVolumeAtmos() const {
    f32 volume = 1.0f;

    if (!_11 && _10)
        volume = 0.7f;
    if (mDecibel > -25.0f)
        return 0.7f * volume;
    if (mDecibel > -32.0f)
        return 0.8f * volume;
    return volume;
}

f32 SeGroupVolumeKeeper::getVolumeAtmosObj() const {
    f32 volume = 1.0f;

    if (!_11 && _10)
        volume = 0.8f;
    if (mDecibel > -25.0f)
        return 0.8f * volume;
    if (mDecibel > -32.0f)
        return 0.85f * volume;
    return volume;
}

f32 SeGroupVolumeKeeper::getVolumePlayerVoice() const {
    return mDecibel > -25.0f || isInAudibleRange(mDecibel) ? 1.0f : 0.85f;
}

f32 SeGroupVolumeKeeper::getVolumePlayer() const {
    return mDecibel > -25.0f || isInAudibleRange(mDecibel) ? 1.0f : 0.9f;
}
}  // namespace al
