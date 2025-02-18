#include "Item/CoinCollectHintState.h"

#include <math/seadVector.h>

#include "Library/Camera/CameraUtil.h"
#include "Library/Effect/EffectSystemInfo.h"
#include "Library/LiveActor/ActorPoseUtil.h"
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
    appearHintEffect();
}

void CoinCollectHintState::kill() {
    al::deleteEffect(mActor, "Emission");
    setDead(true);
}

void CoinCollectHintState::deleteHintEffect() {
    al::tryKillEmitterAndParticleAll(mActor);
}

void CoinCollectHintState::appearHintEffect() {
    al::emitEffect(mActor, "Emission", nullptr);
}

void CoinCollectHintState::exeWait() {
    al::LiveActor* actor = mActor;
    sead::Vector3f cameraDiff = al::getCameraPos(actor, 0) - al::getTrans(actor);
    f32 distanceToCamera = cameraDiff.length();
    if (distanceToCamera > 3500.0f) {
        f32 scale = distanceToCamera / 3500.0f;
        al::setEffectAllScale(actor, "Emission", sead::Vector3f{scale, scale, scale});
    }
}
