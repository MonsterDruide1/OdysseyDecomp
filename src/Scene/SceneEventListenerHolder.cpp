#include "Scene/SceneEventListenerHolder.h"

#include "Scene/SceneEventListener.h"

SceneEventListenerHolder::SceneEventListenerHolder(const char* name, s32 maxListenerCount)
    : mName(name), mMaxListenerCount(maxListenerCount) {
    mListeners = new SceneEventListener*[maxListenerCount];
}

void SceneEventListenerHolder::add(SceneEventListener* listener) {
    mListeners[mListenerCount] = listener;
    mListenerCount++;
}

void SceneEventListenerHolder::notify() {
    for (s32 i = 0; i < mListenerCount; i++)
        mListeners[i]->notify();
}
