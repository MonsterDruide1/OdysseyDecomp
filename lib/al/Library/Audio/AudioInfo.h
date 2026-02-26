#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Audio/System/System.h"

namespace al {
class ByamlIter;
class AudioInfoListCreateFunctorBase;

template <typename T>
struct AudioInfoList;

class AudioInfoListCreateFunctorBase {
public:
    AudioInfoListCreateFunctorBase() = default;

    virtual bool tryCreateAudioInfoAndSetToList(const ByamlIter&) = 0;
};

template <typename T>
class AudioInfoListCreateFunctor : public AudioInfoListCreateFunctorBase {
public:
    using CreateInfoFunc = T* (*)(const ByamlIter&);

    AudioInfoListCreateFunctor(AudioInfoList<T>* list, CreateInfoFunc fun)
        : mCreateInfoFunc(fun), mAudioInfoList(list) {}

    bool tryCreateAudioInfoAndSetToList(const ByamlIter& iter) override {
        T* info = mCreateInfoFunc(iter);

        if (!info)
            return false;

        return mAudioInfoList->list->pushBack(info);
    }

private:
    CreateInfoFunc mCreateInfoFunc;
    AudioInfoList<T>* mAudioInfoList;
};

template <typename T>
struct AudioInfoList {
    static s32 compareInfoAndKey(const T* info, const char* key) { return strcmp(info->name, key); }

    sead::PtrArray<T>* list;
};

template <typename T>
struct AudioInfoListWithParts : public AudioInfoList<T> {
    s32 tryGetInfoIndex(const char*) const;
    T* tryFindInfo(const char*) const;

    s32 getPartsSize() const {
        if (!parts)
            return 0;
        return parts->size();
    }

    bool _8;
    sead::PtrArray<AudioInfoList<T>>* parts;
};

template <typename T>
AudioInfoListWithParts<T>* createAudioInfoList(const ByamlIter& iter, s32 maxNumParts) {
    AudioInfoListWithParts<T>* audioInfoList = new AudioInfoListWithParts<T>;

    s32 listSize = alAudioInfoListFunction::getCreateAudioInfoListSize(iter, 0);
    audioInfoList->_8 = false;

    audioInfoList->list = new sead::PtrArray<T>();
    audioInfoList->list->allocBuffer((listSize == 0) ? 1 : listSize, nullptr);

    audioInfoList->parts = nullptr;
    if (maxNumParts != 0) {
        audioInfoList->parts = new sead::PtrArray<AudioInfoList<T>>();
        audioInfoList->parts->allocBuffer(maxNumParts, nullptr);
    }

    AudioInfoListCreateFunctor<T> functor(audioInfoList, T::createInfo);
    alAudioInfoListFunction::createAudioInfoAndSetToList(&functor, iter);

    if (audioInfoList->list->size() >= 10)
        audioInfoList->list->heapSort(T::compareInfo);
    else
        audioInfoList->list->sort(T::compareInfo);

    for (s32 i = 0; i < audioInfoList->getPartsSize(); i++) {
        sead::PtrArray<T>* partsList = audioInfoList->parts->at(i)->list;
        if (partsList->size() >= 10)
            partsList->heapSort(T::compareInfo);
        else
            partsList->sort(T::compareInfo);
    }
    return audioInfoList;
}

}  // namespace al
