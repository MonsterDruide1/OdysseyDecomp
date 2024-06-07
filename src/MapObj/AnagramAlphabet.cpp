#include "MapObj/AnagramAlphabet.h"

#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_IMPL(AnagramAlphabet, Wait);
NERVE_IMPL(AnagramAlphabet, Complete);

struct {
    NERVE_MAKE(AnagramAlphabet, Wait);
    NERVE_MAKE(AnagramAlphabet, Complete);
} NrvAnagramAlphabet;

}  // namespace
