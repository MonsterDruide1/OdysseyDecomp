#include "Player/PlayerStateRun2D3D.h"

#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "Player/IPlayerModelChanger.h"
#include "Player/PlayerJudgeStartDash.h"
#include "Player/PlayerStateRun.h"
#include "Player/PlayerStateRun2D.h"

namespace {
NERVE_IMPL(PlayerStateRun2D3D, Run3D)
NERVE_IMPL(PlayerStateRun2D3D, Run2D)
NERVES_MAKE_NOSTRUCT(PlayerStateRun2D3D, Run3D, Run2D)
}  // namespace

PlayerStateRun2D3D::PlayerStateRun2D3D(al::LiveActor* player, const PlayerConst* pConst,
                                       const IUseDimension* dimension,
                                       const IPlayerModelChanger* modelChanger,
                                       const PlayerInput* input,
                                       const IUsePlayerCollision* collision,
                                       PlayerAnimator* animator)
    : al::ActorStateBase("走り[2D3D]", player), mConst(pConst), mModelChanger(modelChanger) {
    initNerve(&Run3D, 2);

    PlayerJudgeStartDash* judgeStartDash = new PlayerJudgeStartDash(input);
    mStateRun3D = new PlayerStateRun(player, mConst, input, collision, animator, judgeStartDash);
    mStateRun2D = new PlayerStateRun2D(player, mConst, input, collision, animator);
    al::initNerveState(this, mStateRun3D, &Run3D, "走り3D");
    al::initNerveState(this, mStateRun2D, &Run2D, "走り2D");
}

void PlayerStateRun2D3D::appear() {
    const IPlayerModelChanger* modelChanger = mModelChanger;
    al::NerveStateBase::appear();

    if (modelChanger->is2DModel())
        al::setNerve(this, &Run2D);
    else
        al::setNerve(this, &Run3D);
}

void PlayerStateRun2D3D::syncModel() {
    if (isDead())
        return;

    if (al::isNerve(this, &Run2D)) {
        if (mModelChanger->is2DModel())
            return;

        al::setNerve(this, &Run3D);
        return;
    }

    if (al::isNerve(this, &Run3D) && mModelChanger->is2DModel())
        al::setNerve(this, &Run2D);
}

void PlayerStateRun2D3D::exeRun3D() {
    if (al::updateNerveState(this))
        kill();
}

void PlayerStateRun2D3D::exeRun2D() {
    if (al::updateNerveState(this))
        kill();
}
