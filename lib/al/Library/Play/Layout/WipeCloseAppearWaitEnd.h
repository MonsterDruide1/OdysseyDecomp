#pragma once

#include "Library/Layout/LayoutActor.h"

namespace al {
class LayoutInitInfo;

class WipeCloseAppearWaitEnd : public LayoutActor {
public:
    WipeCloseAppearWaitEnd(const char* name, const char*, const LayoutInitInfo& info, const char*,
                           bool isLocalized);

    void startClose(s32);
    void startCloseEnd();
    void startOpen(s32);
    bool isCloseEnd() const;

    void exeCloseStart();
    void exeCloseWait();
    void exeCloseEnd();
    void exeWait();
    void exeOpen();

private:
    s32 mWaitDelay = -1;
    s32 _130;
};

static_assert(sizeof(WipeCloseAppearWaitEnd) == 0x138);
}  // namespace al
