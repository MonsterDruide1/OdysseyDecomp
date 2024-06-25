#pragma once

#include <basis/seadTypes.h>

namespace al {
class KeyRepeatCtrl {
public:
    KeyRepeatCtrl();

    void init(s32, s32);
    void reset();
    void update(bool, bool);
    bool isDown() const;
    bool isUp() const;

private:
    s32 mInitialMaxWait;
    s32 mMaxWait;
    s32 mUpCounter;
    s32 mDownCounter;
    bool mCounter;
};
}  // namespace al
