#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>

// Minimal partial reconstruction of the player's separate-cap flag object.
// Only the first 4 bytes are known so far. They are accessed two ways in the
// original code, so they are exposed as a union:
// - as one word: the throw getters test bytes 0 and 2 together with a single
//   word load (and #0xff0000 / and #0xff), e.g. getThrowStayTimeMax and
//   getThrowRange. The combination "byte 2 == 0 and byte 0 != 0" is the
//   separate-throw state.
// - as individual bytes: updateSeparateMode reads bytes 1 and 2 with ldrb.
// The rest of the class layout is still unknown; this header only covers
// what HackCap needs. Field names are placeholders until the real ones are
// determined.
class PlayerSeparateCapFlag {
public:
    union {
        u32 _flagsWord;
        struct {
            bool _00;
            bool _01;
            bool _02;
            bool _03;
        };
    };
    // Offset vector read by HackCapStateHide::tryForceFollowSeparate and
    // passed to HackCapFunction::updateSeparateWaitMove (as the last arg):
    // a Vector3f at +0x04, rotated by the player quat and added to the
    // player position. Name is a placeholder.
    sead::Vector3f _4;
};
