#include "Player/PlayerJudgeSameNerve.h"

#include "Library/Nerve/NerveUtil.h"

PlayerJudgeSameNerve::PlayerJudgeSameNerve(const al::IUseNerve* nerveUser, const al::Nerve* nerve)
    : mNerveUser(nerveUser), mNerve(nerve) {}

bool PlayerJudgeSameNerve::judge() const {
    return al::isNerve(mNerveUser, mNerve);
}

void PlayerJudgeSameNerve::reset() {}

void PlayerJudgeSameNerve::update() {}
