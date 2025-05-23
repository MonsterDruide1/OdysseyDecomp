#include "System/GameSystem.h"

#include "Library/Nerve/NerveSetupUtil.h"

namespace {
NERVE_IMPL(GameSystem, Play);

NERVES_MAKE_STRUCT(GameSystem, Play);
}  // namespace

// TODO: Remove this once this class is implemented and the nerves are used
inline void dummy() {
    (void)NrvGameSystem;
}
