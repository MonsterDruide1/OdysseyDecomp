#pragma once

#include <basis/seadTypes.h>

struct SceneEventListener;

struct SceneEventListenerHolder {
public:
    SceneEventListenerHolder(const char* name, s32 maxListenerCount);

    /**
     * @warning This function doesn't check maxListenerCount, possible buffer overflow.
     */
    void add(SceneEventListener* listener);
    void notify();

    const char* name;
    SceneEventListener** listeners;
    s32 listenerCount = 0;
    s32 maxListenerCount;
};
