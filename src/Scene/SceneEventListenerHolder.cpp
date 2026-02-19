#include "Scene/SceneEventListenerHolder.h"

#include "Scene/SceneEventListener.h"

SceneEventListenerHolder::SceneEventListenerHolder(const char* name, s32 maxListenerCount)
    : name(name), maxListenerCount(maxListenerCount) {
    listeners = new SceneEventListener*[maxListenerCount];
}

void SceneEventListenerHolder::add(SceneEventListener* listener) {
    listeners[listenerCount] = listener;
    listenerCount++;
}

void SceneEventListenerHolder::notify() {
    for (s32 i = 0; i < listenerCount; i++)
        listeners[i]->notify();
}
