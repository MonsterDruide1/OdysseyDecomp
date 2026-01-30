#include "Library/Audio/AudioInfo.h"

#include "Library/Yaml/ByamlIter.h"

namespace al {

template <typename T>
bool AudioInfoListCreateFunctor<T>::tryCreateAudioInfoAndSetToList(const ByamlIter& iter) {
    T* info = mCreateInfoFunc(iter);

    if (!info)
        return false;

    return mAudioInfoList->list->pushBack(info);
}

template <typename T>
s32 AudioInfoList<T>::compareInfoAndKey(const T* info, const char* key) {
    return strcmp(info->name, key);
}

}  // namespace al
