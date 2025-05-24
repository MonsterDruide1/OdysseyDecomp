#include "MapObj/AnagramAlphabet.h"

#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_IMPL(AnagramAlphabet, Wait);
NERVE_IMPL(AnagramAlphabet, Complete);

NERVES_MAKE_STRUCT(AnagramAlphabet, Wait, Complete);
}  // namespace

// TODO: Remove this once this class is implemented and the nerves are used
inline void dummy() {
    (void)NrvAnagramAlphabet;
}
