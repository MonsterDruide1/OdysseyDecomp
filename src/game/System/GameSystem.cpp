#include "game/System/GameSystem.h"

#include "Library/Nerve/NerveSetupUtil.h"

namespace {

NERVE_IMPL(GameSystem, Play);

struct {
    NERVE_MAKE(GameSystem, Play);
} NrvGameSystem;

}  // namespace

void GameSystem::init() {}
