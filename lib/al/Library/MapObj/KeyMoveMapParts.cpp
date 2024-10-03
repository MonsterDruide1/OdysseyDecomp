#include "Library/MapObj/KeyMoveMapParts.h"

#include "Library/Nerve/NerveSetupUtil.h"

namespace {
using namespace al;

NERVE_ACTION_IMPL(KeyMoveMapParts, StandBy)
NERVE_ACTION_IMPL(KeyMoveMapParts, Delay)
NERVE_ACTION_IMPL(KeyMoveMapParts, Wait)
NERVE_ACTION_IMPL(KeyMoveMapParts, MoveSign)
NERVE_ACTION_IMPL(KeyMoveMapParts, Move)
NERVE_ACTION_IMPL(KeyMoveMapParts, StopSign)
NERVE_ACTION_IMPL(KeyMoveMapParts, Stop)
}  // namespace

namespace al {
KeyMoveMapParts::KeyMoveMapParts(const char* name) : LiveActor(name) {}
}  // namespace al
