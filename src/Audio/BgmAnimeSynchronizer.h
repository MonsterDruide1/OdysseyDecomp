#pragma once

#include <basis/seadTypes.h>

namespace al {
class ByamlIter;
class LiveActor;
}  // namespace al

// Minimal layout decoded from TalkNpc::endClipped (sets the byte at
// +0x28) and TalkNpc::exeEvent (tail-calls trySyncBgm when non-null).
// Full class lives in Audio/BgmAnimeSynchronizer.o, still NotDecompiled.
class BgmAnimeSynchronizer {
public:
    static BgmAnimeSynchronizer* tryCreate(al::LiveActor* actor, al::ByamlIter& iter);
    void trySyncBgm();

private:
    u8 _0[0x28];

public:
    bool _28;
};
