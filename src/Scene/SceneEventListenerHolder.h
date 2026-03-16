#pragma once

#include <basis/seadTypes.h>

struct SceneEventListener;

class SceneEventListenerHolder {
public:
    SceneEventListenerHolder(const char* name, s32 maxListenerCount);

    /**
     * @warning This function doesn't check maxListenerCount, possible buffer overflow.
     */
    void add(SceneEventListener* listener);
    void notify();

    const char* getName() const { return mName; }

private:
    const char* mName;
    SceneEventListener** mListeners;
    s32 mListenerCount = 0;
    s32 mMaxListenerCount;
};
