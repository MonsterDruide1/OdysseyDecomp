#include "Player/PlayerJudgeIsNerve.h"

#include "Library/Nerve/IUseNerve.h"
#include "Library/Nerve/NerveUtil.h"

PlayerJudgeIsNerve::PlayerJudgeIsNerve(const al::IUseNerve* nerveUser, const al::Nerve* nerve)
    : mNerveUser(nerveUser), mNerve(nerve) {}

bool PlayerJudgeIsNerve::judge() const {
    return al::isNerve(mNerveUser, mNerve);
}
