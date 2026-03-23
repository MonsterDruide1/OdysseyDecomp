#include "Enemy/HosuiTrail.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/PartsConnector.h"
#include "Library/Collision/PartsConnectorUtil.h"
#include "Library/Collision/PartsMtxConnector.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

namespace {
NERVE_IMPL(HosuiTrail, Appear)
NERVE_IMPL(HosuiTrail, Disappear)
NERVES_MAKE_NOSTRUCT(HosuiTrail, Appear, Disappear)
}  // namespace

HosuiTrail::HosuiTrail() : al::LiveActor("ホウスイの移動痕") {}

void HosuiTrail::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "HosuiFootPrint", nullptr);
    al::initNerve(this, &Appear, 0);
    al::startMtpAnimAndSetFrameAndStop(this, "HosuiFootPrintTexPattern", 0.0f);
    makeActorDead();
    mConnector = new al::CollisionPartsConnector();
}

void HosuiTrail::appear() {
    al::LiveActor::appear();
    f32 frame = al::getRandom() < 0.5f ? 0.0f : 1.0f;
    al::startMtpAnimAndSetFrameAndStop(this, "HosuiFootPrintTexPattern", frame);
    al::setNerve(this, &Appear);
}

void HosuiTrail::disappear() {
    if (al::isAlive(this) && !al::isNerve(this, &Disappear))
        al::setNerve(this, &Disappear);
}

void HosuiTrail::setFollowCollisionParts(const al::CollisionParts* parts) {
    mConnector->setBaseQuatTrans(al::getQuat(this), al::getTrans(this));
    mConnector->init(&parts->getBaseMtx(), parts->getBaseInvMtx(), parts);
}

void HosuiTrail::exeAppear() {
    if (al::isFirstStep(this))
        al::startAction(this, "HosuiFootPrintAppear");
    if (!mConnector->isConnecting()) {
        kill();
        return;
    }
    if (al::isGreaterEqualStep(this, 300))
        al::setNerve(this, &Disappear);
}

void HosuiTrail::exeDisappear() {
    if (al::isFirstStep(this))
        al::startAction(this, "HosuiFootPrintDisappear");
    if (al::isActionEnd(this))
        kill();
}

void HosuiTrail::control() {
    if (mConnector->isMoved())
        al::connectPoseQT(this, mConnector);
}
