#include "MapObj/FastenerKnob.h"

#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorAnimFunction.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorInitUtil.h"
#include "Library/LiveActor/ActorModelFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

#include "MapObj/Fastener.h"

namespace {
NERVE_IMPL(FastenerKnob, Wait)
NERVE_IMPL(FastenerKnob, HackEnd)
NERVE_IMPL(FastenerKnob, Reset)

NERVES_MAKE_NOSTRUCT(FastenerKnob, Wait, HackEnd, Reset)
}  // namespace

FastenerKnob::FastenerKnob(const char* name) : al::LiveActor(name) {}

void FastenerKnob::init(const al::ActorInitInfo& info) {
    al::initActorWithArchiveName(this, info, "Fastener", nullptr);
    al::initNerve(this, &Wait, 0);
    makeActorAlive();
}

void FastenerKnob::exeWait() {
    if (al::isFirstStep(this))
        al::startAction(this, "Standby");

    al::resetPosition(this, mCurrentVertex->trans);

    if (mCurrentVertex->_280 != 0) {
        if (mCurrentVertex->_26b)
            return FastenerFunction::resetKnobPose(this, mCurrentVertex);
        return;
    }

    if (mCurrentVertex->_26b)
        FastenerFunction::tryMakeQuatResetKnob(&mResetQuat, mCurrentVertex);

    sead::Quatf* quat = al::getQuatPtr(this);
    return al::slerpQuat(quat, *quat, mResetQuat, 0.2f);
}

void FastenerKnob::exeHackEnd() {
    if (al::isFirstStep(this))
        al::startAction(this, "HackEnd");

    if (mCurrentVertex->_280 == 0) {
        if (mCurrentVertex->_26b)
            FastenerFunction::tryMakeQuatResetKnob(&mResetQuat, mCurrentVertex);

        sead::Quatf* quat = al::getQuatPtr(this);
        al::slerpQuat(quat, *quat, mResetQuat, 0.2f);
    }

    if (al::isActionEnd(this))
        al::setNerve(this, &Wait);
}

void FastenerKnob::onHackEnd(const sead::Quatf& quat, const FastenerVertex* vertex) {
    mCurrentVertex = vertex;
    al::resetQuatPosition(this, quat, vertex->trans);
    al::setNerve(this, &HackEnd);
}

void FastenerKnob::reset() {
    al::startHitReaction(this, "消滅");
    al::hideSilhouetteModelIfShow(this);
    al::invalidateClipping(this);
    al::setNerve(this, &Reset);
}

void FastenerKnob::disappear() {
    al::startHitReaction(this, "消滅");
    return al::LiveActor::kill();
}

void FastenerKnob::exeReset() {
    if (!al::isGreaterStep(this, 1))
        return;

    mCurrentVertex = mStartVertex;
    FastenerFunction::resetKnobPose(this, mCurrentVertex);
    al::resetPosition(this, mCurrentVertex->trans);

    al::startHitReaction(this, "出現");
    al::validateClipping(this);
    al::showModelIfHide(this);
    al::setNerve(this, &Wait);
}

bool FastenerKnob::isEnableCapture() const {
    return al::isNerve(this, &Wait);
}
