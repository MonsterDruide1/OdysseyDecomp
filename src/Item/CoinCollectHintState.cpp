#include "Item/CoinCollectHintState.h"

#include <math/seadVector.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorPoseKeeper.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(CoinCollectHintState, Wait);

NERVES_MAKE_STRUCT(CoinCollectHintState, Wait);
}  // namespace

CoinCollectHintState::CoinCollectHintState(al::LiveActor* actor)
    : al::ActorStateBase("ヒント状態", actor) {}

void CoinCollectHintState::init() {
    initNerve(&NrvCoinCollectHintState.Wait, 0);
}

void CoinCollectHintState::appear() {
    al::setNerve(this, &NrvCoinCollectHintState.Wait);
    setDead(false);
    al::emitEffect(getActor(), "Emission", nullptr);
}

void CoinCollectHintState::kill() {
    al::deleteEffect(getActor(), "Emission");
    setDead(true);
}

void CoinCollectHintState::deleteHintEffect() {
    al::tryKillEmitterAndParticleAll(getActor());
}

void CoinCollectHintState::appearHintEffect() {
    al::emitEffect(getActor(), "Emission", nullptr);
}

void CoinCollectHintState::exeWait() {
    al::LiveActor* actor = getActor();
    const sead::Vector3f objectPos = al::getCameraPos(actor, 0) - al::getTrans(actor);
    const f32 length = objectPos.length();
    if (length > 3500.0f) {
        sead::Vector3f effectScale;
        effectScale.x = length / 3500.0f;
        effectScale.y = effectScale.x;
        effectScale.z = effectScale.x;
        al::setEffectAllScale(actor, "Emission", effectScale);
    }
}
