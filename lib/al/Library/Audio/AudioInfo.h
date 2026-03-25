#pragma once

#include <basis/seadTypes.h>
#include <container/seadPtrArray.h>

#include "Library/Audio/System/System.h"

namespace al {
class ByamlIter;
class AudioInfoListCreateFunctorBase;

template <typename T>
class AudioInfoList;

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

        return mAudioInfoList->setInfo(info);
    }

private:
    CreateInfoFunc mCreateInfoFunc;
    AudioInfoList<T>* mAudioInfoList;
};

template <typename T>
class AudioInfoList {
public:
    static s32 compareInfoAndKey(const T* info, const char* key) { return strcmp(info->name, key); }

    static s32 compareInfoAndId(const T* info, u32 id);

    AudioInfoList() = default;

    void init(s32 listSize) {
        mIsLinearSearch = false;
        mList = new sead::PtrArray<const T>();
        mList->allocBuffer((listSize == 0) ? 1 : listSize, nullptr);
    }

    bool setInfo(const T* info) const { return mList->pushBack(info); }

    const T* getInfoAt(s32 index) const {
        if (index >= 0)
            return mList->unsafeAt(index);
        return nullptr;
    }

    bool copyAndSetInfo(const T* info) const {
        if (!info)
            return false;

        T* newInfo = new T(*info);
        return setInfo(newInfo);
    }

    void sort() const {
        if (mList->size() >= 10)
            mList->heapSort(T::compareInfo);
        else
            mList->sort(T::compareInfo);
    }

    s32 getTotalSize() const { return mList->size(); }

    bool isValidIndex(s32 index) const { return index < getTotalSize(); }

private:
    sead::PtrArray<const T>* mList;
    bool mIsLinearSearch = false;
};

template <typename T>
class AudioInfoListWithParts : public AudioInfoList<T> {
public:
    AudioInfoListWithParts() = default;

    s32 tryGetInfoIndex(const char* key) const;
    const T* tryFindInfo(const char* key) const;

    void init(s32 listSize, s32 maxNumParts) {
        AudioInfoList<T>::init(listSize);

        mParts = nullptr;
        if (maxNumParts != 0) {
            mParts = new sead::PtrArray<AudioInfoList<T>>();
            mParts->allocBuffer(maxNumParts, nullptr);
        }
    }

    void sort() const {
        AudioInfoList<T>::sort();

        for (s32 i = 0; i < getPartsSize(); i++)
            mParts->at(i)->sort();
    }

    const T* getInfoAt(s32 index) const {
        if (AudioInfoList<T>::isValidIndex(index))
            return AudioInfoList<T>::getInfoAt(index);
        index -= AudioInfoList<T>::getTotalSize();

        for (s32 i = 0; i < getPartsSize(); i++) {
            const AudioInfoList<T>* part = mParts->at(i);
            if (part->isValidIndex(index))
                return part->getInfoAt(index);
            index -= part->getTotalSize();
        }

        return nullptr;
    }

    s32 getPartsSize() const {
        if (!mParts)
            return 0;
        return mParts->size();
    }

    s32 getTotalSize() const {
        if (!mParts)
            return AudioInfoList<T>::getTotalSize();

        s32 size = AudioInfoList<T>::getTotalSize();
        s32 partsSize = 0;
        for (s32 i = 0; i < getPartsSize(); i++)
            partsSize += mParts->at(i)->getTotalSize();

        return size + partsSize;
    }

private:
    sead::PtrArray<AudioInfoList<T>>* mParts;
};

template <typename T>
AudioInfoListWithParts<T>* createAudioInfoList(const ByamlIter& iter, s32 maxNumParts) {
    AudioInfoListWithParts<T>* audioInfoList = new AudioInfoListWithParts<T>;

    s32 listSize = alAudioInfoListFunction::getCreateAudioInfoListSize(iter, 0);
    audioInfoList->init(listSize, maxNumParts);

    AudioInfoListCreateFunctor<T> functor(audioInfoList, T::createInfo);
    alAudioInfoListFunction::createAudioInfoAndSetToList(&functor, iter);
    audioInfoList->sort();

    return audioInfoList;
}

template <typename T>
bool trySetAudioInfo(const AudioInfoListWithParts<T>* audioInfoList, const T* audioInfo,
                     bool disableSort) {
    if (!audioInfoList || !audioInfo)
        return false;

    if (!audioInfoList->setInfo(audioInfo))
        return false;

    if (!disableSort)
        audioInfoList->sort();

    return true;
}

template <typename T>
const T* tryFindAudioInfo(const AudioInfoListWithParts<T>* audioInfoList, const char* name) {
    if (!audioInfoList || !name)
        return nullptr;

    return audioInfoList->tryFindInfo(name);
}

template <typename T>
AudioInfoListWithParts<T>* copyAudioInfoList(const AudioInfoListWithParts<T>* audioInfoList,
                                             s32 additionalSize) {
    if (!audioInfoList)
        return nullptr;

    s32 originalSize = audioInfoList->getTotalSize();
    s32 newSize = originalSize + additionalSize;

    AudioInfoListWithParts<T>* newAudioInfoList = new AudioInfoListWithParts<T>;
    newAudioInfoList->init(newSize, 0);

    for (s32 i = 0; i < originalSize; i++) {
        const T* info = audioInfoList->getInfoAt(i);
        newAudioInfoList->copyAndSetInfo(info);
    }

    return newAudioInfoList;
}
}  // namespace al
