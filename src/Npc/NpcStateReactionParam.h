#pragma once

#include <prim/seadSafeString.h>

class NpcStateReactionParam {
public:
    NpcStateReactionParam();
    NpcStateReactionParam(const char*, const char*);
    ~NpcStateReactionParam();

    sead::FixedSafeString<64> mReactionAnim;
    sead::FixedSafeString<64> mReactionEndAnim;
};

static_assert(sizeof(NpcStateReactionParam) == 0xB0);
