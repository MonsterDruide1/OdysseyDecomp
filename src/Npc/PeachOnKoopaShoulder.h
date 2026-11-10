#pragma once

#include "Library/LiveActor/LiveActor.h"

// Minimal header: only the parts Koopa needs so far.
// The class itself (Npc/PeachOnKoopaShoulder.o) is still NotDecompiled.
class PeachOnKoopaShoulder : public al::LiveActor {
public:
    PeachOnKoopaShoulder(const char*);

    void validateAppearTiaraEyes();
    void invalidateAppearTiaraEyes();
};
