#pragma once

#include <prim/seadSafeString.h>

namespace al {
class IUseAudioKeeper;

bool tryStartSe(const IUseAudioKeeper*, const sead::SafeString&);
void startSe(const IUseAudioKeeper*, const sead::SafeString&);
}  // namespace al
