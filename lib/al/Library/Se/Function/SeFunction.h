#pragma once

namespace al {
class AudioKeeper;
class SeKeeper;
class IUseAudioKeeper;
}  // namespace al

namespace alSeFunction {
al::SeKeeper* tryGetSeKeeper(const al::IUseAudioKeeper* user);
}
