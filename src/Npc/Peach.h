#pragma once

#include "Library/LiveActor/LiveActor.h"

// Note: only startWaitChurch and startWaitDemo are known so far; the rest
// of Peach's layout is still NotDecompiled. Size is from
// initAndCreateLinksPeach's operator new.
class Peach : public al::LiveActor {
public:
    Peach(const char* name);

    void startWaitChurch();
    void startWaitDemo(bool);

private:
    char filler_108[0x18];
};

static_assert(sizeof(Peach) == 0x120);
