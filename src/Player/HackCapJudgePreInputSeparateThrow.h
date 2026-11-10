#pragma once

#include "Player/IJudge.h"

// Minimal: only the IJudge base is established so far (used by
// HackCap::tryChangeSeparateThrow via rs::resetJudge). The original passes
// the judge pointer to resetJudge with no offset adjustment, so IJudge is
// the first base.
class HackCapJudgePreInputSeparateThrow : public IJudge {
};
