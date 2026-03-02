#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;
class AudioInfoListCreateFunctorBase;
}  // namespace al

namespace alAudioInfoListFunction {
s32 getCreateAudioInfoListSize(const al::ByamlIter&, s32);
s32 getCreateAudioInfoListSize(const al::ByamlIter&, const al::ByamlIter&);
void createAudioInfoAndSetToList(al::AudioInfoListCreateFunctorBase*, const al::ByamlIter&);
}  // namespace alAudioInfoListFunction
